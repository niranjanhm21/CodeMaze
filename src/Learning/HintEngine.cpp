#include "HintEngine.h"

HintEngine::HintEngine()
{
}

std::string HintEngine::generateHint(
    const Question& question
) const
{
    std::string hint =
        question.getHint();

    if (!hint.empty())
    {
        return "Hint: " + hint;
    }

    const std::string& topic =
        question.getTopic();

    if (topic == "Pointers")
    {
        return
            "Hint: Think about where the pointer "
            "is pointing before using * on it.";
    }

    if (topic == "OOP")
    {
        return
            "Hint: Think about the principle of "
            "organizing data and behaviour together.";
    }

    if (topic == "Arrays")
    {
        return
            "Hint: Remember that C++ arrays use "
            "zero-based indexing.";
    }

    if (topic == "Loops")
    {
        return
            "Hint: Trace the loop from its initial "
            "value until its condition becomes false.";
    }

    if (topic == "STL")
    {
        return
            "Hint: Identify which STL container "
            "matches the required access behaviour.";
    }

    if (topic == "Graphs")
    {
        return
            "Hint: Think about whether the algorithm "
            "explores nodes level by level.";
    }

    if (topic == "Sorting")
    {
        return
            "Hint: Look for an algorithm that chooses "
            "a pivot and partitions the data.";
    }

    return
        "Hint: Break the problem into smaller "
        "steps and trace what the C++ code does.";
}