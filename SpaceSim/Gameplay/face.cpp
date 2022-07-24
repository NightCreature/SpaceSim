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


namespace Face
{

const size_t numberOfTexcoords = 1;

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
CreatedModel CreateFace(const CreationParams& params, Resource* resource)
{ 
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

    std::vector<unsigned int> texCoordDimensions;
    for (int texCoordCounter = 0; texCoordCounter < numberOfTexcoords; ++texCoordCounter)
    {
        texCoordDimensions.push_back(2);
    }

    size_t bufferSize = 0;
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
    bufferSize += sizeof(float) * 3 * numberOfVerts;
    bufferSize += sizeof(float) * 3 * numberOfVerts; //Normal
    bufferSize += sizeof(float) * 3 * numberOfVerts; //Tangent
    bufferSize += sizeof(float) * 2 * numberOfTexcoords * numberOfVerts;
    byte* vertexData = new byte[bufferSize];
    byte* startOfVertexArray = vertexData;
    createVertexData(params, vertexData, face.boundingBox, corridorheight, corridorwidth, rows, columns);

    //Move pointer to start of vertex array  

    VertexDeclarationDescriptor vertexDesc;
    vertexDesc.position = 3;
    vertexDesc.normal = true;
    vertexDesc.tangent = true;
    vertexDesc.textureCoordinateDimensions = texCoordDimensions;

    if (params.m_commandList != nullptr)
    {
        vb.Create(renderResource.getDeviceManager(), *params.m_commandList, bufferSize, startOfVertexArray, vertexDesc.GetVertexStride());
    }
    else
    {
        //Print message
    }

    delete[] startOfVertexArray; //cleanup
    vertexData = nullptr;
    startOfVertexArray = nullptr;

    size_t numberOfIndecis = (rows - 1) * (columns - 1) * 6;
    ib.setNumberOfIndecis(static_cast<unsigned int>(numberOfIndecis));
    unsigned int* indecis = new unsigned int[numberOfIndecis];
    unsigned int* startOfIndexData = indecis;
    createIndexData(indecis, params.changeWindingOrder, rows, columns);

    ib.Create(renderResource.getDeviceManager(), *params.m_commandList, static_cast<unsigned int>(numberOfIndecis) * sizeof(unsigned int), (void*)startOfIndexData);
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
            mat.addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(params.m_materialParameters.m_textureNames[counter])), static_cast<Material::TextureSlotMapping::TextureSlot>(counter)));
            LoadRequest loadRequest;
            loadRequest.m_gameObjectId = 0;
            loadRequest.m_resourceType = hashString("LOAD_TEXTURE");
            loadRequest.m_loadData = static_cast<void*>(new char[256]);
            memcpy(loadRequest.m_loadData, params.m_materialParameters.m_textureNames[counter], 256);
            renderResourceHelper.getWriteableResource().getResourceLoader().AddLoadRequest(loadRequest);
        }
    }
    mat.Prepare(renderResourceHelper.getResource().getEffectCache(), renderResourceHelper.getWriteableResource().getDeviceManager(), renderResourceHelper.getWriteableResource().getDescriptorHeapManager().GetSRVCBVUAVHeap());

    return face;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Face::createVertexData(const CreationParams& params, byte*& vertexData, Bbox& boundingBox, float corridorHeight, float corridorWidth, size_t rows, size_t columns)
{
    if (0 == rows || 0 == columns)
        return;
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


    Vector2 texcoord;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (params.fillx)
                v = Vector3(params.fillvalue, i * heightpart, j * widthpart);
            if (params.filly)
                v = Vector3(i * heightpart, params.fillvalue, j * widthpart);
            if (params.fillz)
                v = Vector3(i * heightpart, j * widthpart, params.fillvalue);
            *(float*)vertexData = v.x(); vertexData += sizeof(float);
            *(float*)vertexData = v.y(); vertexData += sizeof(float);
            *(float*)vertexData = v.z(); vertexData += sizeof(float);

            boundingBox.enclose(v);

            *(float*)vertexData = normal.x(); vertexData += sizeof(float);
            *(float*)vertexData = normal.y(); vertexData += sizeof(float);
            *(float*)vertexData = normal.z(); vertexData += sizeof(float);

            *(float*)vertexData = tangent.x(); vertexData += sizeof(float);
            *(float*)vertexData = tangent.y(); vertexData += sizeof(float);
            *(float*)vertexData = tangent.z(); vertexData += sizeof(float);

            float uPart = 1 / (float)(columns - 1); //Minus one as index start at zero still want one at maxEle - 1
            float vPart = 1 / (float)(rows - 1);
            if ((params.width / corridorWidth) > 1 && params.tesselate)
                uPart = uPart * (params.width / corridorWidth);
            if ((params.height / corridorHeight) > 1 && params.tesselate)
                vPart = vPart * (params.height / corridorHeight);
            texcoord = Vector2(j * uPart, i * vPart);
            for (int k = 0; k < numberOfTexcoords; k++)
            {
                *(float*)vertexData = 1.0f - texcoord.x(); vertexData += sizeof(float);
                *(float*)vertexData = 1.0f - texcoord.y(); vertexData += sizeof(float);
            }
        }
    }
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Face::createIndexData(unsigned int*& indecis, bool changeWindingOrder, size_t rows, size_t columns)
{
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