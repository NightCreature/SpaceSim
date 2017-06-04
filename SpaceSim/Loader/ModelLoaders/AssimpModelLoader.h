#pragma once

#include "Graphics/Model.h"

#include <string>

class Resource;
class Material;

namespace AssimpModelLoader 
{

    CreatedModel LoadModel(Resource* resource, const Material& material, const std::string& fileName);

};

