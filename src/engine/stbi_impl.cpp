//
// Created by Killian on 20/04/2023.
//

// The __OPTIMIZE__ macro is defined by SPDLOG, and it causes compilation errors with Linux and stb_image.h
#if defined(PLATFORM_LINUX) && defined(DEBUG) && defined(__OPTIMIZE__)
#undef __OPTIMIZE__
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>