// Directive similar to include guards, but simpler. It signals the 
// preprocessor to include this source file only once in compilation.
// Design to fight circular imports and other issues
#pragma once

// header of standard or provided libraries
#include <stdint.h>

enum Entity
{
    CLIENT,
    SERVER
};


class Seeds{
    public:
        Seeds(Entity hdlr) noexcept;
        

        void refresh_seeds();

        uint64_t get_seed_pe();
        uint64_t get_seed_mux();
        void set_seed_pe(uint64_t newSeed);
        void set_seed_mux(uint64_t newSeed);

        volatile uint64_t seedPe;
        volatile uint64_t seedMux;
        Entity handler;

    private:

        static const uint32_t seedPeSize = 1024;
        static const uint32_t seedMuxSize = 26;

        static uint64_t generate_random(uint32_t bitSize);

};