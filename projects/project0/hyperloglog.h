#include <openssl/sha.h>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

class HyperLogLog {
 private:
  int m;
  int b;
  vector<int> registers;

  using hash_t = uint64_t;

  bitset<64> ComputeBinary(hash_t hash);
  int PositionOfLeftmost1(bitset<64> binary_stream);
  hash_t CalculateHash(int val);  // Change parameter type to int

 public:
  HyperLogLog(int initial_bits);

  void AddElem(hash_t val);

  double ComputeCardinality();

  double GetCardinality();
};

HyperLogLog::HyperLogLog(int initial_bits) {
  b = initial_bits;
  m = pow(2, b);
  registers.resize(m, 0);
}

HyperLogLog::hash_t HyperLogLog::CalculateHash(int val) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, &val, sizeof(val));
  SHA256_Final(hash, &sha256);

  // Convert the first 8 bytes of the hash to a uint64_t
  hash_t result = 0;
  for (int i = 0; i < 8; ++i) {
    result = (result << 8) | hash[i];
  }
  return result;
}

void HyperLogLog::AddElem(hash_t val) {
  hash_t hash = CalculateHash(static_cast<int>(val));
  bitset<64> binary_stream = ComputeBinary(hash);

  int register_index = binary_stream.to_ulong() >> (64 - b);  // Fix variable name to binary_stream

  int p = PositionOfLeftmost1(binary_stream);

  if (registers[register_index] < p) {
    registers[register_index] = p;
  }
}

double HyperLogLog::ComputeCardinality() {}

double HyperLogLog::GetCardinality() {}