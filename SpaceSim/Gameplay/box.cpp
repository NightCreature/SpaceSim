#include "Gameplay/box.h"
#include "Graphics/texturemanager.h"
#include "windows.h"
#include "Application/BaseApplication.h"
#include "Graphics/EffectCache.h"
#include "Core/Types/Types.h"
#include "Core/Types/TypeHelpers.h"

namespace Box
{
///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
CreatedModel CreateBox(const CreationParams& params)
{
    CreatedModel box;

    RenderResource& renderResource = *(RenderResource*)params.resource;
    Material material = params.material;

    box.model = new Model();
    if (box.model->getMeshData().empty())
    {
        VertexBuffer* vb = new VertexBuffer();
        IndexBuffer* ib = new IndexBuffer();

        box.model->getMeshData().push_back(new MeshGroup(vb, ib, material, renderResource.getDeviceManager()));

        std::vector<unsigned int> texCoordDim;
        if (params.m_gentexcoords)
        {
            texCoordDim.push_back(2);
        }

        unsigned int numberOfBytes = 6 * 4 * (3 + (params.m_gentexcoords ? 2 : 0)) * sizeof(float);
        char* data = new char[numberOfBytes]; //4 points with position and two 2D textureCoordinates * 6 faces for a box
        char* startOfData = data;
        //Front face
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        //Back face
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        //Left face
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        //Right face
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        //	//Top face
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        //Bottom face
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = params.m_upperright.z(); data += sizeof(float);
        if (params.m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }

        VertexDeclarationDescriptor vertexDesc;
        vertexDesc.textureCoordinateDimensions = texCoordDim;

        EffectCache& effectCache = renderResource.getEffectCache();
        const Effect* effect = effectCache.getEffect(material.getEffectHash());
        const Technique* technique = effect->getTechnique(material.getTechnique());
        const VertexShader* shader = renderResource.getShaderCache().getVertexShader(technique->getVertexShader()); //Technique pointer is 0!
        assert(shader);
        vb->createBufferAndLayoutElements(renderResource.getDeviceManager(), numberOfBytes, (void*)startOfData, params.m_dynamic, vertexDesc, shader->getShaderBlob());
        delete[] startOfData;
        data = nullptr;
        startOfData = nullptr;
        unsigned int indexData[] =
        {
            0, 1, 2, 2, 3, 0, //FF
            4, 5, 6, 6, 7, 4, //BF
            8, 9, 10, 10, 11, 8, //SF1
            12, 13, 14, 14, 15, 12, //SF2
            16, 17, 18, 18, 19, 16, //VF1
            20, 21, 22, 22, 23, 20 //VF2
        };

        ib->createBuffer(renderResource.getDeviceManager(), sizeof(indexData), (void*)&indexData[0], false);
        ib->setNumberOfIndecis(36);
    }
    else
    {
        //Should probably update the material here althoguh that is not how this should be used to begin with
    }

    box.model->getMeshData()[0]->getGeometryInstance().setPrimitiveType((unsigned int)D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return box;
}

}