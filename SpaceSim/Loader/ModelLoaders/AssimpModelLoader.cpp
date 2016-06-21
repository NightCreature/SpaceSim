#include "Loader/ModelLoaders/AssimpModelLoader.h"
#include "Graphics/mesh.h"
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
    aiMesh* subMesh = scene->mMeshes[0];
    for (size_t vertCounter = 0; vertCounter < subMesh->mNumVertices; ++vertCounter)
    {
        params.m_vertices.push_back(Vector3(subMesh->mVertices[vertCounter].x, subMesh->mVertices[vertCounter].y, subMesh->mVertices[vertCounter].z));

        if (subMesh->HasNormals())
        {
            params.m_normals.push_back(Vector3(-subMesh->mNormals[vertCounter].x, -subMesh->mNormals[vertCounter].y, -subMesh->mNormals[vertCounter].z));
            //MSG_TRACE_CHANNEL("NORMAL DEBUG", "Normal: %f, %f, %f", subMesh->mNormals[vertCounter].x, subMesh->mNormals[vertCounter].y, subMesh->mNormals[vertCounter].z);
        }

        for (unsigned int uvChannel = 0; uvChannel < subMesh->GetNumUVChannels(); ++uvChannel)
        {
            if (subMesh->HasTextureCoords(uvChannel))
            {
                aiVector3D* texCoordChannel = subMesh->mTextureCoords[uvChannel];
                while (params.m_texcoords.size() <= uvChannel)
                {
                    params.m_texcoords.push_back(std::vector<Vector2>());
                }
                switch (subMesh->mNumUVComponents[uvChannel])
                {
                case 1:
                {
                    Vector2 vec(texCoordChannel[vertCounter].x, 0.0f);
                    std::vector<Vector2>& smit = params.m_texcoords[uvChannel];
                    smit.push_back(vec);
                }
                break;
                case 2:
                {
                    Vector2 vec(texCoordChannel[vertCounter].x, texCoordChannel[vertCounter].y);
                    std::vector<Vector2>& smit = params.m_texcoords[uvChannel];
                    smit.push_back(vec);
                }
                break;
                case 3:
                {
                    Vector2 vec(texCoordChannel[vertCounter].x, texCoordChannel[vertCounter].y);
                    std::vector<Vector2>& smit = params.m_texcoords[uvChannel];

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
    CreatedModel mesh = Mesh::CreateMesh(params);
    return mesh.model;
}
