#pragma once

#include <string>

struct LevelSettings
{
    int levelNumber;

    int mazeRows;

    int mazeColumns;

    int challengeTime;

    int challengeCount;

    std::string name;
};

class LevelManager
{
public:
    LevelManager();

    void reset();

    void setLevel(
        int level
    );

    int getCurrentLevel() const;

    const LevelSettings&
        getCurrentSettings() const;

    int getTotalLevels() const;

private:
    int currentLevel;

    LevelSettings levels[4];
};