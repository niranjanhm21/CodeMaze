#pragma once

#include "Cell.h"

#include <vector>
#include <random>
#include <utility>

class MazeGenerator
{
public:
    MazeGenerator();

    std::vector<std::vector<Cell>> generate(
        int rows,
        int columns
    );

private:
    int rows;
    int columns;

    std::mt19937 randomEngine;

    void carvePassages(
        std::vector<std::vector<Cell>>& grid,
        int row,
        int column
    );

    std::vector<std::pair<int, int>> getUnvisitedNeighbours(
        const std::vector<std::vector<Cell>>& grid,
        int row,
        int column
    );

    void removeWall(
        std::vector<std::vector<Cell>>& grid,
        int currentRow,
        int currentColumn,
        int nextRow,
        int nextColumn
    );
};