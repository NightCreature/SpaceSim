#pragma once

#include <string>

struct TextureLoadRequest
{
    TextureLoadRequest(const std::string& name, size_t slotId) : m_textureName(name), m_textureSlotId(slotId) {}

    std::string m_textureName;
    size_t m_textureSlotId;
};