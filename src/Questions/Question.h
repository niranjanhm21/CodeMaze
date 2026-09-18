#pragma once

#include <string>
#include <vector>

enum class QuestionType
{
    MCQ,
    Output,
    Debugging,
    CodeCompletion,
    Conceptual,
    DataStructure,
    Algorithm
};

enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

class Question
{
public:
    Question(
        std::string topic,
        std::string prompt,
        std::string correctAnswer,
        std::string hint,
        Difficulty difficulty
    );

    virtual ~Question() = default;

    virtual bool isCorrect(
        const std::string& answer
    ) const = 0;

    virtual bool isMultipleChoice() const;

    virtual std::vector<std::string>
        getOptions() const;

    virtual std::string
        getTypeName() const = 0;

    virtual QuestionType
        getQuestionType() const = 0;

    const std::string& getTopic() const;

    const std::string& getPrompt() const;

    const std::string& getHint() const;

    Difficulty getDifficulty() const;

    static std::string
        difficultyToString(
            Difficulty difficulty
        );

protected:
    std::string topic;
    std::string prompt;

    // Multiple accepted answers are stored
    // using ';' as the separator.
    std::string correctAnswer;

    std::string hint;

    Difficulty difficulty;

    bool matchesAcceptedAnswer(
        const std::string& answer
    ) const;

    static std::string normalize(
        const std::string& value
    );

    static std::vector<std::string>
        splitAcceptedAnswers(
            const std::string& answers
        );
};

// --------------------------------------------------
// MCQ
// --------------------------------------------------

class MCQQuestion : public Question
{
public:
    MCQQuestion(
        std::string topic,
        std::string prompt,
        std::vector<std::string> options,
        std::string correctAnswer,
        std::string hint,
        Difficulty difficulty
    );

    bool isCorrect(
        const std::string& answer
    ) const override;

    bool isMultipleChoice() const override;

    std::vector<std::string>
        getOptions() const override;

    std::string
        getTypeName() const override;

    QuestionType
        getQuestionType() const override;

private:
    std::vector<std::string> options;
};

// --------------------------------------------------
// Text question
// --------------------------------------------------

class TextQuestion : public Question
{
public:
    TextQuestion(
        std::string topic,
        std::string prompt,
        std::string correctAnswer,
        std::string hint,
        Difficulty difficulty
    );

    bool isCorrect(
        const std::string& answer
    ) const override;

    QuestionType
        getQuestionType() const override;
};

// --------------------------------------------------
// Specialized question types
// --------------------------------------------------

class OutputQuestion : public TextQuestion
{
public:
    using TextQuestion::TextQuestion;

    std::string
        getTypeName() const override;
};

class DebugQuestion : public TextQuestion
{
public:
    using TextQuestion::TextQuestion;

    std::string
        getTypeName() const override;
};

class CodeCompletionQuestion : public TextQuestion
{
public:
    using TextQuestion::TextQuestion;

    std::string
        getTypeName() const override;
};

class ConceptualQuestion : public TextQuestion
{
public:
    using TextQuestion::TextQuestion;

    std::string
        getTypeName() const override;
};

class DataStructureQuestion : public TextQuestion
{
public:
    using TextQuestion::TextQuestion;

    std::string
        getTypeName() const override;
};

class AlgorithmQuestion : public TextQuestion
{
public:
    using TextQuestion::TextQuestion;

    std::string
        getTypeName() const override;
};