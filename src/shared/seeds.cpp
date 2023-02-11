// header of standard or provided libraries
#include <cassert>
#include <cstdlib>
#include <ctime>

// headers of local libraries
#include "seeds.h"


Seeds::Seeds(Entity hdlr) noexcept
{
    handler = hdlr;
}

void Seeds::refresh_seeds(){
    assert(handler == SERVER && "Only the Server should reset seeds"); // The client cannot generate seeds
    
    set_seed_mux(generate_random(seedMuxSize));
    set_seed_pe(generate_random(seedPeSize));
}

uint64_t Seeds::get_seed_pe()
{
    return seedPe;
}

uint64_t Seeds::get_seed_mux()
{
    return seedMux;
}

uint64_t Seeds::generate_random(uint32_t bitSize)
{
    srand(time(NULL));  // seed the random number generator
    /* Alternativelly
    for (int i = 0; i < 26; i++) {
        std::cout << rand() % 2 << " "; // generate a random 0 or 1
    }*/
    return rand() % bitSize;
}

void Seeds::set_seed_pe(uint64_t newSeed)
{
    assert(newSeed >= 0 && newSeed <= seedPeSize && "Value should be within bit size");
    seedPe = newSeed;
}

void Seeds::set_seed_mux(uint64_t newSeed)
{
    assert(newSeed >= 0 && newSeed <= seedMuxSize && "Value should be within bit size");
    seedMux = newSeed;
}
