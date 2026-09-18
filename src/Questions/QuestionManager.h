#pragma once

#include "Question.h"

#include "../Learning/PerformanceTracker.h"

#include <memory>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

class QuestionManager
{
public:
    QuestionManager();

    bool loadFromFile(
        const std::string& filename
    );

    const Question*
        getRandomQuestion();

    const Question*
        getRandomQuestion(
            const std::string& topic
        );

    const Question*
        getAdaptiveQuestion(
            const PerformanceTracker& tracker
        );

    std::size_t
        getQuestionCount() const;

    std::size_t
        getQuestionCountForTopic(
            const std::string& topic
        ) const;

    std::vector<std::string>
        getAvailableTopics() const;

    void
        resetQuestionHistory();

private:
    std::vector<std::unique_ptr<Question>>
        questions;

    std::mt19937 randomEngine;

    std::unordered_set<const Question*>
        usedQuestions;

    std::vector<std::string>
        split(
            const std::string& line
        ) const;

    std::unique_ptr<Question>
        createQuestion(
            const std::vector<std::string>& fields
        ) const;

    std::vector<const Question*>
        getUnusedCandidates(
            const std::vector<const Question*>&
            candidates
        ) const;

    const Question*
        chooseRandomFromList(
            const std::vector<const Question*>&
            candidates
        );

    const Question*
        getRandomQuestionWithoutReset();

    bool
        allQuestionsUsed() const;

    void
        markQuestionUsed(
            const Question* question
        );
};
