#include "Maze.h"

#include <algorithm>
#include <cmath>
#include <queue>
#include <random>
#include <utility>

Maze::Maze()
    : rows(10),
    columns(10),
    challengeCount(4),
    cellSize(60.0f)
{
    regenerate();
}

void Maze::configure(
    int newRows,
    int newColumns,
    int newChallengeCount
)
{
    rows = newRows;

    columns = newColumns;

    challengeCount =
        newChallengeCount;

    // Keep the complete maze
    // inside the 600 x 600
    // playable area.
    float width =
        600.0f /
        static_cast<float>(columns);

    float height =
        600.0f /
        static_cast<float>(rows);

    cellSize =
        std::min(width, height);
}

void Maze::regenerate()
{
    grid =
        generator.generate(
            rows,
            columns
        );

    calculateStartAndExitPositions();

    generateChallengeCells();
}

float Maze::getPlayerSizeForCurrentMaze() const
{
    /*
     * Keep this synchronized with
     * Game::startCurrentLevel().
     *
     * Level 1: 10 x 10 -> 40 px
     * Level 2: 15 x 15 -> 28 px
     * Level 3: 20 x 20 -> 22 px
     * Level 4: 25 x 25 -> 18 px
     *
     * The value is used only to calculate
     * a safe centered starting position.
     */

    if (rows <= 10)
    {
        return 40.0f;
    }

    if (rows <= 15)
    {
        return 28.0f;
    }

    if (rows <= 20)
    {
        return 22.0f;
    }

    return 18.0f;
}

void Maze::calculateStartAndExitPositions()
{
    /*
     * Start cell:
     * row    = 0
     * column = 0
     *
     * The player is placed in the exact
     * center of the starting cell.
     */
    float playerSize =
        getPlayerSizeForCurrentMaze();

    float startOffset =
        (cellSize - playerSize) / 2.0f;

    startPosition =
    {
        startOffset,
        startOffset
    };

    /*
     * Exit position refers to the top-left
     * corner of the final cell.
     *
     * Game checks intersection with the
     * complete exit cell using getExitBounds().
     */
    exitPosition =
    {
        (columns - 1) * cellSize,
        (rows - 1) * cellSize
    };
}

void Maze::generateChallengeCells()
{
    challengeCells.clear();

    std::vector<std::pair<int, int>>
        availableCells;

    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0;
            column < columns;
            ++column)
        {
            /*
             * Never place a challenge on
             * the start or exit cell.
             */
            if (
                (row == 0 && column == 0) ||
                (
                    row == rows - 1 &&
                    column == columns - 1
                    )
                )
            {
                continue;
            }

            availableCells.push_back(
                {
                    row,
                    column
                }
            );
        }
    }

    std::random_device randomDevice;

    std::mt19937 randomEngine(
        randomDevice()
    );

    std::shuffle(
        availableCells.begin(),
        availableCells.end(),
        randomEngine
    );

    int amount =
        std::min(
            challengeCount,
            static_cast<int>(
                availableCells.size()
                )
        );

    for (int i = 0; i < amount; ++i)
    {
        challengeCells.push_back(
            availableCells[i]
        );
    }
}

void Maze::draw(
    sf::RenderWindow& window
) const
{
    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0;
            column < columns;
            ++column)
        {
            sf::RectangleShape cellBackground;

            cellBackground.setSize(
                {
                    cellSize,
                    cellSize
                }
            );

            cellBackground.setPosition(
                {
                    column * cellSize,
                    row * cellSize
                }
            );

            if (row == 0 && column == 0)
            {
                cellBackground.setFillColor(
                    sf::Color(40, 100, 50)
                );
            }
            else if (
                row == rows - 1 &&
                column == columns - 1
                )
            {
                cellBackground.setFillColor(
                    sf::Color(180, 150, 40)
                );
            }
            else
            {
                cellBackground.setFillColor(
                    sf::Color(25, 25, 30)
                );
            }

            window.draw(
                cellBackground
            );
        }
    }

    for (
        const auto& challengeCell :
        challengeCells
        )
    {
        int row =
            challengeCell.first;

        int column =
            challengeCell.second;

        sf::RectangleShape
            challengeMarker;

        challengeMarker.setSize(
            {
                cellSize - 16.0f,
                cellSize - 16.0f
            }
        );

        challengeMarker.setPosition(
            {
                column * cellSize + 8.0f,
                row * cellSize + 8.0f
            }
        );

        challengeMarker.setFillColor(
            sf::Color(120, 60, 180)
        );

        window.draw(
            challengeMarker
        );
    }

    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0;
            column < columns;
            ++column)
        {
            drawCell(
                window,
                row,
                column
            );
        }
    }
}

void Maze::drawCell(
    sf::RenderWindow& window,
    int row,
    int column
) const
{
    const Cell& cell =
        grid[row][column];

    float x =
        column * cellSize;

    float y =
        row * cellSize;

    sf::RectangleShape wall;

    wall.setFillColor(
        sf::Color(70, 70, 80)
    );

    if (cell.walls[0])
    {
        wall.setSize(
            {
                cellSize,
                WALL_THICKNESS
            }
        );

        wall.setPosition(
            {
                x,
                y
            }
        );

        window.draw(wall);
    }

    if (cell.walls[1])
    {
        wall.setSize(
            {
                WALL_THICKNESS,
                cellSize
            }
        );

        wall.setPosition(
            {
                x + cellSize -
                    WALL_THICKNESS,
                y
            }
        );

        window.draw(wall);
    }

    if (cell.walls[2])
    {
        wall.setSize(
            {
                cellSize,
                WALL_THICKNESS
            }
        );

        wall.setPosition(
            {
                x,
                y + cellSize -
                    WALL_THICKNESS
            }
        );

        window.draw(wall);
    }

    if (cell.walls[3])
    {
        wall.setSize(
            {
                WALL_THICKNESS,
                cellSize
            }
        );

        wall.setPosition(
            {
                x,
                y
            }
        );

        window.draw(wall);
    }
}

bool Maze::isWall(
    const sf::FloatRect& bounds
) const
{
    int leftColumn =
        static_cast<int>(
            bounds.position.x /
            cellSize
            );

    int rightColumn =
        static_cast<int>(
            (
                bounds.position.x +
                bounds.size.x
                ) /
            cellSize
            );

    int topRow =
        static_cast<int>(
            bounds.position.y /
            cellSize
            );

    int bottomRow =
        static_cast<int>(
            (
                bounds.position.y +
                bounds.size.y
                ) /
            cellSize
            );

    for (
        int row = topRow;
        row <= bottomRow;
        ++row
        )
    {
        for (
            int column = leftColumn;
            column <= rightColumn;
            ++column
            )
        {
            if (
                !isInside(
                    row,
                    column
                )
                )
            {
                return true;
            }

            if (
                intersectsWall(
                    bounds,
                    row,
                    column
                )
                )
            {
                return true;
            }
        }
    }

    return false;
}

bool Maze::intersectsWall(
    const sf::FloatRect& bounds,
    int row,
    int column
) const
{
    const Cell& cell =
        grid[row][column];

    float x =
        column * cellSize;

    float y =
        row * cellSize;

    if (cell.walls[0])
    {
        sf::FloatRect wall(
            {
                x,
                y
            },
            {
                cellSize,
                WALL_THICKNESS
            }
        );

        if (
            bounds.findIntersection(
                wall
            )
            )
        {
            return true;
        }
    }

    if (cell.walls[1])
    {
        sf::FloatRect wall(
            {
                x + cellSize -
                    WALL_THICKNESS,
                y
            },
            {
                WALL_THICKNESS,
                cellSize
            }
        );

        if (
            bounds.findIntersection(
                wall
            )
            )
        {
            return true;
        }
    }

    if (cell.walls[2])
    {
        sf::FloatRect wall(
            {
                x,
                y + cellSize -
                    WALL_THICKNESS
            },
            {
                cellSize,
                WALL_THICKNESS
            }
        );

        if (
            bounds.findIntersection(
                wall
            )
            )
        {
            return true;
        }
    }

    if (cell.walls[3])
    {
        sf::FloatRect wall(
            {
                x,
                y
            },
            {
                WALL_THICKNESS,
                cellSize
            }
        );

        if (
            bounds.findIntersection(
                wall
            )
            )
        {
            return true;
        }
    }

    return false;
}

bool Maze::isInside(
    int row,
    int column
) const
{
    return
        row >= 0 &&
        row < rows &&
        column >= 0 &&
        column < columns;
}

sf::Vector2f
Maze::getStartPosition() const
{
    return startPosition;
}

sf::FloatRect
Maze::getExitBounds() const
{
    return sf::FloatRect(
        exitPosition,
        {
            cellSize,
            cellSize
        }
    );
}

bool Maze::hasValidPath() const
{
    if (
        grid.empty() ||
        rows <= 0 ||
        columns <= 0
        )
    {
        return false;
    }

    std::queue<
        std::pair<int, int>
    > positions;

    std::vector<
        std::vector<bool>
    > visited(
        rows,
        std::vector<bool>(
            columns,
            false
        )
    );

    positions.push(
        {
            0,
            0
        }
    );

    visited[0][0] = true;

    while (!positions.empty())
    {
        auto [row, column] =
            positions.front();

        positions.pop();

        if (
            row == rows - 1 &&
            column == columns - 1
            )
        {
            return true;
        }

        if (
            row > 0 &&
            !grid[row][column].walls[0] &&
            !visited[row - 1][column]
            )
        {
            visited[row - 1][column] = true;

            positions.push(
                {
                    row - 1,
                    column
                }
            );
        }

        if (
            column < columns - 1 &&
            !grid[row][column].walls[1] &&
            !visited[row][column + 1]
            )
        {
            visited[row][column + 1] = true;

            positions.push(
                {
                    row,
                    column + 1
                }
            );
        }

        if (
            row < rows - 1 &&
            !grid[row][column].walls[2] &&
            !visited[row + 1][column]
            )
        {
            visited[row + 1][column] = true;

            positions.push(
                {
                    row + 1,
                    column
                }
            );
        }

        if (
            column > 0 &&
            !grid[row][column].walls[3] &&
            !visited[row][column - 1]
            )
        {
            visited[row][column - 1] = true;

            positions.push(
                {
                    row,
                    column - 1
                }
            );
        }
    }

    return false;
}

const std::vector<std::pair<int, int>>&
Maze::getChallengeCells() const
{
    return challengeCells;
}

sf::FloatRect Maze::getCellBounds(
    int row,
    int column
) const
{
    return sf::FloatRect(
        {
            column * cellSize,
            row * cellSize
        },
        {
            cellSize,
            cellSize
        }
    );
}

int Maze::getRows() const
{
    return rows;
}

int Maze::getColumns() const
{
    return columns;
}