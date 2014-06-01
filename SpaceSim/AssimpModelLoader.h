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
    Model* LoadModel( Resource* resource,  const std::string& fileName );
private:
};

