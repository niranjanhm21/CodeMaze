#include "QuestionManager.h"

#include "Question.h"

#include "../Learning/AdaptiveEngine.h"

#include <chrono>
#include <fstream>
#include <stdexcept>

QuestionManager::QuestionManager()
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

bool QuestionManager::loadFromFile(
    const std::string& filename
)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    questions.clear();
    usedQuestions.clear();

    std::string line;

    while (std::getline(file, line))
    {
        if (
            line.empty() ||
            line[0] == '#'
            )
        {
            continue;
        }

        try
        {
            auto fields = split(line);

            if (fields.size() < 10)
            {
                continue;
            }

            auto question = createQuestion(fields);

            if (question)
            {
                questions.push_back(
                    std::move(question)
                );
            }
        }
        catch (const std::exception&)
        {
            continue;
        }
    }

    return !questions.empty();
}

const Question*
QuestionManager::getRandomQuestion()
{
    if (questions.empty())
    {
        return nullptr;
    }

    std::vector<const Question*> candidates;

    for (const auto& question : questions)
    {
        candidates.push_back(question.get());
    }

    const Question* selected =
        chooseRandomFromList(candidates);

    if (selected != nullptr)
    {
        return selected;
    }

    // All questions have been used.
    resetQuestionHistory();

    return chooseRandomFromList(candidates);
}

const Question*
QuestionManager::getRandomQuestion(
    const std::string& topic
)
{
    if (questions.empty())
    {
        return nullptr;
    }

    std::vector<const Question*> topicQuestions;

    for (const auto& question : questions)
    {
        if (question->getTopic() == topic)
        {
            topicQuestions.push_back(
                question.get()
            );
        }
    }

    if (topicQuestions.empty())
    {
        return nullptr;
    }

    const Question* selected =
        chooseRandomFromList(topicQuestions);

    if (selected != nullptr)
    {
        return selected;
    }

    // The requested topic is exhausted while
    // other questions are still unused.
    if (!allQuestionsUsed())
    {
        return getRandomQuestionWithoutReset();
    }

    // The complete question bank is exhausted.
    // Start a new cycle and keep the requested topic.
    resetQuestionHistory();

    return chooseRandomFromList(topicQuestions);
}

const Question*
QuestionManager::getAdaptiveQuestion(
    const PerformanceTracker& tracker
)
{
    if (questions.empty())
    {
        return nullptr;
    }

    AdaptiveEngine adaptiveEngine;

    const std::string preferredTopic =
        adaptiveEngine.chooseTopic(tracker);

    // Priority 1:
    // Adaptive topic + adaptive difficulty.
    if (!preferredTopic.empty())
    {
        std::vector<const Question*> topicQuestions;

        for (const auto& question : questions)
        {
            if (
                question->getTopic() ==
                preferredTopic
                )
            {
                topicQuestions.push_back(
                    question.get()
                );
            }
        }

        if (!topicQuestions.empty())
        {
            const Difficulty preferredDifficulty =
                adaptiveEngine.chooseDifficulty(
                    preferredTopic,
                    tracker
                );

            std::vector<const Question*>
                matchingDifficulty;

            for (const Question* question :
                topicQuestions)
            {
                if (
                    question->getDifficulty() ==
                    preferredDifficulty
                    )
                {
                    matchingDifficulty.push_back(
                        question
                    );
                }
            }

            const Question* selected =
                chooseRandomFromList(
                    matchingDifficulty
                );

            if (selected != nullptr)
            {
                return selected;
            }

            // Priority 2:
            // Same adaptive topic, any difficulty.
            selected =
                chooseRandomFromList(
                    topicQuestions
                );

            if (selected != nullptr)
            {
                return selected;
            }
        }
    }

    // Priority 3:
    // Any unused question.
    //
    // This prevents a small topic/difficulty pool
    // from causing repetition while other questions
    // are still available.
    const Question* selected =
        getRandomQuestionWithoutReset();

    if (selected != nullptr)
    {
        return selected;
    }

    // Every question in the bank has now been used.
    // Start a completely new cycle.
    resetQuestionHistory();

    // Preserve adaptive selection when the new cycle
    // begins.
    if (!preferredTopic.empty())
    {
        std::vector<const Question*> topicQuestions;

        for (const auto& question : questions)
        {
            if (
                question->getTopic() ==
                preferredTopic
                )
            {
                topicQuestions.push_back(
                    question.get()
                );
            }
        }

        if (!topicQuestions.empty())
        {
            const Difficulty preferredDifficulty =
                adaptiveEngine.chooseDifficulty(
                    preferredTopic,
                    tracker
                );

            std::vector<const Question*>
                matchingDifficulty;

            for (const Question* question :
                topicQuestions)
            {
                if (
                    question->getDifficulty() ==
                    preferredDifficulty
                    )
                {
                    matchingDifficulty.push_back(
                        question
                    );
                }
            }

            selected =
                chooseRandomFromList(
                    matchingDifficulty
                );

            if (selected != nullptr)
            {
                return selected;
            }

            selected =
                chooseRandomFromList(
                    topicQuestions
                );

            if (selected != nullptr)
            {
                return selected;
            }
        }
    }

    return getRandomQuestionWithoutReset();
}

std::size_t
QuestionManager::getQuestionCount() const
{
    return questions.size();
}

std::size_t
QuestionManager::getQuestionCountForTopic(
    const std::string& topic
) const
{
    std::size_t count = 0;

    for (const auto& question : questions)
    {
        if (question->getTopic() == topic)
        {
            ++count;
        }
    }

    return count;
}

std::vector<std::string>
QuestionManager::getAvailableTopics() const
{
    std::vector<std::string> topics;

    for (const auto& question : questions)
    {
        const std::string& topic =
            question->getTopic();

        bool alreadyExists = false;

        for (const auto& existing : topics)
        {
            if (existing == topic)
            {
                alreadyExists = true;
                break;
            }
        }

        if (!alreadyExists)
        {
            topics.push_back(topic);
        }
    }

    return topics;
}

void QuestionManager::resetQuestionHistory()
{
    usedQuestions.clear();
}

std::vector<std::string>
QuestionManager::split(
    const std::string& line
) const
{
    std::vector<std::string> fields;

    std::string current;

    for (char character : line)
    {
        if (character == '|')
        {
            fields.push_back(current);
            current.clear();
        }
        else
        {
            current += character;
        }
    }

    fields.push_back(current);

    return fields;
}

std::unique_ptr<Question>
QuestionManager::createQuestion(
    const std::vector<std::string>& fields
) const
{
    const std::string& type =
        fields[0];

    const std::string& topic =
        fields[1];

    const std::string& difficultyText =
        fields[2];

    const std::string& prompt =
        fields[3];

    const std::string& optionA =
        fields[4];

    const std::string& optionB =
        fields[5];

    const std::string& optionC =
        fields[6];

    const std::string& optionD =
        fields[7];

    const std::string& answer =
        fields[8];

    const std::string& hint =
        fields[9];

    Difficulty difficulty =
        Difficulty::Easy;

    if (difficultyText == "Medium")
    {
        difficulty =
            Difficulty::Medium;
    }
    else if (difficultyText == "Hard")
    {
        difficulty =
            Difficulty::Hard;
    }

    if (type == "MCQ")
    {
        return std::make_unique<
            MCQQuestion
        >(
            topic,
            prompt,
            std::vector<std::string>
        {
            optionA,
                optionB,
                optionC,
                optionD
        },
            answer,
            hint,
            difficulty
        );
    }

    if (type == "Output")
    {
        return std::make_unique<
            OutputQuestion
        >(
            topic,
            prompt,
            answer,
            hint,
            difficulty
        );
    }

    if (type == "Debugging")
    {
        return std::make_unique<
            DebugQuestion
        >(
            topic,
            prompt,
            answer,
            hint,
            difficulty
        );
    }

    if (type == "CodeCompletion")
    {
        return std::make_unique<
            CodeCompletionQuestion
        >(
            topic,
            prompt,
            answer,
            hint,
            difficulty
        );
    }

    if (type == "Conceptual")
    {
        return std::make_unique<
            ConceptualQuestion
        >(
            topic,
            prompt,
            answer,
            hint,
            difficulty
        );
    }

    if (type == "DataStructure")
    {
        return std::make_unique<
            DataStructureQuestion
        >(
            topic,
            prompt,
            answer,
            hint,
            difficulty
        );
    }

    if (type == "Algorithm")
    {
        return std::make_unique<
            AlgorithmQuestion
        >(
            topic,
            prompt,
            answer,
            hint,
            difficulty
        );
    }

    return nullptr;
}

std::vector<const Question*>
QuestionManager::getUnusedCandidates(
    const std::vector<const Question*>& candidates
) const
{
    std::vector<const Question*> unused;

    for (const Question* question : candidates)
    {
        if (
            usedQuestions.find(question) ==
            usedQuestions.end()
            )
        {
            unused.push_back(question);
        }
    }

    return unused;
}

const Question*
QuestionManager::chooseRandomFromList(
    const std::vector<const Question*>& candidates
)
{
    if (candidates.empty())
    {
        return nullptr;
    }

    const std::vector<const Question*> unused =
        getUnusedCandidates(candidates);

    if (unused.empty())
    {
        return nullptr;
    }

    std::uniform_int_distribution<std::size_t>
        distribution(
            0,
            unused.size() - 1
        );

    const Question* selected =
        unused[distribution(randomEngine)];

    markQuestionUsed(selected);

    return selected;
}

const Question*
QuestionManager::getRandomQuestionWithoutReset()
{
    if (questions.empty())
    {
        return nullptr;
    }

    std::vector<const Question*> candidates;

    for (const auto& question : questions)
    {
        candidates.push_back(question.get());
    }

    return chooseRandomFromList(candidates);
}

bool
QuestionManager::allQuestionsUsed() const
{
    return (
        !questions.empty() &&
        usedQuestions.size() >= questions.size()
        );
}

void
QuestionManager::markQuestionUsed(
    const Question* question
)
{
    if (question != nullptr)
    {
        usedQuestions.insert(question);
    }
}
