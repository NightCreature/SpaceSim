#pragma once

#include <string>

class Model;
class Resource;
class ShaderInstance;

class ModelLoader
{
public:
    ModelLoader() {}
    virtual ~ModelLoader() {}

    virtual Model* LoadModel(Resource* resource, const ShaderInstance& shaderInstance, const std::string& fileName) = 0;
};

