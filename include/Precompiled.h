//
// Created by Killian on 14/03/2023.
//

#pragma once

#define _USE_MATH_DEFINES

// Include these headers in all compiled files, doing this makes the compile-time go way lower
// mainly because spdlog is a huge library that needs to be recompiled every time it is included
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>

// Detect Platform
#ifdef _WIN32
#define PLATFORM_WIN32
#define WIN32_LEAN_AND_MIN
#include <windows.h>
#elif __linux__
#define PLATFORM_LINUX
#elif __APPLE__
#define PLATFORM_MACOS
#endif