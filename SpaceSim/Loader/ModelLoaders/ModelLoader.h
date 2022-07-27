#pragma once

#include "Graphics/Model.h"

#include <string>

class Resource;
struct CommandList;

struct LoadModelResource
{
    char m_fileName[256];
    size_t m_effectHash;
};

class ModelLoader
{
public:
    ModelLoader() {}
    virtual ~ModelLoader() {}

    struct LoadData
    {
        std::string m_fileName;
    };

    virtual CreatedModel LoadModel(Resource* resource, const LoadData& loadData, CommandList& commandList) = 0;
};

