# CodeMaze executable

set(SFML_DIR "D:/Program files/SFML-3.1.0/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable(CodeMaze
    "CodeMaze.cpp"
    "CodeMaze.h"
)

set_property(TARGET CodeMaze PROPERTY CXX_STANDARD 20)
set_property(TARGET CodeMaze PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(CodeMaze PRIVATE SFML::Graphics)

# Automatically copy SFML runtime DLLs beside CodeMaze.exe
if (WIN32)
    add_custom_command(TARGET CodeMaze POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t
        "$<TARGET_FILE_DIR:CodeMaze>"
        $<TARGET_RUNTIME_DLLS:CodeMaze>
        COMMAND_EXPAND_LISTS
    )
endif()