#pragma once

#include <string>

class Model;
class Resource;

struct LoadModelResource
{
    char m_fileName[256];
};

class ModelLoader
{
public:
    ModelLoader() {}
    virtual ~ModelLoader() {}

    virtual Model* LoadModel(Resource* resource, const std::string& fileName) = 0;
};

