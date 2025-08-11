#pragma once


#include <random>
#include <cstdint>


class GlobalRandomGenerator {
public:
    static GlobalRandomGenerator& get() {
        static GlobalRandomGenerator s;
        return s;
    }
    uint32_t gen();
    bool gen(uint32_t percent);
private:
    std::mt19937 mt;

    GlobalRandomGenerator();
    ~GlobalRandomGenerator();

    GlobalRandomGenerator(GlobalRandomGenerator const&);
    GlobalRandomGenerator& operator= (GlobalRandomGenerator const&);
};