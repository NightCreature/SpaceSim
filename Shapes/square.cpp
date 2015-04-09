#include "square.h"
#include "vector4.h"
#include "matrix44.h"
#include "..\\SpaceSim\BaseApplication.h"

namespace Square
{

//-----------------------------------------------------------------------------
//! @brief   Creates a square model
//! @remark
//-----------------------------------------------------------------------------
CreatedSquare CreateSquare(const SquareCreationParams& params)
{
    CreatedSquare square;
    GameResource& gameResource = *(GameResource*)params.resource;
    square.boundingBox = Bbox(Vector3(params.m_lowerleft.x(), params.m_lowerleft.y(), 0.0f), Vector3(params.m_upperright.x(), params.m_upperright.y(), 0.0f));

    square.model = new Model();

    if (square.model->getMeshData().empty())
    {
        unsigned int bufferSize = 4 * (3 + 2) * sizeof(float);
        byte vertexData[4 * (3 + 2) * sizeof(float)]; //4 points with position and 1 2D textureCoordinates
        byte* data = vertexData;
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_lowerleft.y(); data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 1.0f; data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = params.m_upperright.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 1.0f; data += sizeof(float);
        *(float*)data = 1.0f; data += sizeof(float);
        *(float*)data = params.m_lowerleft.x(); data += sizeof(float);
        *(float*)data = params.m_upperright.y(); data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 1.0f; data += sizeof(float);
        std::vector<unsigned int> texCoordDim;
        texCoordDim.push_back(2);

        VertexBuffer* vb = new VertexBuffer();
        const Technique* technique = square.model->getMeshData()[0]->getShaderInstance().getMaterial().getEffect()->getTechnique("default");
        VertexDecalartionDesctriptor vertexDesc;
        vertexDesc.textureCoordinateDimensions = texCoordDim;
        const VertexShader* shader = gameResource.getShaderCache().getVertexShader(technique->getVertexShader());
        assert(shader);
        vb->createBufferAndLayoutElements(gameResource.getDeviceManager(), bufferSize, (void*)vertexData, false, vertexDesc, shader->getShaderBlob());
        square.model->getMeshData().push_back(new MeshGroup(vb, 0, *(params.shaderInstance)));
        if (square.model->getMeshData()[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        {
            square.model->getMeshData()[0]->getShaderInstance().getMaterial().setEffect(gameResource.getEffectCache().getEffect("simple_effect.fx"));
        }
        data = nullptr;
    }
    else
    {
        square.model->getMeshData()[0]->setShaderInstance(*(params.shaderInstance));
        if (square.model->getMeshData()[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        {
            square.model->getMeshData()[0]->getShaderInstance().getMaterial().setEffect(gameResource.getEffectCache().getEffect("simple_effect.fx"));
        }
    }

    square.model->getMeshData()[0]->getGeometryInstance().setPrimitiveType((unsigned int)D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return square;
}

}
