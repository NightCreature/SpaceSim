#include "Loader/ModelLoaders/AssimpModelLoader.h"
#include "Graphics/mesh.h"
#include "Graphics/texturemanager.h"
#include "Core/Resource/GameResource.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

AssimpModelLoader::AssimpModelLoader(void)
{
}


AssimpModelLoader::~AssimpModelLoader(void)
{
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
Model* AssimpModelLoader::LoadModel(Resource* resource, const ShaderInstance& shaderInstance, const std::string& fileName)
{
    if (fileName.empty())
    {
        return nullptr;
    }
    //Load the model here
    Assimp::Importer importer;  
    // And have it read the given file with some example postprocessing  
    // Usually - if speed is not the most important aspect for you - you'll   
    // propably to request more postprocessing than we do in this example.  aiProcessPreset_TargetRealtime_Quality
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FlipWindingOrder); //Optimize the mesh and scenegraph to reduce drawcalls
    if( !scene)  
    {    
        MSG_TRACE_CHANNEL("ASSIMPMODELLOADER", "failed to open the model file ( %s ) importer error: %s", fileName.c_str(), importer.GetErrorString() )
        return nullptr;
    }  // Now we can access the file's contents.

    //Grab the verts here
    Mesh::CreationParams params;
    params.m_shaderInstance = const_cast<ShaderInstance*>(&shaderInstance);
    params.m_resource = resource;
    unsigned int highestIndex = 0;
    //Extract vertices from the mesh here and store in our own vertex buffer
    for (size_t meshCounter = 0; meshCounter < scene->mNumMeshes; ++meshCounter)
    {
        aiMesh* subMesh = scene->mMeshes[meshCounter];
        for (size_t vertCounter = 0; vertCounter < subMesh->mNumVertices; ++vertCounter)
        {
            params.m_vertices.push_back(Vector3(subMesh->mVertices[vertCounter].x, subMesh->mVertices[vertCounter].y, subMesh->mVertices[vertCounter].z));

            if (subMesh->HasNormals())
            {
                params.m_normals.push_back(Vector3(-subMesh->mNormals[vertCounter].x, -subMesh->mNormals[vertCounter].y, -subMesh->mNormals[vertCounter].z));
                //MSG_TRACE_CHANNEL("NORMAL DEBUG", "Normal: %f, %f, %f", subMesh->mNormals[vertCounter].x, subMesh->mNormals[vertCounter].y, subMesh->mNormals[vertCounter].z);
            }
            if (subMesh->mTangents != nullptr)
            {
                params.m_tangents.push_back(Vector3(-subMesh->mTangents[vertCounter].x, -subMesh->mTangents[vertCounter].y, -subMesh->mTangents[vertCounter].z));
            }
            if (subMesh->mBitangents != nullptr)
            {
                params.m_tangents.push_back(Vector3(-subMesh->mBitangents[vertCounter].x, -subMesh->mBitangents[vertCounter].y, -subMesh->mBitangents[vertCounter].z));
            }

            for (unsigned int uvChannel = 0; uvChannel < subMesh->GetNumUVChannels(); ++uvChannel)
            {
                if (subMesh->HasTextureCoords(uvChannel))
                {
                    aiVector3D* texCoordChannel = subMesh->mTextureCoords[uvChannel];
                    while (params.m_texcoords.size() <= uvChannel)
                    {
                        params.m_texcoords.push_back(std::vector<Vector3>());
                    }
                    switch (subMesh->mNumUVComponents[uvChannel])
                    {
                    case 1:
                    {
                        Vector3 vec(texCoordChannel[vertCounter].x, 0.0f, 0.0f);
                        std::vector<Vector3>& smit = params.m_texcoords[uvChannel];
                        smit.push_back(vec);
                    }
                    break;
                    case 2:
                    {
                        Vector3 vec(texCoordChannel[vertCounter].x, texCoordChannel[vertCounter].y, 0.0f);
                        std::vector<Vector3>& smit = params.m_texcoords[uvChannel];
                        smit.push_back(vec);
                    }
                    break;
                    case 3:
                    {
                        Vector3 vec(texCoordChannel[vertCounter].x, texCoordChannel[vertCounter].y, texCoordChannel[vertCounter].z);
                        std::vector<Vector3>& smit = params.m_texcoords[uvChannel];

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
                params.m_indices.push_back(subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset);
                if (subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset > highestIndex)
                {
                    highestIndex = subMesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset;
                }
            }
        }

        aiMaterial* material = scene->mMaterials[subMesh->mMaterialIndex];
        Material shaderMaterial = params.m_shaderInstance->getMaterial();
        aiColor4D color;
        material->Get(AI_MATKEY_COLOR_AMBIENT, color);
        shaderMaterial.setAmbient(Color(color.r, color.g, color.b, color.a));

        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        shaderMaterial.setDiffuse(Color(color.r, color.g, color.b, color.a));

        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        shaderMaterial.setSpecular(Color(color.r, color.g, color.b, color.a));

        material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        shaderMaterial.setEmissive(Color(color.r, color.g, color.b, color.a));
        
        float shininess;
        material->Get(AI_MATKEY_SHININESS, shininess);
        shaderMaterial.setShininess(shininess);

        //load the texture maps here
        GameResourceHelper gameResource(resource);
        TextureManager& tm = gameResource.getWritableGameResource().getTextureManager();
        DeviceManager& dm = gameResource.getWritableGameResource().getDeviceManager();
        aiString path;
        if (aiReturn_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &path))
        {
            tm.addLoad(dm, path.C_Str());
            shaderMaterial.addTextureReference(hashString(getTextureNameFromFileName(path.C_Str())));
        }
        if (aiReturn_SUCCESS == material->GetTexture(aiTextureType_EMISSIVE, 0, &path))
        {
            tm.addLoad(dm, path.C_Str());
            shaderMaterial.addTextureReference(hashString(getTextureNameFromFileName(path.C_Str())));
        }
        if (aiReturn_SUCCESS == material->GetTexture(aiTextureType_NORMALS, 0, &path))
        {
            tm.addLoad(dm, path.C_Str());
            shaderMaterial.addTextureReference(hashString(getTextureNameFromFileName(path.C_Str())));
        }
        MSG_TRACE_CHANNEL("ASSIMP LOADER","Trying to read material %d", material);
        
    }

    params.m_numtangents = 0; //TODO FIX this needs to look at the model to get this.
    params.m_vertexDeclaration.position = 3;
    params.m_vertexDeclaration.normal = params.m_normals.size() > 0;
    params.m_vertexDeclaration.tangent = params.m_tangents.size() > 0;
    params.m_vertexDeclaration.textureCoordinateDimensions.clear();
    for (size_t counter = 0; counter < params.m_texcoords.size(); ++counter)
    {
        params.m_vertexDeclaration.textureCoordinateDimensions.push_back(2);
    }
    params.m_vertexDeclaration.vertexColor = false;

    CreatedModel mesh = Mesh::CreateMesh(params); //Probably should create a mesh group instead of a mesh
    return mesh.model;
}
