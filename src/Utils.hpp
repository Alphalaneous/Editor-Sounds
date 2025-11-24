#pragma once
#include <random>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace EditorSounds::utils {
    static int random(int lower, int upper) {
        if (lower > upper) std::swap(lower, upper);
        
        static std::random_device rd; 
        static std::mt19937 gen(rd()); 

        std::uniform_int_distribution<> dist(lower, upper);
        return dist(gen);
    }

    
}