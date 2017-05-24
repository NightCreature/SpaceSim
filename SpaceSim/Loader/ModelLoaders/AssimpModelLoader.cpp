#include "Loader/ModelLoaders/AssimpModelLoader.h"
#include "Graphics/mesh.h"
#include "Graphics/MeshGroupCreator.h"
#include "Graphics/texturemanager.h"
#include "Core/Resource/RenderResource.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace AssimpModelLoader
{

//-----------------------------------------------------------------------------
//! @brief   Initialise the application
//! @remark
//-----------------------------------------------------------------------------
CreatedModel LoadModel(Resource* resource, const Material& material, const std::string& fileName)
{
    UNUSEDPARAM(material);

    if (fileName.empty())
    {
        return CreatedModel();
    }

    //Load the model here
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing  
    // Usually - if speed is not the most important aspect for you - you'll   
    // propably to request more postprocessing than we do in this example.  aiProcessPreset_TargetRealtime_Quality
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FlipWindingOrder); //Optimize the mesh and scenegraph to reduce drawcalls
    if (!scene)
    {
        MSG_TRACE_CHANNEL("ASSIMPMODELLOADER", "failed to open the model file ( %s ) importer error: %s", fileName.c_str(), importer.GetErrorString())
            return CreatedModel();
    }  // Now we can access the file's contents.

    MSG_TRACE_CHANNEL("ASSIMP LOADER", "Trying to load model %s", fileName.c_str());

    //Grab the verts here
    Mesh::CreationParams params;
    ShaderInstance shaderInstance;
    shaderInstance.setMaterial(material);
    params.m_shaderInstance = const_cast<ShaderInstance*>(&shaderInstance);
    params.m_resource = resource;
    unsigned int highestIndex = 0;
    //Extract vertices from the mesh here and store in our own vertex buffer
    for (size_t meshCounter = 0; meshCounter < scene->mNumMeshes; ++meshCounter)
    {
        MeshGroupCreator::CreationParams meshGroupParams;
        aiMesh* subMesh = scene->mMeshes[meshCounter];
        for (size_t vertCounter = 0; vertCounter < subMesh->mNumVertices; ++vertCounter)
        {
            meshGroupParams.m_vertices.push_back(Vector3(subMesh->mVertices[vertCounter].x, subMesh->mVertices[vertCounter].y, subMesh->mVertices[vertCounter].z));

            if (subMesh->HasNormals())
            {
                meshGroupParams.m_normals.push_back(Vector3(-subMesh->mNormals[vertCounter].x, -subMesh->mNormals[vertCounter].y, -subMesh->mNormals[vertCounter].z));
                //MSG_TRACE_CHANNEL("NORMAL DEBUG", "Normal: %f, %f, %f", subMesh->mNormals[vertCounter].x, subMesh->mNormals[vertCounter].y, subMesh->mNormals[vertCounter].z);
            }
            if (subMesh->mTangents != nullptr)
            {
                meshGroupParams.m_tangents.push_back(Vector3(-subMesh->mTangents[vertCounter].x, -subMesh->mTangents[vertCounter].y, -subMesh->mTangents[vertCounter].z));
            }
            if (subMesh->mBitangents != nullptr)
            {
                meshGroupParams.m_tangents.push_back(Vector3(-subMesh->mBitangents[vertCounter].x, -subMesh->mBitangents[vertCounter].y, -subMesh->mBitangents[vertCounter].z));
            }

            for (unsigned int uvChannel = 0; uvChannel < subMesh->GetNumUVChannels(); ++uvChannel)
            {
                if (subMesh->HasTextureCoords(uvChannel))
                {
                    aiVector3D* texCoordChannel = subMesh->mTextureCoords[uvChannel];
                    while (meshGroupParams.m_texcoords.size() <= uvChannel)
                    {
                        meshGroupParams.m_texcoords.push_back(std::vector<Vector3>());
                    }
                    switch (subMesh->mNumUVComponents[uvChannel])
                    {
                    case 1:
                    {
                        Vector3 vec(texCoordChannel[vertCounter].x, 0.0f, 0.0f);
                        std::vector<Vector3>& smit = meshGroupParams.m_texcoords[uvChannel];
                        smit.push_back(vec);
                    }
                    break;
                    case 2:
                    {
                        Vector3 vec(texCoordChannel[vertCounter].x, texCoordChannel[vertCounter].y, 0.0f);
                        std::vector<Vector3>& smit = meshGroupParams.m_texcoords[uvChannel];
                        smit.push_back(vec);
                    }
                    break;
                    case 3:
                    {
                        Vector3 vec(texCoordChannel[vertCounter].x, texCoordChannel[vertCounter].y, texCoordChannel[vertCounter].z);
                        std::vector<Vector3>& smit = meshGroupParams.m_texcoords[uvChannel];

                        smit.push_back(vec);

                        //    uvs.push_back(texCoordChannel[vertCounter].x);
                        //    uvs.push_back(texCoordChannel[vertCounter].y);
                        //    uvs.push_back(texCoordChannel[vertCounter].z);
                    }
                    break;
                    default:
                        break;
                    }
                }
            }
        }

        //Need to keep track of highest index and add it to the next batch and so one sadly
        unsigned int baseIndexOffset = highestIndex;
        for (size_t indexCounter = 0; indexCounter < subMesh->mNumFaces; ++indexCounter)
        {
            for (size_t counterIndex = 0; counterIndex < subMesh->mFaces[indexCounter].mNumIndices; ++counterIndex)
            {
                meshGroupParams.m_indices.push_back(subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset);
                if (subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset > highestIndex)
                {
                    highestIndex = subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset;
                }
            }
        }

        meshGroupParams.m_shaderInstance = shaderInstance;
        aiMaterial* aimaterial = scene->mMaterials[subMesh->mMaterialIndex];
        Material shaderMaterial = meshGroupParams.m_shaderInstance.getMaterial();
        aiColor4D color;
        aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
        shaderMaterial.setAmbient(Color(color.r, color.g, color.b, color.a));

        aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        shaderMaterial.setDiffuse(Color(color.r, color.g, color.b, color.a));

        aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
        shaderMaterial.setSpecular(Color(color.r, color.g, color.b, color.a));

        aimaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        shaderMaterial.setEmissive(Color(color.r, color.g, color.b, color.a));

        float shininess;
        aimaterial->Get(AI_MATKEY_SHININESS, shininess);
        shaderMaterial.setShininess(shininess);

        //load the texture maps here
        RenderResourceHelper gameResource(resource);
        TextureManager& tm = gameResource.getWriteableResource().getTextureManager();
        DeviceManager& dm = gameResource.getWriteableResource().getDeviceManager();
        aiString path;
        aiTextureMapping uvMapping;
        unsigned int uv_index = 0xFFFFFFFF;
        //SHould early out on all of these when we cant go past slots any more or we are not getting a slot
        for (size_t counter = 0; counter < aimaterial->GetTextureCount(aiTextureType_AMBIENT) && counter < Material::TextureSlotMapping::Ambient7; ++counter)
        {
            if (aiReturn_SUCCESS == aimaterial->GetTexture(aiTextureType_AMBIENT, static_cast<unsigned int>(counter), &path, &uvMapping, &uv_index))
            {
                tm.addLoad(dm, path.C_Str());
                shaderMaterial.addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(path.C_Str())), static_cast<Material::TextureSlotMapping::TextureSlot>(Material::TextureSlotMapping::Ambient0 + counter)));
            }
        }
        for (size_t counter = 0; counter < aimaterial->GetTextureCount(aiTextureType_DIFFUSE) && counter < Material::TextureSlotMapping::Diffuse7; ++counter)
        {
            if (aiReturn_SUCCESS == aimaterial->GetTexture(aiTextureType_DIFFUSE, static_cast<unsigned int>(counter), &path, &uvMapping, &uv_index))
            {
                tm.addLoad(dm, path.C_Str());
                shaderMaterial.addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(path.C_Str())), static_cast<Material::TextureSlotMapping::TextureSlot>(Material::TextureSlotMapping::Diffuse0 + counter)));
            }
        }
        for (size_t counter = 0; counter < aimaterial->GetTextureCount(aiTextureType_EMISSIVE) && counter < Material::TextureSlotMapping::Emmisive7; ++counter)
        {
            if (aiReturn_SUCCESS == aimaterial->GetTexture(aiTextureType_EMISSIVE, static_cast<unsigned int>(counter), &path, &uvMapping, &uv_index))
            {
                tm.addLoad(dm, path.C_Str());
                shaderMaterial.addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(path.C_Str())), static_cast<Material::TextureSlotMapping::TextureSlot>(Material::TextureSlotMapping::Emmisive0 + counter)));
            }
        }
        for (size_t counter = 0; counter < aimaterial->GetTextureCount(aiTextureType_SPECULAR) && counter < Material::TextureSlotMapping::Specular7; ++counter)
        {
            if (aiReturn_SUCCESS == aimaterial->GetTexture(aiTextureType_SPECULAR, static_cast<unsigned int>(counter), &path, &uvMapping, &uv_index))
            {
                tm.addLoad(dm, path.C_Str());
                shaderMaterial.addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(path.C_Str())), static_cast<Material::TextureSlotMapping::TextureSlot>(Material::TextureSlotMapping::Specular0 + counter)));
            }
        }

        if (aiReturn_SUCCESS == aimaterial->GetTexture(aiTextureType_NORMALS, 0, &path, &uvMapping, &uv_index))
        {
            tm.addLoad(dm, path.C_Str());
            shaderMaterial.addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(path.C_Str())), Material::TextureSlotMapping::Normals));
        }
        MSG_TRACE_CHANNEL("ASSIMP LOADER", "Trying to read material %d", aimaterial);

        meshGroupParams.m_numtangents = meshGroupParams.m_tangents.size(); //TODO FIX this needs to look at the model to get this.
        meshGroupParams.m_vertexDeclaration.position = 3;
        meshGroupParams.m_vertexDeclaration.normal = meshGroupParams.m_normals.size() > 0;
        meshGroupParams.m_vertexDeclaration.tangent = meshGroupParams.m_tangents.size() > 0;
        meshGroupParams.m_vertexDeclaration.textureCoordinateDimensions.clear();
        for (size_t counter = 0; counter < meshGroupParams.m_texcoords.size(); ++counter)
        {
            meshGroupParams.m_vertexDeclaration.textureCoordinateDimensions.push_back(2);
        }
        meshGroupParams.m_vertexDeclaration.vertexColor = false;
        meshGroupParams.m_resource = resource;

        //Add mesh group to the mesh creator params
        params.m_meshGroups.push_back(MeshGroupCreator::CreateMeshGroup(meshGroupParams));
    }



    return  Mesh::CreateMesh(params);
}

}