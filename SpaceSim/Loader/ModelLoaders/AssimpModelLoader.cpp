#include "Loader/ModelLoaders/AssimpModelLoader.h"
#include "Graphics/D3D12/DescriptorHeapManager.h"
#include "Graphics/mesh.h"
#include "Graphics/MeshGroupCreator.h"
#include "Graphics/texturemanager.h"
#include "Core/Resource/RenderResource.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "../ResourceLoader.h"
#include "../ResourceLoadJobs.h"


#include <thread>
#include "Graphics/Model/MeshHelperFunctions.h"
#include <string>
#include <Math/Assimp/AssimpMathHelpers.h>

namespace AssimpModelLoader
{

///-----------------------------------------------------------------------------
///! @brief   
///! @remark  This needs re writing so we can apply uv transforms to correct streams and embedded correct uv streams in to the vbs.
///-----------------------------------------------------------------------------
CreatedModel LoadModel(Resource* resource, const Material& material, const std::string& fileName, CommandList& commandList)
{
    UNUSEDPARAM(material);

    CreatedModel model;

    if (!fileName.empty())
    {
        model.model = new Model();
        //Load the model here
        Assimp::Importer importer;
        // And have it read the given file with some example postprocessing  
        // Usually - if speed is not the most important aspect for you - you'll   
        // propably to request more postprocessing than we do in this example.  aiProcessPreset_TargetRealtime_Quality
        const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_OptimizeMeshes| aiProcess_OptimizeGraph | aiProcess_FlipWindingOrder | aiProcess_FlipUVs); //Optimize the mesh and scenegraph to reduce drawcalls
        if (!scene)
        {
            MSG_TRACE_CHANNEL("ASSIMPMODELLOADER", "failed to open the model file ( %s ) importer error: %s", fileName.c_str(), importer.GetErrorString())
            return CreatedModel();
        }  // Now we can access the file's contents.

        MSG_TRACE_CHANNEL("ASSIMP LOADER", "Trying to load model %s", fileName.c_str());

        //Grab the verts here
        RenderResourceHelper renderResourceHelper(resource);
        RenderResource& renderResource = renderResourceHelper.getWriteableResource();

        unsigned int highestIndex = 0;
        //Extract vertices from the mesh here and store in our own vertex buffer

        model.model->CreateNrMeshGroups(scene->mNumMeshes);
        auto& meshGroups = model.model->getMeshData();
        for (size_t meshCounter = 0; meshCounter < scene->mNumMeshes; ++meshCounter)
        {
            MeshGroup& group = meshGroups[meshCounter];

            aiMesh* subMesh = scene->mMeshes[meshCounter];

            //size_t sourceDataStreamsSize = 1; //Position Stream
            VertexDeclarationDescriptor descriptor;
            descriptor.normal = subMesh->HasNormals();
            descriptor.tangent = subMesh->HasTangentsAndBitangents();
            //Multiple streams :(
            descriptor.vertexColor = subMesh->HasVertexColors(0);

            for (unsigned int uvChannel = 0; uvChannel < subMesh->GetNumUVChannels(); ++uvChannel)
            {
                if (subMesh->HasTextureCoords(uvChannel))
                {
                    descriptor.textureCoordinateDimensions.push_back(subMesh->mNumUVComponents[uvChannel]);
                }
            }

            VertexDataStreams dataStream = CreateDataStreams(descriptor);

            Matrix44 matrix;
            matrix.identity();
            //Have to check if a node in the scene is containing a transform that should be applied to this mesh data
            //for (size_t counter = 0; counter < scene->mRootNode->mNumChildren; ++counter)
            //{
            //    aiNode* node = scene->mRootNode->mChildren[counter];
            //    if (node != nullptr)
            //    {
            //        if (node->mName == subMesh->mName)
            //        {
            //            //Found a node
            //            matrix = AssimpHelpers::ToMatrix(node->mTransformation);
            //        }
            //    }
            //}

            size_t dataStreamIndex = 0;
            for (size_t vertCounter = 0; vertCounter < subMesh->mNumVertices; ++vertCounter)
            {
                std::vector<Vector3>& positionStream = std::get<2>(dataStream.m_streams[VertexStreamType::Position]);
                ++dataStreamIndex;
                //Need to create a descriptor here and fetch and set the data streams
                Vector4 position = AssimpHelpers::ToVector(subMesh->mVertices[vertCounter]);
                position = position * matrix;
                positionStream.push_back(Vector3(position.x(), position.y(), position.z()));

                if (subMesh->HasNormals())
                {
                    std::vector<Vector3>& normalStream = std::get<2>(dataStream.m_streams[VertexStreamType::Normal]);
                    ++dataStreamIndex;
                    normalStream.push_back(Vector3(-subMesh->mNormals[vertCounter].x, -subMesh->mNormals[vertCounter].y, -subMesh->mNormals[vertCounter].z));
                    //MSG_TRACE_CHANNEL("NORMAL DEBUG", "Normal: %f, %f, %f", subMesh->mNormals[vertCounter].x, subMesh->mNormals[vertCounter].y, subMesh->mNormals[vertCounter].z);
                }

                if (subMesh->mTangents != nullptr)
                {
                    std::vector<Vector3>& tangentStream = std::get<2>(dataStream.m_streams[VertexStreamType::Tangent]);
                    ++dataStreamIndex;
                    tangentStream.push_back(Vector3(-subMesh->mTangents[vertCounter].x, -subMesh->mTangents[vertCounter].y, -subMesh->mTangents[vertCounter].z));
                }
                
                if (subMesh->mBitangents != nullptr)
                {
                    //std::vector<Vector3>& biTangentStream = std::get<2>(dataStream.m_streams[VertexStreamType::BiTangent]);
                    //++dataStreamIndex;
                    //biTangentStream.push_back(Vector3(-subMesh->mBitangents[vertCounter].x, -subMesh->mBitangents[vertCounter].y, -subMesh->mBitangents[vertCounter].z));

                    //meshGroupParams.m_tangents.push_back(Vector3(-subMesh->mBitangents[vertCounter].x, -subMesh->mBitangents[vertCounter].y, -subMesh->mBitangents[vertCounter].z));
                }

                if (subMesh->HasVertexColors(0))
                {
                    std::vector<Color>& colorStream = std::get<4>(dataStream.m_streams[VertexStreamType::Color]);
                    colorStream.push_back(Color(subMesh->mColors[0]->r, subMesh->mColors[0]->g, subMesh->mColors[0]->b, subMesh->mColors[0]->a));
                    ++dataStreamIndex;
                }

                size_t uvStreamStart = static_cast<std::underlying_type_t<VertexStreamType>>(VertexStreamType::UVStart);
                for (unsigned int uvChannel = 0; uvChannel < subMesh->GetNumUVChannels(); ++uvChannel)
                {
                    if (subMesh->HasTextureCoords(uvChannel))
                    {
                        aiVector3D* texCoordChannel = subMesh->mTextureCoords[uvChannel];

                        switch (subMesh->mNumUVComponents[uvChannel])
                        {
                        case 1:
                        {
                            std::vector<float>& uStream = std::get<0>(dataStream.m_streams[static_cast<VertexStreamType>(uvStreamStart + uvChannel)]);
                            uStream.push_back(texCoordChannel[vertCounter].x);
                        }
                        break;
                        case 2:
                        {
                            std::vector<Vector2>& uvStream = std::get<1>(dataStream.m_streams[static_cast<VertexStreamType>(uvStreamStart + uvChannel)]);
                            uvStream.push_back(Vector2(texCoordChannel[vertCounter].x, texCoordChannel[vertCounter].y));
                        }
                        break;
                        case 3:
                        {
                            std::vector<Vector3>& uvwStream = std::get<2>(dataStream.m_streams[static_cast<VertexStreamType>(uvStreamStart + uvChannel)]);
                            uvwStream.push_back(Vector3(texCoordChannel[vertCounter].x, texCoordChannel[vertCounter].y, texCoordChannel[vertCounter].z));
                        }
                        break;
                        default:
                            break;
                        }
                    }

                    ++dataStreamIndex;
                }
            }



            MeshResourceIndices& resourceIndices = group.GetResourceInices();
            resourceIndices = group.GetVB().CreateBuffer(renderResource.getDeviceManager(), commandList, renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap(), dataStream);

            
            //Need to keep track of highest index and add it to the next batch and so one sadly
            unsigned int baseIndexOffset = 0;
            std::vector<uint32> indexBuffer;
            for (size_t indexCounter = 0; indexCounter < subMesh->mNumFaces; ++indexCounter)
            {
                for (size_t counterIndex = 0; counterIndex < subMesh->mFaces[indexCounter].mNumIndices; ++counterIndex)
                {
                    indexBuffer.push_back(subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset);
                    //if (subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset > highestIndex)
                    //{
                    //    highestIndex = subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset;
                    //}
                }
            }
            group.GetIB().Create(renderResource.getDeviceManager(), commandList, static_cast<unsigned int>(indexBuffer.size()) * sizeof(unsigned int), static_cast< void*>(indexBuffer.data()));

            aiMaterial* aimaterial = scene->mMaterials[subMesh->mMaterialIndex];
            Material& shaderMaterial = group.GetMaterial();
            shaderMaterial = material;
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
            ResourceLoader& resourceLoader = renderResource.getResourceLoader();
            aiString path;
            aiTextureMapping uvMapping;
            unsigned int uv_index = 0xFFFFFFFF;


            //SHould early out on all of these when we cant go past slots any more or we are not getting a slot
            for (size_t counter = 0; counter < aimaterial->GetTextureCount(aiTextureType_AMBIENT) && counter < Material::TextureSlotMapping::Ambient7; ++counter)
            {
                if (aiReturn_SUCCESS == aimaterial->GetTexture(aiTextureType_AMBIENT, static_cast<unsigned int>(counter), &path, &uvMapping, &uv_index))
                {
                    LoadRequest loadRequest;
                    loadRequest.m_gameObjectId = 0;
                    loadRequest.m_resourceType = hashString("LOAD_TEXTURE");
                    loadRequest.m_loadData = static_cast<void*>(new char[256]);
                    memcpy(loadRequest.m_loadData, path.C_Str(), 256);
                    resourceLoader.AddLoadRequest(loadRequest);

                    shaderMaterial.addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(path.C_Str())), static_cast<Material::TextureSlotMapping::TextureSlot>(Material::TextureSlotMapping::Ambient0 + counter)));
                }
            }
            for (size_t counter = 0; counter < aimaterial->GetTextureCount(aiTextureType_DIFFUSE) && counter < Material::TextureSlotMapping::Diffuse7; ++counter)
            {
                if (aiReturn_SUCCESS == aimaterial->GetTexture(aiTextureType_DIFFUSE, static_cast<unsigned int>(counter), &path, &uvMapping, &uv_index))
                {
                    int val;
                    aiTextureType type = aiTextureType_DIFFUSE;
                    if (AI_SUCCESS == aiGetMaterialInteger(aimaterial, AI_MATKEY_UVWSRC(type, 0), &val))
                    {
                    }
                    //Grab all texture information for this stage as texture coordinates could need to be transformed :(
                    aiUVTransform textureTransform;
                    const aiMaterialProperty* property = nullptr;
                    if (AI_SUCCESS == aiGetMaterialProperty(aimaterial, AI_MATKEY_UVTRANSFORM(aiTextureType_DIFFUSE, static_cast<unsigned int>(counter)), &property))
                    {
                        if (property != nullptr)
                        {
                            textureTransform = *(aiUVTransform*)(property->mData);
                        }
                    }

                    LoadRequest loadRequest;
                    loadRequest.m_gameObjectId = 0;
                    loadRequest.m_resourceType = hashString("LOAD_TEXTURE");
                    loadRequest.m_loadData = static_cast<void*>(new char[256]);
                    memcpy(loadRequest.m_loadData, path.C_Str(), 256);
                    resourceLoader.AddLoadRequest(loadRequest);

                    shaderMaterial.addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(path.C_Str())), static_cast<Material::TextureSlotMapping::TextureSlot>(Material::TextureSlotMapping::Diffuse0 + counter)));
                }
            }
            for (size_t counter = 0; counter < aimaterial->GetTextureCount(aiTextureType_EMISSIVE) && counter < Material::TextureSlotMapping::Emmisive7; ++counter)
            {
                if (aiReturn_SUCCESS == aimaterial->GetTexture(aiTextureType_EMISSIVE, static_cast<unsigned int>(counter), &path, &uvMapping, &uv_index))
                {
                    LoadRequest loadRequest;
                    loadRequest.m_gameObjectId = 0;
                    loadRequest.m_resourceType = hashString("LOAD_TEXTURE");
                    loadRequest.m_loadData = static_cast<void*>(new char[256]);
                    memcpy(loadRequest.m_loadData, path.C_Str(), 256);
                    resourceLoader.AddLoadRequest(loadRequest);

                    shaderMaterial.addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(path.C_Str())), static_cast<Material::TextureSlotMapping::TextureSlot>(Material::TextureSlotMapping::Emmisive0 + counter)));
                }
            }
            for (size_t counter = 0; counter < aimaterial->GetTextureCount(aiTextureType_SPECULAR) && counter < Material::TextureSlotMapping::Specular7; ++counter)
            {
                if (aiReturn_SUCCESS == aimaterial->GetTexture(aiTextureType_SPECULAR, static_cast<unsigned int>(counter), &path, &uvMapping, &uv_index))
                {
                    LoadRequest loadRequest;
                    loadRequest.m_gameObjectId = 0;
                    loadRequest.m_resourceType = hashString("LOAD_TEXTURE");
                    loadRequest.m_loadData = static_cast<void*>(new char[256]);
                    memcpy(loadRequest.m_loadData, path.C_Str(), 256);
                    resourceLoader.AddLoadRequest(loadRequest);

                    shaderMaterial.addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(path.C_Str())), static_cast<Material::TextureSlotMapping::TextureSlot>(Material::TextureSlotMapping::Specular0 + counter)));
                }
            }

            if (aiReturn_SUCCESS == aimaterial->GetTexture(aiTextureType_NORMALS, 0, &path, &uvMapping, &uv_index))
            {
                LoadRequest loadRequest;
                loadRequest.m_gameObjectId = 0;
                loadRequest.m_resourceType = hashString("LOAD_TEXTURE");
                loadRequest.m_loadData = static_cast<void*>(new char[256]);
                memcpy(loadRequest.m_loadData, path.C_Str(), 256);
                resourceLoader.AddLoadRequest(loadRequest);

                shaderMaterial.addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(path.C_Str())), Material::TextureSlotMapping::Normals));
            }
            MSG_TRACE_CHANNEL("ASSIMP LOADER", "Trying to read material %d", aimaterial);

            shaderMaterial.Prepare(renderResourceHelper.getResource().getEffectCache());

            ModelHelperFunctions::AssignTextureIndices(renderResource, shaderMaterial.getTextureHashes(), resourceIndices);
            ModelHelperFunctions::CreateConstantBuffers(resourceIndices, &group, renderResourceHelper.getWriteableResource());

            ModelHelperFunctions::AssignPerSceneIndices(renderResource, resourceIndices);
        }
    }

    return model;
}

}