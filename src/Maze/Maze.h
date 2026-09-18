#pragma once

#include "Cell.h"
#include "MazeGenerator.h"

#include <SFML/Graphics.hpp>

#include <utility>
#include <vector>

class Maze
{
public:
    Maze();

    void configure(
        int rows,
        int columns,
        int challengeCount
    );

    void regenerate();

    void draw(
        sf::RenderWindow& window
    ) const;

    bool isWall(
        const sf::FloatRect& bounds
    ) const;

    sf::Vector2f
        getStartPosition() const;

    sf::FloatRect
        getExitBounds() const;

    bool hasValidPath() const;

    const std::vector<std::pair<int, int>>&
        getChallengeCells() const;

    sf::FloatRect getCellBounds(
        int row,
        int column
    ) const;

    int getRows() const;

    int getColumns() const;

private:
    int rows;

    int columns;

    int challengeCount;

    float cellSize;

    static constexpr float WALL_THICKNESS = 4.0f;

    std::vector<std::vector<Cell>>
        grid;

    MazeGenerator generator;

    sf::Vector2f startPosition;

    sf::Vector2f exitPosition;

    std::vector<std::pair<int, int>>
        challengeCells;

    void generateChallengeCells();

    void calculateStartAndExitPositions();

    float getPlayerSizeForCurrentMaze() const;

    void drawCell(
        sf::RenderWindow& window,
        int row,
        int column
    ) const;

    bool intersectsWall(
        const sf::FloatRect& bounds,
        int row,
        int column
    ) const;

    bool isInside(
        int row,
        int column
    ) const;
};