#pragma once

#include "../Questions/Question.h"

#include <string>

class HintEngine
{
public:
    HintEngine();

    std::string generateHint(
        const Question& question
    ) const;
};