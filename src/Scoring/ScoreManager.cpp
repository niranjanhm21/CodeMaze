#include "ScoreManager.h"

ScoreManager::ScoreManager()
    : score(0)
{
}

void ScoreManager::reset()
{
    score = 0;
}

void ScoreManager::addCorrectAnswer(
    int basePoints
)
{
    score += basePoints;
}

void ScoreManager::addTimeBonus(
    int secondsRemaining
)
{
    if (secondsRemaining > 0)
    {
        score += secondsRemaining * 2;
    }
}

void ScoreManager::addHintPenalty()
{
    if (score >= 10)
    {
        score -= 10;
    }
    else
    {
        score = 0;
    }
}

int ScoreManager::getScore() const
{
    return score;
}