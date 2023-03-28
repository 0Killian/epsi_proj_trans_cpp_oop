cmake_minimum_required(VERSION 3.24.2)
project(Stardew LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git)
FetchContent_MakeAvailable(SFML)

add_executable(Stardew
        src/main.cpp
        src/Application.cpp
        src/RandomNumberGenerator.cpp
        src/MainMenuScene.cpp
        src/ThreadPool.cpp
        src/GameGrid.cpp
        src/tiles/GroundTile.cpp
        src/tiles/PassagePointTile.cpp
        src/tiles/WallTile.cpp
        src/tiles/PathTile.cpp
        src/tiles/SoilTile.cpp)
target_include_directories(Stardew PRIVATE include)
target_precompile_headers(Stardew PRIVATE include/Precompiled.h)
target_link_libraries(Stardew PRIVATE sfml-graphics)
target_compile_features(Stardew PRIVATE cxx_std_23)
if (WIN32)
    if(BUILD_SHARED_LIBS)
        add_custom_command(TARGET Stardew POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Stardew> $<TARGET_FILE_DIR:Stardew> COMMAND_EXPAND_LISTS)
    endif()
    target_link_libraries(Stardew PRIVATE opengl32)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG DEBUG)
endif()

install(TARGETS Stardew)