#pragma once

#include "modelloader.h"

class XMLModelLoader : public ModelLoader
{
public:
    XMLModelLoader(void);
    ~XMLModelLoader(void);

    //-------------------------------------------------------------------------
    // @brief 
    //-------------------------------------------------------------------------
    Model* LoadModel(Resource* resource, const ShaderInstance& shaderInstance, const std::string& fileName);

};

