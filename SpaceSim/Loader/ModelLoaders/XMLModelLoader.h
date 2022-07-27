#pragma once

#include "modelloader.h"

struct CommandList;

class XMLModelLoader : public ModelLoader
{
public:
    XMLModelLoader(void);
    ~XMLModelLoader(void);

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    CreatedModel LoadModel(Resource* resource, const LoadData& loadData, CommandList& commandList);

};

