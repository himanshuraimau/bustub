#include "hyperloglog.h"
#include <cmath>
#include <bitset>
#include <random>
#include <algorithm>  // Added for std::count

HyperLogLog::HyperLogLog(int initial_bits) {
    b = initial_bits;
    m = 1 << b;  // 2^b
    registers = std::vector<int>(m, 0);
}

uint64_t HyperLogLog::CalculateHash(uint64_t val) {
    // MurmurHash-like mixing for better distribution
    val ^= val >> 33;
    val *= 0xff51afd7ed558ccdULL;
    val ^= val >> 33;
    val *= 0xc4ceb9fe1a85ec53ULL;
    val ^= val >> 33;
    return val;
}

std::string HyperLogLog::ComputeBinary(uint64_t hash) {
    return std::bitset<64>(hash).to_string();
}

int HyperLogLog::PositionOfLeftmostOne(const std::string& binary) {
    for (size_t i = 0; i < binary.length(); i++) {
        if (binary[i] == '1') {
            return i + 1;  // 1-based position
        }
    }
    return 1;  // Return 1 if no 1s found
}

void HyperLogLog::AddElem(uint64_t val) {
    uint64_t hash = CalculateHash(val);
    
    // Use the first b bits as bucket index
    int bucket = hash & (m - 1);  // Faster than string manipulation
    
    // Count leading zeros in the remaining bits
    uint64_t remaining = hash >> b;
    int pos = 1;
    if (remaining != 0) {
        pos = 1 + __builtin_clzll(remaining);  // Use built-in leading zeros count
    }
    
    registers[bucket] = std::max(registers[bucket], pos);
}

double HyperLogLog::GetCardinality() {
    return ComputeCardinality();
}

double HyperLogLog::ComputeCardinality() {
    double sum = 0.0;
    for (int val : registers) {
        sum += std::pow(2.0, -val);
    }
    
    double alpha;
    switch (m) {
        case 16:
            alpha = 0.673;
            break;
        case 32:
            alpha = 0.697;
            break;
        case 64:
            alpha = 0.709;
            break;
        default:
            alpha = 0.7213 / (1.0 + 1.079 / m);
    }
    
    double estimate = alpha * m * m / sum;
    
    // Apply corrections for small and large ranges
    if (estimate <= 2.5 * m) {
        // Small range correction
        int zero_registers = std::count(registers.begin(), registers.end(), 0);
        if (zero_registers > 0) {
            estimate = m * std::log(static_cast<double>(m) / zero_registers);
        }
    } else if (estimate > (1.0/30.0) * std::pow(2, 64)) {
        // Large range correction
        estimate = -std::pow(2, 64) * std::log(1.0 - estimate/std::pow(2, 64));
    }
    
    return estimate;
}
