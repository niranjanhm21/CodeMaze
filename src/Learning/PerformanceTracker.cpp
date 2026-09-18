#include "PerformanceTracker.h"

double TopicStatistics::getAccuracy() const
{
    if (attempted == 0)
    {
        return 0.0;
    }

    return
        static_cast<double>(correct) /
        static_cast<double>(attempted) *
        100.0;
}

double TopicStatistics::getAverageTime() const
{
    if (attempted == 0)
    {
        return 0.0;
    }

    return
        static_cast<double>(totalTimeUsed) /
        static_cast<double>(attempted);
}

PerformanceTracker::PerformanceTracker()
    : totalAttempts(0),
    totalCorrect(0),
    totalIncorrect(0)
{
}

void PerformanceTracker::reset()
{
    topicStatistics.clear();

    totalAttempts = 0;

    totalCorrect = 0;

    totalIncorrect = 0;
}

void PerformanceTracker::recordAttempt(
    const std::string& topic,
    bool correct,
    int timeUsed
)
{
    TopicStatistics& statistics =
        topicStatistics[topic];

    ++statistics.attempted;

    statistics.totalTimeUsed += timeUsed;

    ++totalAttempts;

    if (correct)
    {
        ++statistics.correct;

        ++totalCorrect;
    }
    else
    {
        ++statistics.incorrect;

        ++totalIncorrect;
    }
}

void PerformanceTracker::recordHint(
    const std::string& topic
)
{
    TopicStatistics& statistics =
        topicStatistics[topic];

    ++statistics.hintsUsed;
}

void PerformanceTracker::recordLifeLost(
    const std::string& topic
)
{
    TopicStatistics& statistics =
        topicStatistics[topic];

    ++statistics.livesLost;
}

const std::map<std::string, TopicStatistics>&
PerformanceTracker::getTopicStatistics() const
{
    return topicStatistics;
}

const TopicStatistics*
PerformanceTracker::getStatisticsForTopic(
    const std::string& topic
) const
{
    auto iterator =
        topicStatistics.find(topic);

    if (iterator == topicStatistics.end())
    {
        return nullptr;
    }

    return &iterator->second;
}

int PerformanceTracker::getTotalAttempts() const
{
    return totalAttempts;
}

int PerformanceTracker::getTotalCorrect() const
{
    return totalCorrect;
}

int PerformanceTracker::getTotalIncorrect() const
{
    return totalIncorrect;
}

double PerformanceTracker::getOverallAccuracy() const
{
    if (totalAttempts == 0)
    {
        return 0.0;
    }

    return
        static_cast<double>(totalCorrect) /
        static_cast<double>(totalAttempts) *
        100.0;
}