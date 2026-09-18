#pragma once

#include "../Learning/PerformanceTracker.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <utility>
#include <vector>

class ResultScreen
{
public:
    explicit ResultScreen(
        sf::Font& font
    );

    void show(
        int level,
        int score,
        int livesLost,
        int hintsUsed,
        const PerformanceTracker& tracker
    );

    bool isVisible() const;

    void hide();

    void handleEvent(
        const sf::Event& event,
        bool& restart,
        bool& nextLevel
    );

    void draw(
        sf::RenderWindow& window
    ) const;

private:
    sf::Font& font;

    bool visible;

    int level;

    int score;

    int livesLost;

    int hintsUsed;

    double accuracy;

    int totalAttempts;

    int totalCorrect;

    std::vector<
        std::pair<
        std::string,
        double
        >
    > topicAccuracy;

    std::vector<
        std::pair<
        std::string,
        std::string
        >
    > topicFeedback;

    void drawText(
        sf::RenderWindow& window,
        const std::string& text,
        sf::Vector2f position,
        unsigned int size
    ) const;

    void drawPanel(
        sf::RenderWindow& window,
        sf::Vector2f position,
        sf::Vector2f size,
        sf::Color fillColor,
        sf::Color outlineColor,
        float outlineThickness
    ) const;

    std::string getLearningFeedback(
        double accuracy
    ) const;
};