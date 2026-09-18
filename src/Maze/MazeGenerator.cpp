#include "MazeGenerator.h"

#include <algorithm>
#include <chrono>

MazeGenerator::MazeGenerator()
{
    std::random_device randomDevice;

    randomEngine.seed(
        randomDevice() ^
        static_cast<unsigned int>(
            std::chrono::steady_clock::now()
            .time_since_epoch()
            .count()
            )
    );
}

std::vector<std::vector<Cell>> MazeGenerator::generate(
    int rows,
    int columns
)
{
    this->rows = rows;
    this->columns = columns;

    std::vector<std::vector<Cell>> grid(
        rows,
        std::vector<Cell>(columns)
    );

    carvePassages(grid, 0, 0);

    return grid;
}

void MazeGenerator::carvePassages(
    std::vector<std::vector<Cell>>& grid,
    int row,
    int column
)
{
    grid[row][column].visited = true;

    auto neighbours =
        getUnvisitedNeighbours(grid, row, column);

    std::shuffle(
        neighbours.begin(),
        neighbours.end(),
        randomEngine
    );

    for (const auto& neighbour : neighbours)
    {
        int nextRow = neighbour.first;
        int nextColumn = neighbour.second;

        if (!grid[nextRow][nextColumn].visited)
        {
            removeWall(
                grid,
                row,
                column,
                nextRow,
                nextColumn
            );

            carvePassages(
                grid,
                nextRow,
                nextColumn
            );
        }
    }
}

std::vector<std::pair<int, int>>
MazeGenerator::getUnvisitedNeighbours(
    const std::vector<std::vector<Cell>>& grid,
    int row,
    int column
)
{
    std::vector<std::pair<int, int>> neighbours;

    if (row > 0 &&
        !grid[row - 1][column].visited)
    {
        neighbours.push_back({ row - 1, column });
    }

    if (row < rows - 1 &&
        !grid[row + 1][column].visited)
    {
        neighbours.push_back({ row + 1, column });
    }

    if (column > 0 &&
        !grid[row][column - 1].visited)
    {
        neighbours.push_back({ row, column - 1 });
    }

    if (column < columns - 1 &&
        !grid[row][column + 1].visited)
    {
        neighbours.push_back({ row, column + 1 });
    }

    return neighbours;
}

void MazeGenerator::removeWall(
    std::vector<std::vector<Cell>>& grid,
    int currentRow,
    int currentColumn,
    int nextRow,
    int nextColumn
)
{
    // Moving UP
    if (nextRow == currentRow - 1)
    {
        grid[currentRow][currentColumn].walls[0] = false;
        grid[nextRow][nextColumn].walls[2] = false;
    }

    // Moving RIGHT
    else if (nextColumn == currentColumn + 1)
    {
        grid[currentRow][currentColumn].walls[1] = false;
        grid[nextRow][nextColumn].walls[3] = false;
    }

    // Moving DOWN
    else if (nextRow == currentRow + 1)
    {
        grid[currentRow][currentColumn].walls[2] = false;
        grid[nextRow][nextColumn].walls[0] = false;
    }

    // Moving LEFT
    else if (nextColumn == currentColumn - 1)
    {
        grid[currentRow][currentColumn].walls[3] = false;
        grid[nextRow][nextColumn].walls[1] = false;
    }
}