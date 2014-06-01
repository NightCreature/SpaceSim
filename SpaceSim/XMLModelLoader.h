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
    Model* LoadModel( Resource* resource, const std::string& fileName );

};

