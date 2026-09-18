#pragma once

#include "PerformanceTracker.h"

#include "../Questions/Question.h"

#include <random>
#include <string>

enum class LearningLevel
{
    Weak,
    Normal,
    Mastered
};

class AdaptiveEngine
{
public:
    AdaptiveEngine();

    void reset();

    LearningLevel getLearningLevel(
        const std::string& topic,
        const PerformanceTracker& tracker
    ) const;

    Difficulty chooseDifficulty(
        const std::string& topic,
        const PerformanceTracker& tracker
    );

    std::string chooseTopic(
        const PerformanceTracker& tracker
    );

    bool shouldPreferTopic(
        const std::string& topic,
        const PerformanceTracker& tracker
    );

private:
    std::mt19937 randomEngine;

    static constexpr double MASTERED_THRESHOLD = 80.0;

    static constexpr double NORMAL_THRESHOLD = 50.0;

    static constexpr int MIN_ATTEMPTS_FOR_CLASSIFICATION = 2;

    std::string chooseWeakTopic(
        const PerformanceTracker& tracker
    ) const;

    std::string chooseNormalTopic(
        const PerformanceTracker& tracker
    ) const;

    std::string chooseMasteredTopic(
        const PerformanceTracker& tracker
    ) const;

    Difficulty difficultyFromLevel(
        LearningLevel level
    );
};