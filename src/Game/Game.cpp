#include "Game.h"

Game::Game()
    : window(
        sf::VideoMode({ 650, 650 }),
        "CodeMaze - C++ Learning Maze"
    ),
    fontLoaded(false),
    challengeUI(font),
    resultScreen(font),
    state(GameState::MainMenu),
    menuSelection(0),
    selectedLevel(1),
    activeChallengeCell({ -1, -1 }),
    activeQuestion(nullptr),
    lives(STARTING_LIVES),
    challengeTimeLimit(30),
    challengeTimeRemaining(30),
    hintsUsed(0),
    livesLost(0),
    challengesAttempted(0),
    challengesCorrect(0)
{
    window.setFramerateLimit(60);

    fontLoaded =
        font.openFromFile(
            "assets/arial.ttf"
        );

    bool questionsLoaded =
        questionManager.loadFromFile(
            "data/questions.txt"
        );

    if (!fontLoaded)
    {
        window.setTitle(
            "CodeMaze - ERROR: assets/arial.ttf missing"
        );
    }
    else if (!questionsLoaded)
    {
        window.setTitle(
            "CodeMaze - ERROR: questions.txt missing"
        );
    }
    else
    {
        updateWindowTitle();
    }
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();

        float deltaTime =
            clock.restart().asSeconds();

        if (deltaTime > 0.1f)
        {
            deltaTime = 0.1f;
        }

        update(deltaTime);

        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event =
        window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();

            continue;
        }

        if (state == GameState::MainMenu)
        {
            handleMainMenuInput(
                *event
            );

            continue;
        }

        if (state == GameState::Instructions)
        {
            handleInstructionsInput(
                *event
            );

            continue;
        }

        if (state == GameState::LevelSelect)
        {
            handleLevelSelectInput(
                *event
            );

            continue;
        }

        if (state == GameState::Challenge)
        {
            if (const auto* keyPressed =
                event->getIf<
                sf::Event::KeyPressed>())
            {
                if (
                    keyPressed->code ==
                    sf::Keyboard::Key::Escape
                    )
                {
                    window.close();

                    continue;
                }
            }

            bool submitted = false;

            challengeUI.handleEvent(
                *event,
                submitted
            );

            if (submitted)
            {
                submitChallenge();
            }

            continue;
        }

        if (state == GameState::Result)
        {
            if (const auto* keyPressed =
                event->getIf<
                sf::Event::KeyPressed>())
            {
                if (
                    keyPressed->code ==
                    sf::Keyboard::Key::Escape
                    )
                {
                    resultScreen.hide();

                    state =
                        GameState::MainMenu;

                    updateWindowTitle();

                    continue;
                }
            }

            bool restart = false;

            bool nextLevel = false;

            resultScreen.handleEvent(
                *event,
                restart,
                nextLevel
            );

            if (restart)
            {
                resetGame();
            }
            else if (
                nextLevel &&
                levelManager.getCurrentLevel() <
                levelManager.getTotalLevels()
                )
            {
                startNextLevel();
            }

            continue;
        }

        if (state == GameState::Playing ||
            state == GameState::GameOver)
        {
            if (const auto* keyPressed =
                event->getIf<
                sf::Event::KeyPressed>())
            {
                if (
                    keyPressed->code ==
                    sf::Keyboard::Key::Escape
                    )
                {
                    state =
                        GameState::MainMenu;

                    updateWindowTitle();

                    continue;
                }

                if (
                    keyPressed->code ==
                    sf::Keyboard::Key::R
                    )
                {
                    resetGame();
                }
            }
        }
    }
}

void Game::handleMainMenuInput(
    const sf::Event& event
)
{
    const auto* key =
        event.getIf<
        sf::Event::KeyPressed>();

    if (!key)
    {
        return;
    }

    if (
        key->code ==
        sf::Keyboard::Key::Up
        )
    {
        --menuSelection;

        if (menuSelection < 0)
        {
            menuSelection = 3;
        }

        return;
    }

    if (
        key->code ==
        sf::Keyboard::Key::Down
        )
    {
        ++menuSelection;

        if (menuSelection > 3)
        {
            menuSelection = 0;
        }

        return;
    }

    if (
        key->code ==
        sf::Keyboard::Key::Num1
        )
    {
        menuSelection = 0;
    }
    else if (
        key->code ==
        sf::Keyboard::Key::Num2
        )
    {
        menuSelection = 1;
    }
    else if (
        key->code ==
        sf::Keyboard::Key::Num3
        )
    {
        menuSelection = 2;
    }
    else if (
        key->code ==
        sf::Keyboard::Key::Num4
        )
    {
        menuSelection = 3;
    }

    if (
        key->code !=
        sf::Keyboard::Key::Enter &&
        key->code !=
        sf::Keyboard::Key::Num1 &&
        key->code !=
        sf::Keyboard::Key::Num2 &&
        key->code !=
        sf::Keyboard::Key::Num3 &&
        key->code !=
        sf::Keyboard::Key::Num4
        )
    {
        return;
    }

    switch (menuSelection)
    {
    case 0:

        levelManager.setLevel(
            selectedLevel
        );

        resetGame();

        break;

    case 1:

        state =
            GameState::Instructions;

        updateWindowTitle();

        break;

    case 2:

        state =
            GameState::LevelSelect;

        updateWindowTitle();

        break;

    case 3:

        window.close();

        break;
    }
}

void Game::handleInstructionsInput(
    const sf::Event& event
)
{
    const auto* key =
        event.getIf<
        sf::Event::KeyPressed>();

    if (!key)
    {
        return;
    }

    if (
        key->code ==
        sf::Keyboard::Key::Escape ||
        key->code ==
        sf::Keyboard::Key::B
        )
    {
        state =
            GameState::MainMenu;

        updateWindowTitle();

        return;
    }

    if (
        key->code ==
        sf::Keyboard::Key::Enter
        )
    {
        levelManager.setLevel(
            selectedLevel
        );

        resetGame();
    }
}

void Game::handleLevelSelectInput(
    const sf::Event& event
)
{
    const auto* key =
        event.getIf<
        sf::Event::KeyPressed>();

    if (!key)
    {
        return;
    }

    if (
        key->code ==
        sf::Keyboard::Key::Escape ||
        key->code ==
        sf::Keyboard::Key::B
        )
    {
        state =
            GameState::MainMenu;

        updateWindowTitle();

        return;
    }

    if (
        key->code ==
        sf::Keyboard::Key::Num1
        )
    {
        selectedLevel = 1;
    }
    else if (
        key->code ==
        sf::Keyboard::Key::Num2
        )
    {
        selectedLevel = 2;
    }
    else if (
        key->code ==
        sf::Keyboard::Key::Num3
        )
    {
        selectedLevel = 3;
    }
    else if (
        key->code ==
        sf::Keyboard::Key::Num4
        )
    {
        selectedLevel = 4;
    }
    else if (
        key->code ==
        sf::Keyboard::Key::Enter
        )
    {
        levelManager.setLevel(
            selectedLevel
        );

        resetGame();

        return;
    }
    else
    {
        return;
    }

    levelManager.setLevel(
        selectedLevel
    );

    updateWindowTitle();
}

void Game::update(
    float deltaTime
)
{
    if (state == GameState::Challenge)
    {
        updateChallengeTimer();

        return;
    }

    if (state != GameState::Playing)
    {
        return;
    }

    sf::Vector2f oldPosition =
        player.getPosition();

    player.handleInput(
        deltaTime
    );

    if (
        maze.isWall(
            player.getBounds()
        )
        )
    {
        player.setPosition(
            oldPosition
        );
    }

    if (
        player.getBounds()
        .findIntersection(
            maze.getExitBounds()
        )
        )
    {
        if (
            completedChallenges.size() ==
            maze.getChallengeCells().size()
            )
        {
            finishLevel();

            return;
        }
        else
        {
            updateWindowTitle();
        }
    }

    checkForChallenge();
}

void Game::updateChallengeTimer()
{
    if (!activeQuestion)
    {
        return;
    }

    float elapsed =
        challengeClock
        .getElapsedTime()
        .asSeconds();

    int remaining =
        challengeTimeLimit -
        static_cast<int>(elapsed);

    if (remaining < 0)
    {
        remaining = 0;
    }

    challengeTimeRemaining =
        remaining;

    challengeUI.setTimeRemaining(
        challengeTimeRemaining
    );

    if (challengeTimeRemaining <= 0)
    {
        handleChallengeFailure(
            "Time is up!"
        );
    }
}

void Game::render()
{
    window.clear(
        sf::Color(
            10,
            12,
            20
        )
    );

    if (state == GameState::MainMenu)
    {
        drawMainMenu();
    }
    else if (state == GameState::Instructions)
    {
        drawInstructions();
    }
    else if (state == GameState::LevelSelect)
    {
        drawLevelSelect();
    }
    else if (state == GameState::Result)
    {
        resultScreen.draw(
            window
        );
    }
    else
    {
        maze.draw(
            window
        );

        player.draw(
            window
        );

        drawHUD();

        if (
            state ==
            GameState::Challenge
            )
        {
            challengeUI.draw(
                window
            );
        }

        if (
            state ==
            GameState::GameOver
            )
        {
            sf::RectangleShape overlay;

            overlay.setSize(
                {
                    650.0f,
                    600.0f
                }
            );

            overlay.setFillColor(
                sf::Color(
                    0,
                    0,
                    0,
                    190
                )
            );

            window.draw(
                overlay
            );

            drawText(
                "GAME OVER",
                {
                    225.0f,
                    180.0f
                },
                42
            );

            drawText(
                "You lost all 3 lives.",
                {
                    185.0f,
                    245.0f
                },
                22
            );

            drawText(
                "Press R to retry this level.",
                {
                    170.0f,
                    300.0f
                },
                20
            );

            drawText(
                "Press ESC for Main Menu.",
                {
                    175.0f,
                    340.0f
                },
                20
            );
        }
    }

    window.display();
}

void Game::resetGame()
{
    scoreManager.reset();

    performanceTracker.reset();

    adaptiveEngine.reset();

    lives =
        STARTING_LIVES;

    hintsUsed = 0;

    livesLost = 0;

    challengesAttempted = 0;

    challengesCorrect = 0;

    resultScreen.hide();

    state =
        GameState::Playing;

    startCurrentLevel();
}

void Game::startCurrentLevel()
{
    const LevelSettings&
        settings =
        levelManager
        .getCurrentSettings();

    maze.configure(
        settings.mazeRows,
        settings.mazeColumns,
        settings.challengeCount
    );

    maze.regenerate();

    float playerSize =
        maze.getRows() <= 10
        ? 40.0f
        : maze.getRows() <= 15
        ? 28.0f
        : maze.getRows() <= 20
        ? 22.0f
        : 18.0f;

    float playerSpeed =
        maze.getRows() <= 10
        ? 220.0f
        : maze.getRows() <= 15
        ? 180.0f
        : maze.getRows() <= 20
        ? 160.0f
        : 140.0f;

    player.configure(
        playerSize,
        playerSpeed
    );

    player.setPosition(
        maze.getStartPosition()
    );

    completedChallenges.clear();

    activeChallengeCell =
    {
        -1,
        -1
    };

    activeQuestion =
        nullptr;

    challengeUI.close();

    challengeTimeLimit =
        settings.challengeTime;

    challengeTimeRemaining =
        challengeTimeLimit;

    state =
        GameState::Playing;

    updateWindowTitle();
}

void Game::startNextLevel()
{
    int nextLevel =
        levelManager.getCurrentLevel() +
        1;

    if (
        nextLevel >
        levelManager.getTotalLevels()
        )
    {
        return;
    }

    levelManager.setLevel(
        nextLevel
    );

    selectedLevel =
        nextLevel;

    lives =
        STARTING_LIVES;

    resultScreen.hide();

    state =
        GameState::Playing;

    startCurrentLevel();
}

void Game::updateWindowTitle()
{
    if (state == GameState::MainMenu)
    {
        window.setTitle(
            "CodeMaze - C++ Learning Maze | Main Menu"
        );

        return;
    }

    if (state == GameState::Instructions)
    {
        window.setTitle(
            "CodeMaze - Instructions"
        );

        return;
    }

    if (state == GameState::LevelSelect)
    {
        window.setTitle(
            "CodeMaze - Level Selection"
        );

        return;
    }

    const LevelSettings&
        settings =
        levelManager
        .getCurrentSettings();

    if (
        state ==
        GameState::Playing
        )
    {
        window.setTitle(
            "CodeMaze - Level " +
            std::to_string(
                settings.levelNumber
            ) +
            " " +
            settings.name +
            " | Lives: " +
            std::to_string(
                lives
            ) +
            " | Score: " +
            std::to_string(
                scoreManager.getScore()
            ) +
            " | Challenges: " +
            std::to_string(
                completedChallenges.size()
            ) +
            "/" +
            std::to_string(
                maze.getChallengeCells()
                .size()
            )
        );
    }
    else if (
        state ==
        GameState::Challenge
        )
    {
        window.setTitle(
            "CodeMaze - Level " +
            std::to_string(
                settings.levelNumber
            ) +
            " | C++ MCQ | Lives: " +
            std::to_string(
                lives
            ) +
            " | Time: " +
            std::to_string(
                challengeTimeRemaining
            )
        );
    }
    else if (
        state ==
        GameState::Won
        )
    {
        window.setTitle(
            "CodeMaze - Level Complete"
        );
    }
    else if (
        state ==
        GameState::GameOver
        )
    {
        window.setTitle(
            "CodeMaze - Game Over"
        );
    }
}

void Game::checkForChallenge()
{
    if (
        !fontLoaded ||
        questionManager
        .getQuestionCount() == 0
        )
    {
        return;
    }

    sf::Vector2f playerCenter =
        player.getBounds()
        .getCenter();

    for (
        const auto& cell :
        maze.getChallengeCells()
        )
    {
        if (
            isChallengeCompleted(
                cell
            )
            )
        {
            continue;
        }

        sf::FloatRect cellBounds =
            maze.getCellBounds(
                cell.first,
                cell.second
            );

        if (
            cellBounds.contains(
                playerCenter
            )
            )
        {
            startChallenge(
                cell
            );

            return;
        }
    }
}

bool Game::isChallengeCompleted(
    std::pair<int, int> cell
) const
{
    return completedChallenges
        .contains(
            cell
        );
}

void Game::startChallenge(
    std::pair<int, int> cell
)
{
    activeChallengeCell =
        cell;

    activeQuestion =
        questionManager
        .getAdaptiveQuestion(
            performanceTracker
        );

    if (!activeQuestion)
    {
        return;
    }

    challengeTimeRemaining =
        challengeTimeLimit;

    challengeClock.restart();

    challengeUI.open(
        activeQuestion,
        challengeTimeLimit
    );

    ++challengesAttempted;

    state =
        GameState::Challenge;

    updateWindowTitle();
}

void Game::submitChallenge()
{
    if (!activeQuestion)
    {
        return;
    }

    std::string answer =
        challengeUI.getAnswer();

    int timeUsed =
        challengeTimeLimit -
        challengeTimeRemaining;

    if (timeUsed < 0)
    {
        timeUsed = 0;
    }

    bool correct =
        activeQuestion->isCorrect(
            answer
        );

    performanceTracker
        .recordAttempt(
            activeQuestion->getTopic(),
            correct,
            timeUsed
        );

    if (correct)
    {
        scoreManager
            .addCorrectAnswer(
                BASE_CHALLENGE_POINTS
            );

        scoreManager
            .addTimeBonus(
                challengeTimeRemaining
            );

        ++challengesCorrect;

        completedChallenges.insert(
            activeChallengeCell
        );

        challengeUI.close();

        activeQuestion =
            nullptr;

        activeChallengeCell =
        {
            -1,
            -1
        };

        state =
            GameState::Playing;

        updateWindowTitle();
    }
    else
    {
        handleChallengeFailure(
            "Incorrect answer."
        );
    }
}

void Game::handleChallengeFailure(
    const std::string& reason
)
{
    if (!activeQuestion)
    {
        return;
    }

    --lives;

    ++livesLost;

    performanceTracker
        .recordLifeLost(
            activeQuestion->getTopic()
        );

    challengeUI.clearInput();

    challengeUI.setFeedback(
        reason +
        " You lost 1 life."
    );

    showHint();

    if (lives <= 0)
    {
        challengeUI.close();

        activeQuestion =
            nullptr;

        state =
            GameState::GameOver;

        updateWindowTitle();

        return;
    }

    challengeClock.restart();

    challengeTimeRemaining =
        challengeTimeLimit;

    challengeUI.setTimeRemaining(
        challengeTimeRemaining
    );

    updateWindowTitle();
}

void Game::showHint()
{
    if (!activeQuestion)
    {
        return;
    }

    std::string hint =
        hintEngine.generateHint(
            *activeQuestion
        );

    challengeUI.setHint(
        hint
    );

    performanceTracker
        .recordHint(
            activeQuestion->getTopic()
        );

    ++hintsUsed;

    scoreManager
        .addHintPenalty();
}

void Game::finishChallenge()
{
    challengeUI.close();

    activeQuestion =
        nullptr;

    activeChallengeCell =
    {
        -1,
        -1
    };

    state =
        GameState::Playing;

    updateWindowTitle();
}

void Game::finishLevel()
{
    state =
        GameState::Won;

    resultScreen.show(
        levelManager.getCurrentLevel(),
        scoreManager.getScore(),
        livesLost,
        hintsUsed,
        performanceTracker
    );

    state =
        GameState::Result;
}

void Game::drawText(
    const std::string& text,
    sf::Vector2f position,
    unsigned int size
)
{
    if (!fontLoaded)
    {
        return;
    }

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

void Game::drawMainMenu()
{
    if (!fontLoaded)
    {
        return;
    }

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
            14,
            28
        )
    );

    window.draw(
        background
    );

    drawText(
        "CODEMAZE",
        {
            205.0f,
            70.0f
        },
        48
    );

    drawText(
        "Adaptive C++ Learning Maze",
        {
            155.0f,
            130.0f
        },
        20
    );

    drawText(
        "Learn C++ by solving challenges inside a maze.",
        {
            105.0f,
            175.0f
        },
        17
    );

    const std::string items[4] =
    {
        "START GAME",
        "INSTRUCTIONS",
        "LEVEL SELECT",
        "EXIT"
    };

    float startY = 250.0f;

    for (int i = 0; i < 4; ++i)
    {
        bool selected =
            i == menuSelection;

        sf::RectangleShape button;

        button.setSize(
            {
                330.0f,
                52.0f
            }
        );

        button.setPosition(
            {
                160.0f,
                startY +
                i * 65.0f
            }
        );

        if (selected)
        {
            button.setFillColor(
                sf::Color(
                    80,
                    70,
                    170
                )
            );
        }
        else
        {
            button.setFillColor(
                sf::Color(
                    35,
                    40,
                    60
                )
            );
        }

        window.draw(
            button
        );

        drawText(
            std::to_string(i + 1) +
            ". " +
            items[i],
            {
                215.0f,
                startY +
                i * 65.0f +
                13.0f
            },
            20
        );
    }

    drawText(
        "Selected level: " +
        std::to_string(
            selectedLevel
        ),
        {
            245.0f,
            535.0f
        },
        17
    );

    drawText(
        "UP / DOWN = Navigate     ENTER = Select",
        {
            145.0f,
            575.0f
        },
        15
    );

    drawText(
        "1-4 = Quick selection",
        {
            230.0f,
            605.0f
        },
        14
    );
}

void Game::drawInstructions()
{
    if (!fontLoaded)
    {
        return;
    }

    sf::RectangleShape background;

    background.setSize(
        {
            650.0f,
            650.0f
        }
    );

    background.setFillColor(
        sf::Color(
            12,
            16,
            28
        )
    );

    window.draw(
        background
    );

    drawText(
        "HOW TO PLAY",
        {
            215.0f,
            35.0f
        },
        32
    );

    drawText(
        "CODEMAZE",
        {
            275.0f,
            80.0f
        },
        20
    );

    drawText(
        "1. Move through the maze using WASD or Arrow Keys.",
        {
            55.0f,
            125.0f
        },
        17
    );

    drawText(
        "2. Purple cells contain C++ programming challenges.",
        {
            55.0f,
            165.0f
        },
        17
    );

    drawText(
        "3. Select the correct answer to unlock that cell.",
        {
            55.0f,
            205.0f
        },
        17
    );

    drawText(
        "4. Every challenge has a time limit.",
        {
            55.0f,
            245.0f
        },
        17
    );

    drawText(
        "5. A wrong answer costs exactly one life.",
        {
            55.0f,
            285.0f
        },
        17
    );

    drawText(
        "6. Timeout also costs exactly one life.",
        {
            55.0f,
            325.0f
        },
        17
    );

    drawText(
        "7. After an unsuccessful attempt, a learning hint appears.",
        {
            55.0f,
            365.0f
        },
        17
    );

    drawText(
        "8. You start every level with 3 lives.",
        {
            55.0f,
            405.0f
        },
        17
    );

    drawText(
        "9. Solve every challenge before reaching the exit.",
        {
            55.0f,
            445.0f
        },
        17
    );

    drawText(
        "10. Your performance affects future question selection.",
        {
            55.0f,
            485.0f
        },
        17
    );

    drawText(
        "CONTROLS",
        {
            55.0f,
            535.0f
        },
        20
    );

    drawText(
        "WASD / ARROWS = Move",
        {
            55.0f,
            570.0f
        },
        16
    );

    drawText(
        "R = Restart Level",
        {
            300.0f,
            570.0f
        },
        16
    );

    drawText(
        "ESC / B = Back to Menu",
        {
            215.0f,
            610.0f
        },
        16
    );
}

void Game::drawLevelSelect()
{
    if (!fontLoaded)
    {
        return;
    }

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
            14,
            28
        )
    );

    window.draw(
        background
    );

    drawText(
        "SELECT LEVEL",
        {
            210.0f,
            55.0f
        },
        34
    );

    drawText(
        "Choose the maze and challenge difficulty",
        {
            165.0f,
            105.0f
        },
        17
    );

    const LevelSettings&
        settings =
        levelManager
        .getCurrentSettings();

    const std::string names[4] =
    {
        "BEGINNER",
        "INTERMEDIATE",
        "ADVANCED",
        "EXPERT"
    };

    const std::string mazeSizes[4] =
    {
        "10 x 10 Maze",
        "15 x 15 Maze",
        "20 x 20 Maze",
        "25 x 25 Maze"
    };

    const std::string times[4] =
    {
        "45 seconds",
        "35 seconds",
        "30 seconds",
        "25 seconds"
    };

    const std::string challenges[4] =
    {
        "4 challenges",
        "6 challenges",
        "8 challenges",
        "10 challenges"
    };

    float startY = 155.0f;

    for (int i = 0; i < 4; ++i)
    {
        bool selected =
            selectedLevel == i + 1;

        sf::RectangleShape card;

        card.setSize(
            {
                500.0f,
                75.0f
            }
        );

        card.setPosition(
            {
                75.0f,
                startY +
                i * 88.0f
            }
        );

        if (selected)
        {
            card.setFillColor(
                sf::Color(
                    80,
                    70,
                    170
                )
            );
        }
        else
        {
            card.setFillColor(
                sf::Color(
                    32,
                    38,
                    58
                )
            );
        }

        window.draw(
            card
        );

        drawText(
            std::to_string(i + 1) +
            ". " +
            names[i],
            {
                95.0f,
                startY +
                i * 88.0f +
                10.0f
            },
            19
        );

        drawText(
            mazeSizes[i] +
            "   |   " +
            times[i] +
            "   |   " +
            challenges[i],
            {
                95.0f,
                startY +
                i * 88.0f +
                42.0f
            },
            14
        );
    }

    drawText(
        "Selected: Level " +
        std::to_string(
            settings.levelNumber
        ) +
        " - " +
        settings.name,
        {
            190.0f,
            525.0f
        },
        18
    );

    drawText(
        "1-4 = Choose Level",
        {
            215.0f,
            565.0f
        },
        16
    );

    drawText(
        "ENTER = Start     ESC / B = Back",
        {
            180.0f,
            600.0f
        },
        16
    );
}

void Game::drawHUD()
{
    if (!fontLoaded)
    {
        return;
    }

    const LevelSettings&
        settings =
        levelManager
        .getCurrentSettings();

    sf::RectangleShape hud;

    hud.setSize(
        {
            650.0f,
            50.0f
        }
    );

    hud.setPosition(
        {
            0.0f,
            600.0f
        }
    );

    hud.setFillColor(
        sf::Color(
            12,
            15,
            25
        )
    );

    window.draw(
        hud
    );

    drawText(
        "LV " +
        std::to_string(
            settings.levelNumber
        ) +
        " " +
        settings.name,
        {
            10.0f,
            614.0f
        },
        15
    );

    drawText(
        "Lives: " +
        std::to_string(
            lives
        ),
        {
            145.0f,
            614.0f
        },
        15
    );

    drawText(
        "Score: " +
        std::to_string(
            scoreManager.getScore()
        ),
        {
            235.0f,
            614.0f
        },
        15
    );

    drawText(
        "Challenges: " +
        std::to_string(
            completedChallenges.size()
        ) +
        "/" +
        std::to_string(
            maze.getChallengeCells()
            .size()
        ),
        {
            345.0f,
            614.0f
        },
        15
    );

    drawText(
        "Accuracy: " +
        std::to_string(
            static_cast<int>(
                performanceTracker
                .getOverallAccuracy()
                )
        ) +
        "%",
        {
            515.0f,
            614.0f
        },
        15
    );
}