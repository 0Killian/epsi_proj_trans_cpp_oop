cmake_minimum_required(VERSION 3.24.4)
project(HarvestHaven LANGUAGES CXX)


# Compile Shaders
if(NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "VULKAN_SDK environment variable not set. Make sure that the Vulkan SDK is installed and that the VULKAN_SDK environment variable is set.")
endif()



# Add extension corresponding to OS
if(WIN32)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
    else()
        set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
    endif()
elseif(UNIX)
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/bin/glslangValidator")
elseif(APPLE)
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/macOS/bin/glslangValidator")
endif()

if(NOT EXISTS ${GLSL_VALIDATOR})
    message(FATAL_ERROR "glslangValidator not found. Make sure that the Vulkan SDK is installed and that the VULKAN_SDK environment variable is set.")
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/shaders/*.vert"
        "${PROJECT_SOURCE_DIR}/shaders/*.frag")

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/working_directory/assets/shaders/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/working_directory/assets/shaders/"
            COMMAND ${GLSL_VALIDATOR} -V -g ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES})



# Compile main program
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(SPIRV_CROSS
        GIT_REPOSITORY https://github.com/Unity-Technologies/SPIRV-Cross.git)
FetchContent_MakeAvailable(SPIRV_CROSS)

find_package(Stb REQUIRED)


add_executable(HarvestHaven
        src/main.cpp
        src/engine/Application.cpp
        src/engine/RandomNumberGenerator.cpp
        src/MainMenuScene.cpp
        src/engine/ThreadPool.cpp
        src/GameGrid.cpp
        src/tiles/GroundTile.cpp
        src/tiles/PassagePointTile.cpp
        src/tiles/PathTile.cpp
        src/tiles/SoilTile.cpp
        src/Player.cpp
        src/engine/GUILayer.cpp
        src/Item.cpp
        src/Inventory.cpp
        src/engine/CollisionUtils.cpp
        src/Hotbar.cpp
        src/items/Hoe.cpp
        src/items/WateringCan.cpp
        src/items/Carrot.cpp
        src/engine/Windows/Window.cpp
        src/engine/renderers/vulkan/VulkanRenderer.cpp
        src/engine/renderers/directx/DirectXRenderer.cpp
        src/engine/renderers/metal/MetalRenderer.cpp
        src/engine/Windows/opengl/OpenGLRenderer.cpp
        src/engine/glad.cpp
        src/engine/glad_wgl.cpp
        src/engine/renderers/opengl/VertexBuffer.cpp
        src/engine/renderers/opengl/IndexBuffer.cpp
        src/engine/renderers/opengl/Shader.cpp
        src/engine/renderers/opengl/Pipeline.cpp
        src/engine/Linux/Window.cpp
        src/engine/Linux/opengl/OpenGLRenderer.cpp
        src/engine/glad_glx.cpp
        src/engine/renderers/opengl/Texture.cpp src/engine/stbi_impl.cpp)

target_include_directories(HarvestHaven PRIVATE include)
target_include_directories(HarvestHaven PRIVATE ${Stb_INCLUDE_DIR})
target_precompile_headers(HarvestHaven PRIVATE include/Precompiled.h)
target_link_libraries(HarvestHaven PRIVATE sfml-graphics)
target_link_libraries(HarvestHaven PRIVATE spirv-cross-cpp)

if(UNIX)
    target_link_libraries(HarvestHaven PRIVATE X11)
    target_link_libraries(HarvestHaven PRIVATE dl)
    target_link_libraries(HarvestHaven PRIVATE pthread)
endif()

add_dependencies(HarvestHaven Shaders)

target_compile_features(HarvestHaven PRIVATE cxx_std_20)
if (WIN32)
    if(BUILD_SHARED_LIBS)
        add_custom_command(TARGET HarvestHaven POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:HarvestHaven> $<TARGET_FILE_DIR:HarvestHaven> COMMAND_EXPAND_LISTS)
    endif()
    target_link_libraries(HarvestHaven PRIVATE opengl32)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG DEBUG)
endif()

install(TARGETS HarvestHaven)

add_executable(GenerateTilemap scripts/GenerateTilemap.cpp)
target_compile_features(GenerateTilemap PRIVATE cxx_std_20)