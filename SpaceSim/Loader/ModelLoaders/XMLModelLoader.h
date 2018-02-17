#pragma once

#include "modelloader.h"

class XMLModelLoader : public ModelLoader
{
public:
    XMLModelLoader(void);
    ~XMLModelLoader(void);

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    CreatedModel LoadModel(Resource* resource, const LoadData& loadData);

};

