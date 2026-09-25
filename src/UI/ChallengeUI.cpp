#include "ChallengeUI.h"

#include <algorithm>

// --------------------------------------------------
// Constructor
// --------------------------------------------------

ChallengeUI::ChallengeUI(sf::Font& font)
    : font(font),
    question(nullptr),
    selectedOption(-1),
    timeRemaining(0),
    hintVisible(false)
{
}

// --------------------------------------------------
// Open
// --------------------------------------------------

void ChallengeUI::open(
    const Question* newQuestion,
    int timeLimit
)
{
    question = newQuestion;

    input.clear();

    selectedOption = -1;

    feedback.clear();

    hintText.clear();

    timeRemaining = timeLimit;

    hintVisible = false;
}

// --------------------------------------------------
// Close
// --------------------------------------------------

void ChallengeUI::close()
{
    question = nullptr;

    input.clear();

    selectedOption = -1;

    feedback.clear();

    hintText.clear();

    timeRemaining = 0;

    hintVisible = false;
}

// --------------------------------------------------
// Is Open
// --------------------------------------------------

bool ChallengeUI::isOpen() const
{
    return question != nullptr;
}

// --------------------------------------------------
// Handle Event
// --------------------------------------------------

bool ChallengeUI::handleEvent(
    const sf::Event& event,
    bool& submitted
)
{
    submitted = false;

    if (!question)
    {
        return false;
    }

    if (
        const auto* key =
        event.getIf<
        sf::Event::KeyPressed
        >()
        )
    {
        // --------------------------------------------------
        // Backspace
        // --------------------------------------------------

        if (
            key->code ==
            sf::Keyboard::Key::Backspace
            )
        {
            if (
                !question->isMultipleChoice() &&
                !input.empty()
                )
            {
                input.pop_back();
            }

            return true;
        }

        // --------------------------------------------------
        // ENTER
        // --------------------------------------------------

        if (
            key->code ==
            sf::Keyboard::Key::Enter
            )
        {
            if (
                question->isMultipleChoice()
                )
            {
                if (selectedOption >= 0)
                {
                    submitted = true;
                }
            }
            else
            {
                if (!input.empty())
                {
                    submitted = true;
                }
            }

            return true;
        }

        // --------------------------------------------------
        // MCQ Selection
        // --------------------------------------------------

        if (
            question->isMultipleChoice()
            )
        {
            if (
                key->code ==
                sf::Keyboard::Key::Num1 ||
                key->code ==
                sf::Keyboard::Key::Numpad1
                )
            {
                selectedOption = 0;
            }
            else if (
                key->code ==
                sf::Keyboard::Key::Num2 ||
                key->code ==
                sf::Keyboard::Key::Numpad2
                )
            {
                selectedOption = 1;
            }
            else if (
                key->code ==
                sf::Keyboard::Key::Num3 ||
                key->code ==
                sf::Keyboard::Key::Numpad3
                )
            {
                selectedOption = 2;
            }
            else if (
                key->code ==
                sf::Keyboard::Key::Num4 ||
                key->code ==
                sf::Keyboard::Key::Numpad4
                )
            {
                selectedOption = 3;
            }
        }
    }

    // --------------------------------------------------
    // Text Question Input
    // --------------------------------------------------

    if (!question->isMultipleChoice())
    {
        if (
            const auto* textEntered =
            event.getIf<
            sf::Event::TextEntered
            >()
            )
        {
            char32_t unicode =
                textEntered->unicode;

            if (
                unicode >= 32 &&
                unicode <= 126 &&
                input.size() < 80
                )
            {
                input +=
                    static_cast<char>(
                        unicode
                        );
            }
        }
    }

    return true;
}

// --------------------------------------------------
// Get Answer
// --------------------------------------------------

std::string
ChallengeUI::getAnswer() const
{
    if (!question)
    {
        return "";
    }

    if (
        question->isMultipleChoice()
        )
    {
        if (selectedOption < 0)
        {
            return "";
        }

        // IMPORTANT:
        // Return the actual option text currently
        // displayed on screen.
        //
        // The options have already been shuffled
        // by MCQQuestion.

        const auto options =
            question->getOptions();

        if (
            selectedOption >=
            static_cast<int>(
                options.size()
                )
            )
        {
            return "";
        }

        return options[
            selectedOption
        ];
    }

    return input;
}

// --------------------------------------------------
// Clear Input
// --------------------------------------------------

void ChallengeUI::clearInput()
{
    input.clear();

    selectedOption = -1;
}

// --------------------------------------------------
// Feedback
// --------------------------------------------------

void ChallengeUI::setFeedback(
    const std::string& newFeedback
)
{
    feedback = newFeedback;
}

// --------------------------------------------------
// Hint
// --------------------------------------------------

void ChallengeUI::setHint(
    const std::string& hint
)
{
    hintText = hint;

    hintVisible = true;
}

// --------------------------------------------------
// Time
// --------------------------------------------------

void ChallengeUI::setTimeRemaining(
    int seconds
)
{
    timeRemaining =
        std::max(
            0,
            seconds
        );
}

int ChallengeUI::getTimeRemaining() const
{
    return timeRemaining;
}

bool ChallengeUI::isHintVisible() const
{
    return hintVisible;
}

// --------------------------------------------------
// Text Wrapping
// --------------------------------------------------

std::vector<std::string>
ChallengeUI::wrapText(
    const std::string& text,
    std::size_t maxCharacters
) const
{
    std::vector<std::string>
        lines;

    std::string currentLine;

    std::string currentWord;

    std::string textWithSpaces =
        text;

    // Support explicit line breaks.
    for (
        char& character :
        textWithSpaces
        )
    {
        if (character == '\n')
        {
            character = ' ';
        }
    }

    for (
        char character :
    textWithSpaces
        )
    {
        if (character == ' ')
        {
            if (!currentWord.empty())
            {
                std::string testLine =
                    currentLine.empty()
                    ? currentWord
                    : currentLine +
                    " " +
                    currentWord;

                if (
                    testLine.size() >
                    maxCharacters
                    )
                {
                    if (!currentLine.empty())
                    {
                        lines.push_back(
                            currentLine
                        );
                    }

                    currentLine =
                        currentWord;
                }
                else
                {
                    currentLine =
                        testLine;
                }

                currentWord.clear();
            }
        }
        else
        {
            currentWord += character;
        }
    }

    if (!currentWord.empty())
    {
        std::string testLine =
            currentLine.empty()
            ? currentWord
            : currentLine +
            " " +
            currentWord;

        if (
            testLine.size() >
            maxCharacters &&
            !currentLine.empty()
            )
        {
            lines.push_back(
                currentLine
            );

            currentLine =
                currentWord;
        }
        else
        {
            currentLine =
                testLine;
        }
    }

    if (!currentLine.empty())
    {
        lines.push_back(
            currentLine
        );
    }

    // Prevent an empty question area.
    if (lines.empty())
    {
        lines.push_back("");
    }

    return lines;
}

// --------------------------------------------------
// Draw Text
// --------------------------------------------------

void ChallengeUI::drawText(
    sf::RenderWindow& window,
    const std::string& text,
    sf::Vector2f position,
    unsigned int size
) const
{
    sf::Text drawableText(
        font,
        text,
        size
    );

    drawableText.setFillColor(
        sf::Color::White
    );

    drawableText.setPosition(
        position
    );

    window.draw(
        drawableText
    );
}

// --------------------------------------------------
// Draw Panel
// --------------------------------------------------

void ChallengeUI::drawPanel(
    sf::RenderWindow& window,
    sf::Vector2f position,
    sf::Vector2f size,
    sf::Color fillColor,
    sf::Color outlineColor,
    float outlineThickness
) const
{
    sf::RectangleShape panel;

    panel.setSize(
        size
    );

    panel.setPosition(
        position
    );

    panel.setFillColor(
        fillColor
    );

    panel.setOutlineColor(
        outlineColor
    );

    panel.setOutlineThickness(
        outlineThickness
    );

    window.draw(
        panel
    );
}

// --------------------------------------------------
// Draw Challenge UI
// --------------------------------------------------

void ChallengeUI::draw(
    sf::RenderWindow& window
) const
{
    if (!question)
    {
        return;
    }

    // --------------------------------------------------
    // FULL SCREEN OVERLAY
    // --------------------------------------------------

    sf::RectangleShape overlay;

    overlay.setSize(
        {
            650.0f,
            650.0f
        }
    );

    overlay.setFillColor(
        sf::Color(
            8,
            8,
            12,
            245
        )
    );

    window.draw(
        overlay
    );

    // --------------------------------------------------
    // MAIN CHALLENGE PANEL
    // --------------------------------------------------

    drawPanel(
        window,
        {
            30.0f,
            20.0f
        },
        {
            590.0f,
            610.0f
        },
        sf::Color(
            28,
            28,
            38
        ),
        sf::Color(
            120,
            90,
            200
        ),
        3.0f
    );

    // --------------------------------------------------
    // HEADER
    // --------------------------------------------------

    drawText(
        window,
        "C++ CHALLENGE",
        {
            55.0f,
            38.0f
        },
        28
    );

    drawText(
        window,
        "Topic: " +
        question->getTopic(),
        {
            55.0f,
            78.0f
        },
        17
    );

    drawText(
        window,
        "Type: " +
        question->getTypeName(),
        {
            55.0f,
            103.0f
        },
        17
    );

    // --------------------------------------------------
    // TIMER PANEL
    // --------------------------------------------------

    sf::Color timerOutline =
        timeRemaining <= 5
        ? sf::Color(
            220,
            70,
            70
        )
        : timeRemaining <= 10
        ? sf::Color(
            220,
            170,
            50
        )
        : sf::Color(
            80,
            180,
            100
        );

    drawPanel(
        window,
        {
            455.0f,
            40.0f
        },
        {
            135.0f,
            55.0f
        },
        sf::Color(
            18,
            18,
            25
        ),
        timerOutline,
        2.0f
    );

    drawText(
        window,
        "TIME",
        {
            470.0f,
            47.0f
        },
        13
    );

    drawText(
        window,
        std::to_string(
            timeRemaining
        ) + "s",
        {
            515.0f,
            45.0f
        },
        24
    );

    // --------------------------------------------------
    // QUESTION PANEL
    // --------------------------------------------------

    drawPanel(
        window,
        {
            55.0f,
            140.0f
        },
        {
            540.0f,
            125.0f
        },
        sf::Color(
            20,
            20,
            28
        ),
        sf::Color(
            70,
            70,
            90
        ),
        2.0f
    );

    drawText(
        window,
        "QUESTION",
        {
            72.0f,
            153.0f
        },
        13
    );

    auto questionLines =
        wrapText(
            question->getPrompt(),
            60
        );

    float questionY =
        180.0f;

    for (
        const auto& line :
        questionLines
        )
    {
        drawText(
            window,
            line,
            {
                72.0f,
                questionY
            },
            17
        );

        questionY +=
            24.0f;
    }

    // --------------------------------------------------
    // MCQ OPTIONS
    // --------------------------------------------------

    if (
        question->isMultipleChoice()
        )
    {
        auto options =
            question->getOptions();

        std::size_t optionCount =
            std::min<std::size_t>(
                4,
                options.size()
            );

        float optionStartY =
            280.0f;

        for (
            std::size_t i = 0;
            i < optionCount;
            ++i
            )
        {
            float optionY =
                optionStartY +
                static_cast<float>(i) *
                54.0f;

            bool selected =
                static_cast<int>(i) ==
                selectedOption;

            sf::Color fillColor =
                selected
                ? sf::Color(
                    65,
                    50,
                    105
                )
                : sf::Color(
                    20,
                    20,
                    28
                );

            sf::Color outlineColor =
                selected
                ? sf::Color(
                    160,
                    110,
                    230
                )
                : sf::Color(
                    75,
                    75,
                    95
                );

            drawPanel(
                window,
                {
                    55.0f,
                    optionY
                },
                {
                    540.0f,
                    45.0f
                },
                fillColor,
                outlineColor,
                selected
                ? 3.0f
                : 1.5f
            );

            char letter =
                static_cast<char>(
                    'A' + i
                    );

            drawText(
                window,
                std::string(
                    1,
                    letter
                ),
                {
                    70.0f,
                    optionY + 10.0f
                },
                18
            );

            auto optionLines =
                wrapText(
                    options[i],
                    48
                );

            std::string displayOption =
                optionLines.empty()
                ? ""
                : optionLines[0];

            drawText(
                window,
                displayOption,
                {
                    105.0f,
                    optionY + 10.0f
                },
                16
            );

            if (selected)
            {
                drawText(
                    window,
                    "SELECTED",
                    {
                        500.0f,
                        optionY + 12.0f
                    },
                    11
                );
            }
        }

        // --------------------------------------------------
        // MCQ Instructions
        // --------------------------------------------------

        drawText(
            window,
            "1-4: Select answer",
            {
                55.0f,
                505.0f
            },
            14
        );

        drawText(
            window,
            "ENTER: Submit",
            {
                220.0f,
                505.0f
            },
            14
        );

        if (selectedOption < 0)
        {
            drawText(
                window,
                "Choose an option before submitting.",
                {
                    55.0f,
                    530.0f
                },
                14
            );
        }
    }
    else
    {
        // --------------------------------------------------
        // TEXT QUESTION INPUT
        // --------------------------------------------------

        drawPanel(
            window,
            {
                55.0f,
                300.0f
            },
            {
                540.0f,
                60.0f
            },
            sf::Color(
                18,
                18,
                25
            ),
            sf::Color(
                90,
                90,
                110
            ),
            2.0f
        );

        drawText(
            window,
            input.empty()
            ? "Type your answer..."
            : input,
            {
                72.0f,
                318.0f
            },
            17
        );

        drawText(
            window,
            "Type answer and press ENTER",
            {
                55.0f,
                375.0f
            },
            14
        );
    }

    // --------------------------------------------------
    // FEEDBACK PANEL
    // --------------------------------------------------

    if (!feedback.empty())
    {
        drawPanel(
            window,
            {
                55.0f,
                535.0f
            },
            {
                540.0f,
                35.0f
            },
            sf::Color(
                45,
                35,
                25
            ),
            sf::Color(
                190,
                140,
                60
            ),
            1.5f
        );

        auto feedbackLines =
            wrapText(
                feedback,
                65
            );

        drawText(
            window,
            feedbackLines[0],
            {
                68.0f,
                544.0f
            },
            13
        );
    }

    // --------------------------------------------------
    // HINT PANEL
    // --------------------------------------------------

    if (hintVisible)
    {
        float hintY =
            feedback.empty()
            ? 535.0f
            : 575.0f;

        float panelHeight =
            50.0f;

        if (
            hintY + panelHeight >
            625.0f
            )
        {
            hintY =
                575.0f;
        }

        drawPanel(
            window,
            {
                55.0f,
                hintY
            },
            {
                540.0f,
                panelHeight
            },
            sf::Color(
                30,
                40,
                50
            ),
            sf::Color(
                70,
                150,
                190
            ),
            1.5f
        );

        auto hintLines =
            wrapText(
                hintText,
                65
            );

        drawText(
            window,
            "HINT",
            {
                68.0f,
                hintY + 7.0f
            },
            12
        );

        if (!hintLines.empty())
        {
            drawText(
                window,
                hintLines[0],
                {
                    120.0f,
                    hintY + 7.0f
                },
                12
            );
        }
    }
}