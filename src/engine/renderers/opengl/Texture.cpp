//
// Created by Killian on 19/04/2023.
//
#include "engine/renderers/opengl/Texture.h"
#include "engine/glad/glad.h"
#include <stb_image.h>

namespace Engine::OpenGL
{

uint32_t Texture::s_boundId = 0;

Texture::Texture()
{
    if(m_id == 0)
        glGenTextures(1, &m_id);

        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef DEBUG
    if(m_id == 0)
        throw std::runtime_error("Failed to create texture");
#endif
}

Texture::Texture(uint32_t id)
    : m_id(id)
{
#ifdef DEBUG
    if(m_id == 0)
        throw std::runtime_error("Failed to create texture");
#endif

    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

Texture::Texture(Texture&& other) noexcept
{
    *this = std::move(other);
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    m_id = other.m_id;
    other.m_id = 0;

    return *this;
}

Texture::~Texture()
{
#ifdef DEBUG
    if(m_id == 0)
        spdlog::error("Tried to delete texture with an id of 0");
#endif

    glDeleteTextures(1, &m_id);
}

void Texture::LoadFromFile(const std::string_view& path)
{
    Bind();

    int width, height, channels;
    uint8_t* data = stbi_load(path.data(), &width, &height, &channels, 0);

    if(data == nullptr)
        throw std::runtime_error("Failed to load texture from file: " + std::string(path));

    if(channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if(channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    if(s_boundId != m_id)
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
        s_boundId = m_id;
    }
}

void Texture::Unbind()
{
    if (s_boundId != 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        s_boundId = 0;
    }
}

}