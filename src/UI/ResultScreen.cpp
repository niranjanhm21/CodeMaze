#include "ResultScreen.h"

#include <algorithm>

ResultScreen::ResultScreen(
    sf::Font& font
)
    : font(font),
    visible(false),
    level(1),
    score(0),
    livesLost(0),
    hintsUsed(0),
    accuracy(0.0),
    totalAttempts(0),
    totalCorrect(0)
{
}

void ResultScreen::show(
    int newLevel,
    int newScore,
    int newLivesLost,
    int newHintsUsed,
    const PerformanceTracker& tracker
)
{
    level =
        newLevel;

    score =
        newScore;

    livesLost =
        newLivesLost;

    hintsUsed =
        newHintsUsed;

    accuracy =
        tracker.getOverallAccuracy();

    totalAttempts =
        tracker.getTotalAttempts();

    totalCorrect =
        tracker.getTotalCorrect();

    topicAccuracy.clear();

    topicFeedback.clear();

    for (
        const auto& [topic, statistics] :
        tracker.getTopicStatistics()
        )
    {
        double topicValue =
            statistics.getAccuracy();

        topicAccuracy.push_back(
            {
                topic,
                topicValue
            }
        );

        topicFeedback.push_back(
            {
                topic,
                getLearningFeedback(
                    topicValue
                )
            }
        );
    }

    visible = true;
}

bool ResultScreen::isVisible() const
{
    return visible;
}

void ResultScreen::hide()
{
    visible = false;
}

void ResultScreen::handleEvent(
    const sf::Event& event,
    bool& restart,
    bool& nextLevel
)
{
    restart = false;

    nextLevel = false;

    if (
        const auto* key =
        event.getIf<
        sf::Event::KeyPressed>()
        )
    {
        if (
            key->code ==
            sf::Keyboard::Key::R
            )
        {
            restart = true;

            return;
        }

        if (
            key->code ==
            sf::Keyboard::Key::N
            )
        {
            nextLevel = true;

            return;
        }
    }
}

void ResultScreen::drawText(
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

void ResultScreen::drawPanel(
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

std::string ResultScreen::getLearningFeedback(
    double value
) const
{
    if (value < 50.0)
    {
        return "Needs Practice";
    }

    if (value < 75.0)
    {
        return "Improving";
    }

    if (value < 90.0)
    {
        return "Strong";
    }

    return "Mastered";
}

void ResultScreen::draw(
    sf::RenderWindow& window
) const
{
    if (!visible)
    {
        return;
    }

    /*
     * ------------------------------------------------
     * BACKGROUND
     * ------------------------------------------------
     */

    sf::RectangleShape background;

    background.setSize(
        {
            650.0f,
            650.0f
        }
    );

    background.setFillColor(
        sf::Color(
            10,
            10,
            16
        )
    );

    window.draw(
        background
    );

    /*
     * ------------------------------------------------
     * TITLE
     * ------------------------------------------------
     */

    drawText(
        window,
        "LEVEL COMPLETE",
        {
            175.0f,
            25.0f
        },
        30
    );

    drawText(
        window,
        "Your learning progress",
        {
            205.0f,
            62.0f
        },
        14
    );

    /*
     * ------------------------------------------------
     * SUMMARY PANEL
     * ------------------------------------------------
     */

    drawPanel(
        window,
        {
            45.0f,
            95.0f
        },
        {
            560.0f,
            185.0f
        },
        sf::Color(
            27,
            27,
            38
        ),
        sf::Color(
            100,
            80,
            150
        ),
        2.0f
    );

    drawText(
        window,
        "LEVEL",
        {
            70.0f,
            112.0f
        },
        12
    );

    drawText(
        window,
        std::to_string(level),
        {
            70.0f,
            133.0f
        },
        23
    );

    drawText(
        window,
        "SCORE",
        {
            190.0f,
            112.0f
        },
        12
    );

    drawText(
        window,
        std::to_string(score),
        {
            190.0f,
            133.0f
        },
        23
    );

    drawText(
        window,
        "ACCURACY",
        {
            340.0f,
            112.0f
        },
        12
    );

    drawText(
        window,
        std::to_string(
            static_cast<int>(
                accuracy
                )
        ) + "%",
        {
            340.0f,
            133.0f
        },
        23
    );

    drawText(
        window,
        "CORRECT",
        {
            480.0f,
            112.0f
        },
        12
    );

    drawText(
        window,
        std::to_string(
            totalCorrect
        ) +
        "/" +
        std::to_string(
            totalAttempts
        ),
        {
            480.0f,
            133.0f
        },
        23
    );

    drawText(
        window,
        "Lives lost: " +
        std::to_string(
            livesLost
        ),
        {
            70.0f,
            190.0f
        },
        17
    );

    drawText(
        window,
        "Hints used: " +
        std::to_string(
            hintsUsed
        ),
        {
            70.0f,
            225.0f
        },
        17
    );

    drawText(
        window,
        "Overall learning status: " +
        getLearningFeedback(
            accuracy
        ),
        {
            300.0f,
            190.0f
        },
        15
    );

    /*
     * ------------------------------------------------
     * TOPIC PERFORMANCE
     * ------------------------------------------------
     */

    drawText(
        window,
        "TOPIC PERFORMANCE",
        {
            55.0f,
            300.0f
        },
        22
    );

    drawText(
        window,
        "Topic",
        {
            70.0f,
            335.0f
        },
        13
    );

    drawText(
        window,
        "Accuracy",
        {
            330.0f,
            335.0f
        },
        13
    );

    drawText(
        window,
        "Status",
        {
            460.0f,
            335.0f
        },
        13
    );

    /*
     * Show up to seven topics so that the
     * result screen remains readable.
     */

    std::size_t displayCount =
        std::min<std::size_t>(
            7,
            topicAccuracy.size()
        );

    float topicY =
        360.0f;

    for (
        std::size_t i = 0;
        i < displayCount;
        ++i
        )
    {
        const auto& topic =
            topicAccuracy[i];

        const auto& feedbackValue =
            topicFeedback[i];

        drawPanel(
            window,
            {
                55.0f,
                topicY - 4.0f
            },
            {
                540.0f,
                30.0f
            },
            sf::Color(
                19,
                19,
                27
            ),
            sf::Color(
                55,
                55,
                70
            ),
            1.0f
        );

        drawText(
            window,
            topic.first,
            {
                70.0f,
                topicY
            },
            14
        );

        drawText(
            window,
            std::to_string(
                static_cast<int>(
                    topic.second
                    )
            ) + "%",
            {
                345.0f,
                topicY
            },
            14
        );

        drawText(
            window,
            feedbackValue.second,
            {
                460.0f,
                topicY
            },
            13
        );

        topicY += 34.0f;
    }

    /*
     * ------------------------------------------------
     * LEARNING INSIGHT
     * ------------------------------------------------
     */

    float insightY =
        topicY + 5.0f;

    drawPanel(
        window,
        {
            55.0f,
            insightY
        },
        {
            540.0f,
            70.0f
        },
        sf::Color(
            25,
            35,
            45
        ),
        sf::Color(
            70,
            140,
            175
        ),
        2.0f
    );

    drawText(
        window,
        "LEARNING INSIGHT",
        {
            70.0f,
            insightY + 8.0f
        },
        13
    );

    /*
     * Find the weakest displayed topic.
     */

    if (!topicAccuracy.empty())
    {
        const auto* weakest =
            &topicAccuracy.front();

        for (
            const auto& item :
            topicAccuracy
            )
        {
            if (
                item.second <
                weakest->second
                )
            {
                weakest =
                    &item;
            }
        }

        drawText(
            window,
            "Focus more on: " +
            weakest->first,
            {
                70.0f,
                insightY + 31.0f
            },
            15
        );

        drawText(
            window,
            "Accuracy: " +
            std::to_string(
                static_cast<int>(
                    weakest->second
                    )
            ) +
            "% | Adaptive practice will prioritize this area.",
            {
                70.0f,
                insightY + 50.0f
            },
            12
        );
    }
    else
    {
        drawText(
            window,
            "Complete more challenges to generate learning insights.",
            {
                70.0f,
                insightY + 34.0f
            },
            13
        );
    }

    /*
     * ------------------------------------------------
     * CONTROLS
     * ------------------------------------------------
     */

    drawText(
        window,
        "R = Replay Level",
        {
            70.0f,
            610.0f
        },
        17
    );

    if (level < 4)
    {
        drawText(
            window,
            "N = Next Level",
            {
                395.0f,
                610.0f
            },
            17
        );
    }
    else
    {
        drawText(
            window,
            "Final Level Complete!",
            {
                355.0f,
                610.0f
            },
            14
        );
    }
}