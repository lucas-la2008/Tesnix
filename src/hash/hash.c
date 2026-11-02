#include "hash/hash.h"

int64_t hash(const long valor) {
    uint64_t temp = (uint64_t)valor;
    for (int64_t i = 0; i <= 3; i++) {
        temp *= 0x9e3779b97f4a7c15ULL;
        temp = ~((temp << 9) ^ (temp >> 7) ^ temp);
    }
    return (int64_t)temp;
}