#include "Question.h"

#include <cctype>
#include <utility>

// --------------------------------------------------
// Question
// --------------------------------------------------

Question::Question(
    std::string topic,
    std::string prompt,
    std::string correctAnswer,
    std::string hint,
    Difficulty difficulty
)
    : topic(std::move(topic)),
    prompt(std::move(prompt)),
    correctAnswer(std::move(correctAnswer)),
    hint(std::move(hint)),
    difficulty(difficulty)
{
}

bool Question::isMultipleChoice() const
{
    return false;
}

std::vector<std::string>
Question::getOptions() const
{
    return {};
}

const std::string&
Question::getTopic() const
{
    return topic;
}

const std::string&
Question::getPrompt() const
{
    return prompt;
}

const std::string&
Question::getHint() const
{
    return hint;
}

Difficulty
Question::getDifficulty() const
{
    return difficulty;
}

std::string Question::difficultyToString(
    Difficulty difficulty
)
{
    switch (difficulty)
    {
    case Difficulty::Easy:
        return "Easy";

    case Difficulty::Medium:
        return "Medium";

    case Difficulty::Hard:
        return "Hard";
    }

    return "Unknown";
}

bool Question::matchesAcceptedAnswer(
    const std::string& answer
) const
{
    std::string normalizedAnswer =
        normalize(answer);

    auto acceptedAnswers =
        splitAcceptedAnswers(
            correctAnswer
        );

    for (
        const std::string& accepted :
        acceptedAnswers
        )
    {
        if (
            normalizedAnswer ==
            normalize(accepted)
            )
        {
            return true;
        }
    }

    return false;
}

std::vector<std::string>
Question::splitAcceptedAnswers(
    const std::string& answers
)
{
    std::vector<std::string>
        result;

    std::string current;

    for (char character : answers)
    {
        if (character == ';')
        {
            if (!current.empty())
            {
                result.push_back(
                    current
                );
            }

            current.clear();
        }
        else
        {
            current += character;
        }
    }

    if (!current.empty())
    {
        result.push_back(current);
    }

    return result;
}

std::string Question::normalize(
    const std::string& value
)
{
    std::string result;

    bool previousWasSpace = false;

    for (char character : value)
    {
        unsigned char c =
            static_cast<unsigned char>(
                character
                );

        if (std::isspace(c))
        {
            if (
                !result.empty() &&
                !previousWasSpace
                )
            {
                result += ' ';
            }

            previousWasSpace = true;
        }
        else
        {
            result += static_cast<char>(
                std::tolower(c)
                );

            previousWasSpace = false;
        }
    }

    while (
        !result.empty() &&
        result.back() == ' '
        )
    {
        result.pop_back();
    }

    return result;
}

// --------------------------------------------------
// MCQQuestion
// --------------------------------------------------

MCQQuestion::MCQQuestion(
    std::string topic,
    std::string prompt,
    std::vector<std::string> options,
    std::string correctAnswer,
    std::string hint,
    Difficulty difficulty
)
    : Question(
        std::move(topic),
        std::move(prompt),
        std::move(correctAnswer),
        std::move(hint),
        difficulty
    ),
    options(std::move(options))
{
}

bool MCQQuestion::isCorrect(
    const std::string& answer
) const
{
    return matchesAcceptedAnswer(
        answer
    );
}

bool MCQQuestion::isMultipleChoice() const
{
    return true;
}

std::vector<std::string>
MCQQuestion::getOptions() const
{
    return options;
}

std::string
MCQQuestion::getTypeName() const
{
    return "Multiple Choice";
}

QuestionType
MCQQuestion::getQuestionType() const
{
    return QuestionType::MCQ;
}

// --------------------------------------------------
// TextQuestion
// --------------------------------------------------

TextQuestion::TextQuestion(
    std::string topic,
    std::string prompt,
    std::string correctAnswer,
    std::string hint,
    Difficulty difficulty
)
    : Question(
        std::move(topic),
        std::move(prompt),
        std::move(correctAnswer),
        std::move(hint),
        difficulty
    )
{
}

bool TextQuestion::isCorrect(
    const std::string& answer
) const
{
    return matchesAcceptedAnswer(
        answer
    );
}

QuestionType
TextQuestion::getQuestionType() const
{
    return QuestionType::Conceptual;
}

// --------------------------------------------------
// Specialized question names
// --------------------------------------------------

std::string
OutputQuestion::getTypeName() const
{
    return "Predict Output";
}

std::string
DebugQuestion::getTypeName() const
{
    return "Debugging";
}

std::string
CodeCompletionQuestion::getTypeName() const
{
    return "Code Completion";
}

std::string
ConceptualQuestion::getTypeName() const
{
    return "Conceptual Programming";
}

std::string
DataStructureQuestion::getTypeName() const
{
    return "Data Structures";
}

std::string
AlgorithmQuestion::getTypeName() const
{
    return "Algorithm Reasoning";
}