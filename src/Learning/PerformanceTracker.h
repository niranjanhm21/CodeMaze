#pragma once

#include <map>
#include <string>

struct TopicStatistics
{
    int attempted = 0;
    int correct = 0;
    int incorrect = 0;
    int hintsUsed = 0;
    int livesLost = 0;
    int totalTimeUsed = 0;

    double getAccuracy() const;

    double getAverageTime() const;
};

class PerformanceTracker
{
public:
    PerformanceTracker();

    void reset();

    void recordAttempt(
        const std::string& topic,
        bool correct,
        int timeUsed
    );

    void recordHint(
        const std::string& topic
    );

    void recordLifeLost(
        const std::string& topic
    );

    const std::map<std::string, TopicStatistics>&
        getTopicStatistics() const;

    const TopicStatistics*
        getStatisticsForTopic(
            const std::string& topic
        ) const;

    int getTotalAttempts() const;

    int getTotalCorrect() const;

    int getTotalIncorrect() const;

    double getOverallAccuracy() const;

private:
    std::map<std::string, TopicStatistics>
        topicStatistics;

    int totalAttempts;
    int totalCorrect;
    int totalIncorrect;
};