#include "AdaptiveEngine.h"

#include <chrono>
#include <limits>
#include <vector>

AdaptiveEngine::AdaptiveEngine()
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

void AdaptiveEngine::reset()
{
    std::random_device randomDevice;

    randomEngine.seed(
        randomDevice()
    );
}

LearningLevel
AdaptiveEngine::getLearningLevel(
    const std::string& topic,
    const PerformanceTracker& tracker
) const
{
    const TopicStatistics* statistics =
        tracker.getStatisticsForTopic(
            topic
        );

    if (
        statistics == nullptr ||
        statistics->attempted <
        MIN_ATTEMPTS_FOR_CLASSIFICATION
        )
    {
        return LearningLevel::Normal;
    }

    double accuracy =
        statistics->getAccuracy();

    if (
        accuracy >= MASTERED_THRESHOLD
        )
    {
        return LearningLevel::Mastered;
    }

    if (
        accuracy >= NORMAL_THRESHOLD
        )
    {
        return LearningLevel::Normal;
    }

    return LearningLevel::Weak;
}

Difficulty
AdaptiveEngine::chooseDifficulty(
    const std::string& topic,
    const PerformanceTracker& tracker
)
{
    LearningLevel level =
        getLearningLevel(
            topic,
            tracker
        );

    return difficultyFromLevel(
        level
    );
}

std::string
AdaptiveEngine::chooseTopic(
    const PerformanceTracker& tracker
)
{
    const auto& statistics =
        tracker.getTopicStatistics();

    if (statistics.empty())
    {
        return "";
    }

    std::uniform_int_distribution<int>
        distribution(
            1,
            100
        );

    int roll =
        distribution(randomEngine);

    if (roll <= 60)
    {
        std::string weakTopic =
            chooseWeakTopic(
                tracker
            );

        if (!weakTopic.empty())
        {
            return weakTopic;
        }
    }

    if (roll <= 90)
    {
        std::string normalTopic =
            chooseNormalTopic(
                tracker
            );

        if (!normalTopic.empty())
        {
            return normalTopic;
        }
    }

    std::string masteredTopic =
        chooseMasteredTopic(
            tracker
        );

    if (!masteredTopic.empty())
    {
        return masteredTopic;
    }

    return chooseNormalTopic(
        tracker
    );
}

bool AdaptiveEngine::shouldPreferTopic(
    const std::string& topic,
    const PerformanceTracker& tracker
)
{
    LearningLevel level =
        getLearningLevel(
            topic,
            tracker
        );

    if (level == LearningLevel::Weak)
    {
        return true;
    }

    if (level == LearningLevel::Normal)
    {
        return true;
    }

    std::uniform_int_distribution<int>
        distribution(
            1,
            10
        );

    return distribution(
        randomEngine
    ) == 1;
}

std::string
AdaptiveEngine::chooseWeakTopic(
    const PerformanceTracker& tracker
) const
{
    std::vector<std::string> topics;

    for (
        const auto& [topic, statistics] :
        tracker.getTopicStatistics()
        )
    {
        if (
            statistics.attempted >=
            MIN_ATTEMPTS_FOR_CLASSIFICATION &&
            statistics.getAccuracy() <
            NORMAL_THRESHOLD
            )
        {
            topics.push_back(topic);
        }
    }

    if (topics.empty())
    {
        return "";
    }

    // Select the weakest topic.
    std::string selectedTopic =
        topics.front();

    double lowestAccuracy =
        std::numeric_limits<double>::max();

    for (const auto& topic : topics)
    {
        const TopicStatistics* statistics =
            tracker.getStatisticsForTopic(
                topic
            );

        if (
            statistics &&
            statistics->getAccuracy() <
            lowestAccuracy
            )
        {
            lowestAccuracy =
                statistics->getAccuracy();

            selectedTopic =
                topic;
        }
    }

    return selectedTopic;
}

std::string
AdaptiveEngine::chooseNormalTopic(
    const PerformanceTracker& tracker
) const
{
    for (
        const auto& [topic, statistics] :
        tracker.getTopicStatistics()
        )
    {
        LearningLevel level =
            getLearningLevel(
                topic,
                tracker
            );

        if (
            level == LearningLevel::Normal
            )
        {
            return topic;
        }
    }

    return "";
}

std::string
AdaptiveEngine::chooseMasteredTopic(
    const PerformanceTracker& tracker
) const
{
    for (
        const auto& [topic, statistics] :
        tracker.getTopicStatistics()
        )
    {
        LearningLevel level =
            getLearningLevel(
                topic,
                tracker
            );

        if (
            level == LearningLevel::Mastered
            )
        {
            return topic;
        }
    }

    return "";
}

Difficulty
AdaptiveEngine::difficultyFromLevel(
    LearningLevel level
)
{
    switch (level)
    {
    case LearningLevel::Weak:
        return Difficulty::Easy;

    case LearningLevel::Normal:
        return Difficulty::Medium;

    case LearningLevel::Mastered:
        return Difficulty::Hard;
    }

    return Difficulty::Medium;
}