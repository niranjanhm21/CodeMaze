#pragma once

class ScoreManager
{
public:
    ScoreManager();

    void reset();

    void addCorrectAnswer(
        int basePoints
    );

    void addTimeBonus(
        int secondsRemaining
    );

    void addHintPenalty();

    int getScore() const;

private:
    int score;
};