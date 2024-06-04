#include "Loader/ModelLoaders/XMLModelLoader.h"
#include "Graphics/mesh.h"
#include "Graphics/MeshGroupCreator.h"
#include "Graphics/EffectCache.h"
#include "Graphics/DebugHelperFunctions.h"

XMLModelLoader::XMLModelLoader(void)
{
}


XMLModelLoader::~XMLModelLoader(void)
{
}

///-------------------------------------------------------------------------
// @brief 
// @remark this type should probably carry a data definition
///-------------------------------------------------------------------------
CreatedModel XMLModelLoader::LoadModel(Resource* resource, const LoadData& loadData, CommandList& commandList)
{
    if (loadData.m_fileName.empty())
        return CreatedModel();

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(loadData.m_fileName.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        MSG_TRACE_CHANNEL("XMLMODELLOADER", "Failed to load %s\nWith error: %d", loadData.m_fileName.c_str(), doc.ErrorID() )
        return CreatedModel();
    }

    CreatedModel model;

    tinyxml2::XMLElement* element;
    element = doc.FirstChildElement();

    constexpr auto meshFaceHash = "MeshFace"_hash;
    constexpr auto meshVertexHash = "MeshVertex"_hash;
    constexpr auto meshIndecisHash = "MeshIndices"_hash;

    tinyxml2::XMLElement* declarationElement = doc.FirstChildElement("VertexDeclaration");
    if (declarationElement == nullptr)
    {
        return model;
    }

    VertexDeclarationDescriptor descriptor;
    for (const tinyxml2::XMLAttribute* attribute = declarationElement->FirstAttribute(); attribute != nullptr; attribute->Next())
    {
        if (std::string_view(attribute->Name()) == "positions")
        {
            descriptor.position = attribute->IntValue();
        }
        else if (std::string_view(attribute->Name()) == "normal")
        {
            descriptor.normal = attribute->BoolValue();
        }
        else if (std::string_view(attribute->Name()) == "nrTextureCoords")
        {
            for (size_t index = 0; index < static_cast<size_t>(attribute->IntValue()); ++index)
            {
                descriptor.textureCoordinateDimensions.push_back(2);
            }
        }
    }

    VertexDataStreams dataStreams = CreateDataStreams(descriptor);
    std::vector<Vector3>& positionStream = std::get<2>(dataStreams.m_streams[VertexStreamType::Position]);
    std::vector<Vector3>& normalStream = std::get<2>(dataStreams.m_streams[VertexStreamType::Normal]);
    size_t uvStreamStart = static_cast<std::underlying_type_t<VertexStreamType>>(VertexStreamType::UVStart);

    std::vector<uint32> indices;

    element = element->FirstChildElement();
    for (;element; element = element->NextSiblingElement())
    {
        auto typeHash = Hashing::hashString(element->Value());

        if (meshFaceHash == typeHash)
        {
            for (tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement())
            {
                unsigned int count = 0;
                unsigned int texCoordSetCount = 0;
                typeHash = Hashing::hashString(childElement->Value());
                if( typeHash == meshVertexHash)
                {
                    for (tinyxml2::XMLElement* vertexDataElement = childElement->FirstChildElement(); vertexDataElement; vertexDataElement = vertexDataElement->NextSiblingElement())
                    {
                        typeHash = Hashing::hashString(vertexDataElement->Value());
                        if (Vector3::m_hash == typeHash)
                        {
                            Vector3 vec;
                            vec.deserialise(vertexDataElement);
                            if (count == 0)
                            {
                                
                                //See this as position data
                                positionStream.push_back(vec);
                                //mesh->getBoundingBox().enclose(vec);
                                ++count;
                            }
                            else
                            {
                                
                                //See normal data
                                normalStream.push_back(vec);
                            }
                        }
                        else if (Vector2::m_hash == typeHash)
                        {
                            Vector2 vec;
                            vec.deserialise(vertexDataElement);
                            std::vector<Vector2>& uvStream = std::get<1>(dataStreams.m_streams[static_cast<VertexStreamType>(uvStreamStart + texCoordSetCount)]);
                            uvStream.push_back(vec);
                            ++texCoordSetCount;
                        }
                    }
                }
                else if (meshIndecisHash == typeHash)
                {
                    std::stringstream indecis( childElement->GetText() );
                    while(!indecis.eof())
                    {
                        unsigned int index = 0;
                        char c;
                        indecis >> index >> c;
                        indices.push_back(index);
                    }
                }
            }
        }
    }

    Mesh::normalizeNormals(normalStream); //Avoid read in errors so that all the normals are actually unit vectors

    model.model = new Model();
    MeshGroup& group = model.model->CreateMeshGroup();
    RenderResource& renderResource = RenderResourceHelper(resource).getWriteableResource();
    group.GetVB().CreateBuffer(renderResource.getDeviceManager(), commandList, renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap(), dataStreams);
    group.GetIB().Create(renderResource.getDeviceManager(), commandList, indices.size() * sizeof(uint32), static_cast<void*>(indices.data()));

    
    //if (mesh->getMeshData()[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
    //{
    //    renderResourceHelper helper(resource);
    //    mesh->getMeshData()[0]->getShaderInstance().getMaterial().setEffect(helper.getResource().getEffectCache().getEffect("simple_effect.fx"));
    //}
    //if (mesh->getRenderInstance().getMaterial().getEffect().getVertexShaderBlob() == 0 )
    //{
    //    mesh->getRenderInstance().getMaterial().getEffect().loadEffect("SimpleEffect2.fx", Effect::e5_0);
    //}
    //mesh->initialise(ShaderInstance());

    return model;
}
