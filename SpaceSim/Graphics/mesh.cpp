#include "Graphics/mesh.h"
#include <fstream>
#include <iostream>
#include "Graphics/texture.h"
#include "Graphics/texturemanager.h"
#include "Application/BaseApplication.h"
#include "Graphics/DeviceManager.h"
#include "Core/Types/TypeHelpers.h"
#ifdef  _WIN32
#include <windows.h>
#endif//_WIN32



#include "assert.h"

namespace Mesh
{

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
CreatedModel CreateMesh(const CreationParams& params, const EffectCache& effectCache)
{
    CreatedModel mesh;

    mesh.model = new Model();

    for (size_t counter = 0; counter < params.m_meshGroups.size(); ++counter)
    {
        mesh.model->addMeshGroup(params.m_meshGroups[counter].meshGroup);
        mesh.boundingBox.enclose(params.m_meshGroups[counter].boundingBox);
    }

    mesh.model->CalculateSortKey(effectCache);
    return mesh;
}

void normalizeNormals(std::vector<Vector3>& normals)
{
    for (int i = 0; i < normals.size(); i++)
    {
        normals[i].normalize();
    }
}

}