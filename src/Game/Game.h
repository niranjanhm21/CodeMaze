#pragma once

#include <SFML/Graphics.hpp>

#include "../Learning/AdaptiveEngine.h"
#include "../Learning/HintEngine.h"
#include "../Learning/PerformanceTracker.h"
#include "../Maze/Maze.h"
#include "../Player/Player.h"
#include "../Questions/QuestionManager.h"
#include "../Scoring/ScoreManager.h"
#include "../UI/ChallengeUI.h"
#include "../UI/ResultScreen.h"

#include "LevelManager.h"

#include <set>
#include <string>
#include <utility>

class Game
{
public:

    Game();

    void run();

private:

    enum class GameState
    {
        MainMenu,
        Instructions,
        LevelSelect,
        Playing,
        Challenge,
        Won,
        GameOver,
        Result
    };

    static constexpr int STARTING_LIVES = 3;

    static constexpr int BASE_CHALLENGE_POINTS = 100;

    sf::RenderWindow window;

    sf::Clock clock;

    Maze maze;

    Player player;

    QuestionManager questionManager;

    ScoreManager scoreManager;

    HintEngine hintEngine;

    PerformanceTracker performanceTracker;

    AdaptiveEngine adaptiveEngine;

    LevelManager levelManager;

    sf::Font font;

    bool fontLoaded;

    ChallengeUI challengeUI;

    ResultScreen resultScreen;

    GameState state;

    int menuSelection;

    int selectedLevel;

    std::set<std::pair<int, int>>
        completedChallenges;

    std::pair<int, int>
        activeChallengeCell;

    const Question* activeQuestion;

    int lives;

    sf::Clock challengeClock;

    int challengeTimeLimit;

    int challengeTimeRemaining;

    int hintsUsed;

    int livesLost;

    int challengesAttempted;

    int challengesCorrect;

    void processEvents();

    void update(
        float deltaTime
    );

    void updateChallengeTimer();

    void render();

    void resetGame();

    void startCurrentLevel();

    void startNextLevel();

    void updateWindowTitle();

    void checkForChallenge();

    bool isChallengeCompleted(
        std::pair<int, int> cell
    ) const;

    void startChallenge(
        std::pair<int, int> cell
    );

    void submitChallenge();

    void handleChallengeFailure(
        const std::string& reason
    );

    void showHint();

    void finishChallenge();

    void finishLevel();

    void drawHUD();

    void drawMainMenu();

    void drawInstructions();

    void drawLevelSelect();

    void drawText(
        const std::string& text,
        sf::Vector2f position,
        unsigned int size
    );

    void handleMainMenuInput(
        const sf::Event& event
    );

    void handleInstructionsInput(
        const sf::Event& event
    );

    void handleLevelSelectInput(
        const sf::Event& event
    );
};