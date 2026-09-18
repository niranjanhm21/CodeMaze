#pragma once

#include "../Questions/Question.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

class ChallengeUI
{
public:
    explicit ChallengeUI(sf::Font& font);

    void open(
        const Question* question,
        int timeLimit
    );

    void close();

    bool isOpen() const;

    bool handleEvent(
        const sf::Event& event,
        bool& submitted
    );

    std::string getAnswer() const;

    void clearInput();

    void setFeedback(
        const std::string& feedback
    );

    void setHint(
        const std::string& hint
    );

    void setTimeRemaining(
        int seconds
    );

    int getTimeRemaining() const;

    bool isHintVisible() const;

    void draw(
        sf::RenderWindow& window
    ) const;

private:
    sf::Font& font;

    const Question* question;

    std::string input;

    int selectedOption;

    std::string feedback;

    std::string hintText;

    int timeRemaining;

    bool hintVisible;

    std::vector<std::string> wrapText(
        const std::string& text,
        std::size_t maxCharacters
    ) const;

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
};