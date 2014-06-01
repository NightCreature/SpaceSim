#pragma once

#include <string>

class Model;
class Resource;

class ModelLoader
{
public:
    ModelLoader() {}
    virtual ~ModelLoader() {}

    virtual Model* LoadModel(Resource* resource, const std::string& fileName) = 0;
};

