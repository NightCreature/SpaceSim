#pragma once

#include <string>

class Resource;

class ShaderPack
{
public:
    ShaderPack(Resource* resource) : m_resource(resource) {}
    ~ShaderPack(void) {}

    bool loadShaderPack(std::string shaderPack);
private:
    Resource* m_resource;
};

