//#include "ModelLoader.h"
//#include <assimp/Importer.hpp>      // C++ importer interface
//#include <assimp/scene.h>           // Output data structure
//#include <assimp/postprocess.h>     // Post processing flags
//#include <vector>
//#include <iostream>
//
//ModelLoader::ModelLoader(void)
//{
//}
//
//
//ModelLoader::~ModelLoader(void)
//{
//}
//
/////-------------------------------------------------------------------------
//// @brief 
/////-------------------------------------------------------------------------
//void ModelLoader::LoadModel( const std::string& fileName )
//{
//    Assimp::Importer importer;  
//    // And have it read the given file with some example postprocessing  
//    // Usually - if speed is not the most important aspect for you - you'll   
//    // propably to request more postprocessing than we do in this example.  
//    const aiScene* scene = importer.ReadFile( fileName, aiProcess_CalcTangentSpace | aiProcess_PreTransformVertices |
//        aiProcess_Triangulate      | aiProcess_JoinIdenticalVertices |
//        aiProcess_SortByPType      | aiProcess_ConvertToLeftHanded);    // If the import failed, report it  
//    if( !scene)  
//    {    
//        MSG_TRACE_CHANNEL("", "failed to open the model file (" << fileName << ") importer error code: " << importer.GetErrorString() << std::endl;
//    }  // Now we can access the file's contents.   
//
//    //Grab the verts here
//    //m_drawableObject = new Mesh();
//    std::vector<float> vertices;
//    std::vector<float> normals;
//    std::vector<float> uvs;
//    std::vector<unsigned int> indices;
//    unsigned int highestIndex = 0;
//    for (size_t counter = 0; counter < scene->mNumMeshes; ++counter)
//    {
//        //Extract vertices from the mesh here and store in our own vertex buffer
//        aiMesh* mesh = scene->mMeshes[counter];
//        for (size_t vertCounter = 0; vertCounter < mesh->mNumVertices; ++vertCounter)
//        {
//            vertices.push_back(mesh->mVertices[vertCounter].x);
//            vertices.push_back(mesh->mVertices[vertCounter].y);
//            vertices.push_back(mesh->mVertices[vertCounter].z);
//
//            if (mesh->HasNormals())
//            {
//                normals.push_back(mesh->mNormals[vertCounter].x);
//                normals.push_back(mesh->mNormals[vertCounter].y);
//                normals.push_back(mesh->mNormals[vertCounter].y);
//            }
//
//            for (unsigned int uvChannel = 0; uvChannel < mesh->GetNumUVChannels(); ++uvChannel)
//            {
//                if (mesh->HasTextureCoords(uvChannel))
//                {
//                    aiVector3D* texCoordChannel = mesh->mTextureCoords[uvChannel];
//                    switch (mesh->mNumUVComponents[uvChannel])
//                    {
//                    case 1:
//                        uvs.push_back(texCoordChannel[vertCounter].x);
//                        break;
//                    case 2:
//                        uvs.push_back(texCoordChannel[vertCounter].x);
//                        uvs.push_back(texCoordChannel[vertCounter].y);
//                        break;
//                    case 3:
//                        uvs.push_back(texCoordChannel[vertCounter].x);
//                        uvs.push_back(texCoordChannel[vertCounter].y);
//                        uvs.push_back(texCoordChannel[vertCounter].z);
//                        break;
//                    default:
//                        break;
//                    }
//                }
//            }
//        }
//
//        //Need to keep track of highest index and add it to the next batch and so one sadly
//        unsigned int baseIndexOffset = highestIndex;
//        for (size_t indexCounter = 0; indexCounter < mesh->mNumFaces; ++indexCounter)
//        {
//            for (size_t counterIndex = 0; counterIndex < mesh->mFaces[indexCounter].mNumIndices; ++counterIndex)
//            {
//                indices.push_back(mesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset);
//                if (mesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset > highestIndex)
//                {
//                    highestIndex = mesh->mFaces[indexCounter].mIndices[counterIndex] + baseIndexOffset;
//                }
//            }
//        }
//    }
//
//    //Build our vbs
//    size_t bufferSize = vertices.size() * sizeof(float) + normals.size() * sizeof(float) + (vertices.size() / 3) * 2 * sizeof(float);
//    unsigned char* vertexData = new unsigned char[bufferSize];
//    for (unsigned int counter = 0; counter < vertices.size(); counter += 3)
//    {
//        *(float*)vertexData = vertices[counter]; vertexData += sizeof(float);
//        *(float*)vertexData = vertices[counter + 1]; vertexData += sizeof(float);
//        *(float*)vertexData = vertices[counter + 2]; vertexData += sizeof(float);
//        *(float*)vertexData = normals[counter]; vertexData += sizeof(float);
//        *(float*)vertexData = normals[counter + 1]; vertexData += sizeof(float);
//        *(float*)vertexData = normals[counter + 2]; vertexData += sizeof(float);
//        *(float*)vertexData = 0.0f; vertexData += sizeof(float);
//        *(float*)vertexData = 0.0f; vertexData += sizeof(float);
//        //for (unsigned int texCoordCounter = 0; texCoordCounter < (unsigned int)m_nummultitexcoords; ++texCoordCounter)
//        //{
//        //    TexCoords texCoords = texcoords[texCoordCounter];
//        //    *(float*)vertexData = texCoords[counter].x(); vertexData += sizeof(float);
//        //    *(float*)vertexData = texCoords[counter].y(); vertexData += sizeof(float);
//        //}
//
//        //m_drawableObject->getBoundingBox().enclose(Vector3(vertices[counter], vertices[counter + 1], vertices[counter + 2]));
//    }
//    std::vector<unsigned int> texCoordDimensions;
//    //for (unsigned int texCoordCounter = 0; texCoordCounter < (unsigned int)m_nummultitexcoords; ++texCoordCounter)
//    //{
//    texCoordDimensions.push_back(2);
//    //}
//    vertexData = vertexData - bufferSize;
//    //m_drawableObject->getRenderInstance().getVertexBuffer().createBufferAndLayoutElements((unsigned int)bufferSize, vertexData, false, true, true, texCoordDimensions);
//
//    //if (m_drawableObject->getRenderInstance().getMaterial().getEffect().getVertexShaderBlob() == 0 )
//    //{
//    //    m_drawableObject->getRenderInstance().getMaterial().getEffect().loadEffect("SimpleEffect.fx", Effect::e5_0);
//    //}
//
//    //m_drawableObject->getRenderInstance().getVertexBuffer().createVertexInputLayout(m_drawableObject->getRenderInstance().getMaterial().getEffect().getVertexShaderBlob());
//    delete [] vertexData;
//
//    //m_drawableObject->getRenderInstance().getIndexBuffer().setNumberOfIndecis( (unsigned int)indices.size() );
//    //m_drawableObject->getRenderInstance().getIndexBuffer().createBuffer((unsigned int)indices.size() * sizeof(unsigned int), (void*)&indices[0], false, D3D11_BIND_INDEX_BUFFER);
//
//}
