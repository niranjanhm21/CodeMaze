#include "LevelManager.h"

LevelManager::LevelManager()
    : currentLevel(1),
    levels
    {
        {
            1,
            10,
            10,
            45,
            4,
            "Beginner"
        },

        {
            2,
            15,
            15,
            35,
            6,
            "Intermediate"
        },

        {
            3,
            20,
            20,
            30,
            8,
            "Advanced"
        },

        {
            4,
            25,
            25,
            25,
            10,
            "Expert"
        }
    }
{
}

void LevelManager::reset()
{
    currentLevel = 1;
}

void LevelManager::setLevel(
    int level
)
{
    if (level < 1)
    {
        level = 1;
    }

    if (level > 4)
    {
        level = 4;
    }

    currentLevel = level;
}

int LevelManager::getCurrentLevel() const
{
    return currentLevel;
}

const LevelSettings&
LevelManager::getCurrentSettings() const
{
    return levels[
        currentLevel - 1
    ];
}

int LevelManager::getTotalLevels() const
{
    return 4;
}