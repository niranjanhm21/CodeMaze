set(SFML_DIR "D:/Program files/SFML-3.1.0/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable(CodeMaze
    "../src/main.cpp"

    "../src/Game/Game.cpp"
    "../src/Game/Game.h"

    "../src/Game/LevelManager.cpp"
    "../src/Game/LevelManager.h"

    "../src/Maze/Cell.h"
    "../src/Maze/Maze.cpp"
    "../src/Maze/Maze.h"
    "../src/Maze/MazeGenerator.cpp"
    "../src/Maze/MazeGenerator.h"

    "../src/Player/Player.cpp"
    "../src/Player/Player.h"

    "../src/Questions/Question.cpp"
    "../src/Questions/Question.h"
    "../src/Questions/QuestionManager.cpp"
    "../src/Questions/QuestionManager.h"

    "../src/Learning/HintEngine.cpp"
    "../src/Learning/HintEngine.h"

    "../src/Learning/PerformanceTracker.cpp"
    "../src/Learning/PerformanceTracker.h"

    "../src/Learning/AdaptiveEngine.cpp"
    "../src/Learning/AdaptiveEngine.h"

    "../src/Scoring/ScoreManager.cpp"
    "../src/Scoring/ScoreManager.h"

    "../src/UI/ChallengeUI.cpp"
    "../src/UI/ChallengeUI.h"

    "../src/UI/ResultScreen.cpp"
    "../src/UI/ResultScreen.h"
)

set_property(
    TARGET CodeMaze
    PROPERTY CXX_STANDARD 20
)

set_property(
    TARGET CodeMaze
    PROPERTY CXX_STANDARD_REQUIRED ON
)

target_link_libraries(
    CodeMaze
    PRIVATE SFML::Graphics
)

if (WIN32)

    add_custom_command(
        TARGET CodeMaze
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy -t
        "$<TARGET_FILE_DIR:CodeMaze>"
        $<TARGET_RUNTIME_DLLS:CodeMaze>

        COMMAND_EXPAND_LISTS

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../data"
        "$<TARGET_FILE_DIR:CodeMaze>/data"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../assets"
        "$<TARGET_FILE_DIR:CodeMaze>/assets"
    )

endif()