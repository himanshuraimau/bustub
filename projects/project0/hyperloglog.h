#ifndef HYPERLOGLOG_H
#define HYPERLOGLOG_H

#include <vector>
#include <cmath>
#include <bitset>
#include <cstdint>
#include <algorithm>  // Added for std::count

class HyperLogLog {
public:
    HyperLogLog(int initial_bits);
    void AddElem(uint64_t val);
    double GetCardinality();
    double ComputeCardinality();

private:
    int b; // Number of leading bits
    int m; // Number of registers (2^b)
    std::vector<int> registers; // Array of registers

    uint64_t CalculateHash(uint64_t val);
    std::string ComputeBinary(uint64_t hash);
    int PositionOfLeftmostOne(const std::string& binary);
};

#endif // HYPERLOGLOG_H
