#pragma once

#include "modelloader.h"

class AssimpModelLoader : public ModelLoader
{
public:
    AssimpModelLoader(void);
    ~AssimpModelLoader(void);

    //-------------------------------------------------------------------------
    // @brief 
    //-------------------------------------------------------------------------
    Model* LoadModel(Resource* resource, const ShaderInstance& shaderInstance, const std::string& fileName);
private:
};

