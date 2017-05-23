#include "Loader/ModelLoaders/XMLModelLoader.h"
#include "Graphics/mesh.h"
#include "Graphics/MeshGroupCreator.h"
#include "Graphics/EffectCache.h"

XMLModelLoader::XMLModelLoader(void)
{
}


XMLModelLoader::~XMLModelLoader(void)
{
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
Model* XMLModelLoader::LoadModel(Resource* resource, const std::string& fileName)
{
    if (fileName.empty())
        return nullptr;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        MSG_TRACE_CHANNEL("XMLMODELLOADER", "Failed to load %s\nWith error: %d", fileName.c_str(), doc.ErrorID() )
        return nullptr;
    }

    tinyxml2::XMLElement* element;
    element = doc.FirstChildElement();

    unsigned int meshFaceHash = hashString("MeshFace");
    unsigned int meshVertexHash = hashString("MeshVertex");
    unsigned int meshIndecisHash = hashString("MeshIndices");

    MeshGroupCreator::CreationParams params;
    params.m_resource = resource;
    params.m_shaderInstance = shaderInstance;

    //Mesh* mesh = new Mesh(resource);
    std::vector<Vector3>& vertices = params.m_vertices;
    std::vector<Vector3>& normals = params.m_normals;
    std::vector< std::vector<Vector3> >& texCoords = params.m_texcoords;
    size_t numberOfTexCoords = 0;
    std::vector<unsigned int>& indices = params.m_indices;

    element = element->FirstChildElement();
    for (;element; element = element->NextSiblingElement())
    {
        unsigned int typeHash = hashString(element->Value());
        if (meshFaceHash == typeHash)
        {
            for (tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement())
            {
                unsigned int count = 0;
                unsigned int texCoordSetCount = 0;
                typeHash = hashString(childElement->Value());
                if( typeHash == meshVertexHash)
                {
                    for (tinyxml2::XMLElement* vertexDataElement = childElement->FirstChildElement(); vertexDataElement; vertexDataElement = vertexDataElement->NextSiblingElement())
                    {
                        typeHash = hashString(vertexDataElement->Value());
                        if (Vector3::m_hash == typeHash)
                        {
                            Vector3 vec;
                            vec.deserialise(vertexDataElement);
                            if (count == 0)
                            {
                                //See this as position data
                                vertices.push_back(vec);
                                //mesh->getBoundingBox().enclose(vec);
                                ++count;
                            }
                            else
                            {
                                //See normal data
                                normals.push_back(vec);
                            }
                        }
                        else if (Vector2::m_hash == typeHash)
                        {
                            Vector2 vec;
                            vec.deserialise(vertexDataElement);
                            //Always see this as texture coordinate data
                            while (params.m_texcoords.size() <= texCoordSetCount)
                            {
                                params.m_texcoords.push_back(std::vector<Vector3>());
                            }
                            std::vector<Vector3>& smit = texCoords[texCoordSetCount];
                            smit.push_back(Vector3(vec));
                            ++texCoordSetCount;
                        }
                    }
                    if (numberOfTexCoords == 0)
                    {
                        numberOfTexCoords = 1;
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

    Mesh::normalizeNormals(params.m_normals); //Avoid read in errors so that all the normals are actually unit vectors
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

    CreatedMeshGroup mesh = MeshGroupCreator::CreateMeshGroup(params);

    Mesh::CreationParams meshParams;
    meshParams.m_meshGroups.push_back(mesh);
    CreatedModel model = Mesh::CreateMesh(meshParams);
    return model.model;
}
