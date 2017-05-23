#pragma once

#include "Loader/ModelLoaders/ModelLoader.h"

class MmlLoader : public ModelLoader
{
public:
    MmlLoader() {}
    ~MmlLoader() {}
    virtual CreatedModel LoadModel(Resource* resource, const LoadData& loadData) override;
private:
};