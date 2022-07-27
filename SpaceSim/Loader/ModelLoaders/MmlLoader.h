#pragma once

#include "Loader/ModelLoaders/ModelLoader.h"

struct CommandList;

class MmlLoader : public ModelLoader
{
public:
    MmlLoader() {}
    ~MmlLoader() {}
    virtual CreatedModel LoadModel(Resource* resource, const LoadData& loadData, CommandList& commandList) override;
private:
};