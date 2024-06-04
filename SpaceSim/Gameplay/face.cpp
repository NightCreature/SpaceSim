#include "Gameplay/face.h"
#include "Math/matrixmath.h"
#include "Core/Settings/settingsparser.h"
#include "Graphics/DeviceManager.h"
#include <math.h>
#include <assert.h>
#include <Windows.h>
#include <directxmath.h>

#include "Application/BaseApplication.h" //Hack for now
#include "Core/Types/TypeHelpers.h" //Hack for now
#include "Graphics/D3D12/DescriptorHeapManager.h"
#include "Graphics/MeshGroupCreator.h"
#include "Graphics/ShaderInstance.h"
#include "Core/StringOperations/StringHelperFunctions.h"

#include "Loader/ResourceLoadRequests.h"
#include <Graphics/Model/MeshHelperFunctions.h>
#include "Loader/ResourceLoadJobs.h"

#include "Core/Profiler/ProfilerMacros.h"


namespace Face
{

const size_t numberOfTexcoords = 1;


///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
CreatedModel CreateFace(const CreationParams& params, Resource* resource, Job* currentJob, CommandList& commandList)
{ 
    PROFILE_FUNCTION();
    UNUSEDPARAM(currentJob);

    RenderResourceHelper renderResourceHelper(resource);
    const RenderResource& renderResource = renderResourceHelper.getResource();
    //const ShaderInstance& shaderInstance = *(params.shaderInstance);

    CreatedModel face;
    face.model = new Model();

    float corridorwidth = 50.0f;
    float corridorheight = 50.0f;

    auto& group = face.model->CreateMeshGroup();
    group.SetPrimitiveLayout((unsigned int)D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    VertexBuffer& vb = group.GetVB();
    IndexBuffer& ib = group.GetIB();




    size_t rows = params.nrVerticesInX;
    size_t columns = params.nrVerticesInY;
    //if ((params.height / corridorheight) > 1 && params.tesselate)
    //{
    //    rows *= (int)(params.height / corridorheight);
    //}
    //if ((params.width / corridorwidth) > 1 && params.tesselate)
    //{
    //    columns *= (int)(params.width / corridorheight);
    //}
    size_t numberOfVerts = rows * columns;
    m_totalNumberOfVerts += numberOfVerts;

    VertexDataStreams dataStreams = createVertexData(params, face.boundingBox, corridorheight, corridorwidth, rows, columns);

    MeshResourceIndices& resourceIndices = group.GetResourceInices();
    if (!dataStreams.m_streams.empty())
    {
        resourceIndices = vb.CreateBuffer(renderResource.getDeviceManager(), commandList, renderResourceHelper.getWriteableResource().getDescriptorHeapManager().GetSRVCBVUAVHeap(), dataStreams);
    }
    else
    {
        //Print message
    }

    size_t numberOfIndecis = (rows - 1) * (columns - 1) * 6;
    ib.setNumberOfIndecis(static_cast<unsigned int>(numberOfIndecis));
    unsigned int* indecis = new unsigned int[numberOfIndecis];
    unsigned int* startOfIndexData = indecis;
    createIndexData(indecis, params.changeWindingOrder, rows, columns);

    ib.Create(renderResource.getDeviceManager(), commandList, static_cast<unsigned int>(numberOfIndecis) * sizeof(unsigned int), (void*)startOfIndexData);
    delete[] startOfIndexData; //cleanup
    indecis = nullptr;
    startOfIndexData = nullptr;
    m_totalNumberOfPolygons += numberOfIndecis / 3;

    //Setup material
    Material& mat = group.GetMaterial();
    mat.setEffectHash(params.m_materialParameters.m_effectHash);
    mat.setMaterialContent(params.m_materialParameters.m_materialContent);
    mat.setBlendState(params.m_materialParameters.m_alphaBlend);
    mat.setTechnique(params.m_materialParameters.m_techniqueHash);
    mat.setEffectHash(params.m_materialParameters.m_effectHash);
    //Fix up texture references here
    for (auto counter = 0; counter < Material::TextureSlotMapping::NumSlots; ++counter)
    {
        if (!strICmp(params.m_materialParameters.m_textureNames[counter], ""))
        {
            mat.addTextureReference(Material::TextureSlotMapping(Hashing::hashString(getTextureNameFromFileName(params.m_materialParameters.m_textureNames[counter])), static_cast<Material::TextureSlotMapping::TextureSlot>(counter)));
            //LoadRequest loadRequest;
            //loadRequest.m_gameObjectId = 0;
            //loadRequest.m_resourceType = hashString("LOAD_TEXTURE");
            //loadRequest.m_loadData = static_cast<void*>(new char[256]);
            //memcpy(loadRequest.m_loadData, params.m_materialParameters.m_textureNames[counter], 256);
            //renderResourceHelper.getWriteableResource().getResourceLoader().AddLoadRequest(loadRequest);
            //LoadTextureJob* textureJob = new LoadTextureJob(resource, 0, 0, params.m_materialParameters.m_textureNames[counter]);

            //This might need to be a function
            std::string inputTextureName = params.m_materialParameters.m_textureNames[counter];
            auto loadTextureLambda = [inputTextureName](ThreadContext* context)
            {
                PROFILE_TAG("Load Texture lambda");

                if (inputTextureName.empty())
                    return true;

                RenderResource renderResource = RenderResourceHelper(context->m_renderResource).getWriteableResource();

                CommandList commandList;
                CommandQueue* commandQueue = nullptr;
                bool retVal = renderResource.getResourceLoader().GetCommandListHandleForThreadIndex(context->m_threadIndex, commandList, commandQueue);
                if (retVal)
                {
                    //Extract filename if file name contains a path as well, this is not always true need to deal with relative paths here too
                    std::string textureName = getTextureNameFromFileName(inputTextureName);

                    TextureManager& texManager = renderResource.getTextureManager();

                    auto info = texManager.AddOrCreateTexture(textureName);
                    if (info.m_heapIndex == DescriptorHeap::invalidDescriptorIndex && info.m_loadRequested == false && !info.m_texture.IsValid())
                    {
                        info.m_loadRequested = true;
                        Texture12 texture;
                        size_t descriptorIndex = DescriptorHeap::invalidDescriptorIndex;
                        if (!texture.loadTextureFromFile(renderResource.getDeviceManager(), commandList, *commandQueue, inputTextureName, renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap().GetCPUDescriptorHandle(descriptorIndex)))
                        {
                            MSG_TRACE_CHANNEL("ERROR", "Texture cannot be loaded: %s on thread: %d", inputTextureName.c_str(), context->m_threadIndex);
                            return true;
                        }

                        texManager.addTexture(textureName, texture, descriptorIndex);
                    } 

                    renderResource.getResourceLoader().ReturnCommandListForThreadIndex(context->m_threadIndex);
                }

                return retVal;
            };

            renderResourceHelper.getWriteableResource().getJobQueue().AddFunctionJob(loadTextureLambda);
        }
    }
    mat.Prepare(renderResourceHelper.getResource().getEffectCache());
    
    //we dont need this anymore this should be replaced with setting up the indices for the constatns
    //Dit werkt niet want textures zijn nog niet geladen
    ModelHelperFunctions::AssignTextureIndices(renderResource, mat.getTextureHashes(), resourceIndices);
    ModelHelperFunctions::CreateConstantBuffers(resourceIndices, &group, renderResourceHelper.getWriteableResource());

    ModelHelperFunctions::AssignPerSceneIndices(renderResource, resourceIndices);

    group.SetName(params.m_name);
    face.model->CalculateSortKey(renderResource.getEffectCache());
    return face;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
VertexDataStreams createVertexData(const CreationParams& params, Bbox& boundingBox, float corridorHeight, float corridorWidth, size_t rows, size_t columns)
{
    PROFILE_FUNCTION();

    VertexDeclarationDescriptor vertexDesc;
    vertexDesc.position = 3;
    vertexDesc.normal = true;
    vertexDesc.tangent = true;
    std::vector<unsigned int> texCoordDimensions;
    for (size_t texCoordCounter = 0; texCoordCounter < numberOfTexcoords; ++texCoordCounter)
    {
        texCoordDimensions.push_back(2);
    }
    vertexDesc.textureCoordinateDimensions = texCoordDimensions;

    VertexDataStreams dataStreams = CreateDataStreams(vertexDesc);

    if (0 == rows || 0 == columns)
        return dataStreams;
    float heightpart = params.height / ((float)rows - 1);
    if ((params.height / corridorHeight) > 1 && params.tesselate)
    {
        heightpart = params.height / ((float)rows - 1);
    }
    float widthpart = params.width / ((float)columns - 1);
    if ((params.width / corridorWidth) > 1 && params.tesselate)
    {
        widthpart = params.width / ((float)columns - 1);
    }
    Vector3 v;
    Vector3 normal;
    if (params.fillx)
    {
        normal = Vector3::xAxis();
    }
    else if (params.filly)
    {
        normal = Vector3::yAxis();
    }
    else if (params.fillz)
    {
        normal = Vector3::zAxis();
    }

    Vector3 tangent;
    if (params.fillx)
    {
        tangent = Vector3::zAxis();
    }
    else if (params.filly)
    {
        tangent = Vector3::xAxis();
    }
    else if (params.fillz)
    {
        tangent = Vector3::yAxis();
    }
    if (params.invertNormal)
    {
        normal = -normal;
        tangent = -tangent;
    }

    std::vector<Vector3>& positions = std::get<2>(dataStreams.m_streams[VertexStreamType::Position]);
    std::vector<Vector3>& normals = std::get<2>(dataStreams.m_streams[VertexStreamType::Normal]);
    std::vector<Vector3>& tangents = std::get<2>(dataStreams.m_streams[VertexStreamType::Tangent]);
    size_t uvStart = static_cast<std::underlying_type_t<VertexStreamType>>(VertexStreamType::UVStart);
    Vector2 texcoord;
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            if (params.fillx)
                v = Vector3(params.fillvalue, i * heightpart, j * widthpart);
            if (params.filly)
                v = Vector3(i * heightpart, params.fillvalue, j * widthpart);
            if (params.fillz)
                v = Vector3(i * heightpart, j * widthpart, params.fillvalue);
            positions.push_back(v);
            boundingBox.enclose(v);

            normals.push_back(normal);
            tangents.push_back(tangent);

            float uPart = 1 / (float)(columns - 1); //Minus one as index start at zero still want one at maxEle - 1
            float vPart = 1 / (float)(rows - 1);
            if ((params.width / corridorWidth) > 1 && params.tesselate)
                uPart = uPart * (params.width / corridorWidth);
            if ((params.height / corridorHeight) > 1 && params.tesselate)
                vPart = vPart * (params.height / corridorHeight);
            texcoord = Vector2(j * uPart, i * vPart);
            for (size_t k = 0; k < numberOfTexcoords; k++)
            {
                std::vector<Vector2>& uvStream = std::get<1>(dataStreams.m_streams[static_cast<VertexStreamType>(uvStart + k)]);
                uvStream.push_back(Vector2(1.0f - texcoord.x(), 1.0f - texcoord.y()));
            }
        }
    }
    return dataStreams;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void createIndexData(unsigned int*& indecis, bool changeWindingOrder, size_t rows, size_t columns)
{
    PROFILE_FUNCTION();
    unsigned int nrcolumns = static_cast<unsigned int>(columns);
    for (unsigned int i = 0; i < static_cast<unsigned int>(rows - 1); i++)
    {
        for (unsigned int j = 0; j < nrcolumns - 1; j++)
        {
            unsigned int indexvertex = (i * nrcolumns) + j;
            if (changeWindingOrder)
            {
                *indecis = indexvertex + nrcolumns + 1; ++indecis;
                *indecis = indexvertex + nrcolumns; ++indecis;
                *indecis = indexvertex; ++indecis;

                *indecis = indexvertex + 1; ++indecis;
                *indecis = indexvertex + nrcolumns + 1; ++indecis;
                *indecis = indexvertex; ++indecis;
            }
            else
            {
                *indecis = indexvertex; ++indecis;
                *indecis = indexvertex + nrcolumns; ++indecis;
                *indecis = indexvertex + nrcolumns + 1; ++indecis;

                *indecis = indexvertex; ++indecis;
                *indecis = indexvertex + nrcolumns + 1; ++indecis;
                *indecis = indexvertex + 1; ++indecis;
            }
        }
    }
}

}