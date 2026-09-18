#pragma once

#include <array>

struct Cell
{
    bool visited = false;

    // Order:
    // 0 = Top
    // 1 = Right
    // 2 = Bottom
    // 3 = Left
    std::array<bool, 4> walls{ true, true, true, true };
};