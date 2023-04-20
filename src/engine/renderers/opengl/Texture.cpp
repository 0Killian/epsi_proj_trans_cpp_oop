//
// Created by Killian on 19/04/2023.
//
#include "engine/renderers/opengl/Texture.h"
#include "engine/glad/glad.h"
#include <stb_image.h>

namespace Engine::OpenGL
{

Texture::Texture()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

    Init();

#ifdef DEBUG
    if(m_id == 0) throw std::runtime_error("Failed to create texture");
#endif
}

Texture::Texture(uint32_t id) : m_id(id)
{
#ifdef DEBUG
    if (m_id == 0) throw std::runtime_error("Failed to create texture");
#endif

    Init();
}

void Texture::Init()
{
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

std::vector<std::shared_ptr<Engine::Texture>> Texture::CreateTextures(size_t count)
{
    std::vector<uint32_t> ids(count);
    glGenTextures(static_cast<GLsizei>(count), ids.data());
    std::vector<std::shared_ptr<Engine::Texture>> textures(count);
    std::transform(ids.begin(), ids.end(), textures.begin(),
       [](uint32_t id) { return std::shared_ptr<Engine::Texture>(new Texture(id));});
    return textures;
}

Texture::~Texture()
{
#ifdef DEBUG
    if(m_id == 0)
        spdlog::error("Tried to delete texture with an id of 0");
#endif

    glDeleteTextures(1, &m_id);
}

// TODO: Use std::filesystem::path
void Texture::LoadFromFile(const std::string_view& path)
{
    int width, height, channels;
    uint8_t* data = stbi_load(path.data(), &width, &height, &channels, 0);

    if(data == nullptr) throw std::runtime_error("Failed to load texture from file: " + std::string(path));

    // TODO: make configurable
    glTextureStorage2D(m_id, 1, GL_RGBA8, width, height);

    if(channels == 3) glTextureSubImage2D(m_id, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if(channels == 4) glTextureSubImage2D(m_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else throw std::runtime_error("Unsupported number of channels for texture: " + std::string(path));

    stbi_image_free(data);
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

}