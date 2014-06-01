#include "XMLModelLoader.h"
#include "mesh.h"
#include "EffectCache.h"

XMLModelLoader::XMLModelLoader(void)
{
}


XMLModelLoader::~XMLModelLoader(void)
{
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
Model* XMLModelLoader::LoadModel( Resource* resource, const std::string& fileName )
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

    Mesh* mesh = new Mesh(resource);
    std::vector<Vector3>& vertices = mesh->getVertices();
    std::vector<Vector3>& normals = mesh->getNormals();
    std::map<int, std::vector<Vector2> >& texCoords = mesh->getTexCoords();
    size_t numberOfTexCoords = 0;
    std::vector<unsigned int>& indices = mesh->getIndices();

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
                                mesh->getBoundingBox().enclose(vec);
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
                            std::map<int, std::vector<Vector2> >::iterator smit = texCoords.find(texCoordSetCount);
                            if (smit != texCoords.end())
                            {
                                smit->second.push_back(vec);
                            }
                            else
                            {
                                texCoords.insert(std::pair<int, std::vector<Vector2> >(texCoordSetCount, std::vector<Vector2>(1, vec)));
                            }
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

    mesh->normalizeNormals(); //Avoid read in errors so that all the normals are actually unit vectors
    //if (mesh->getMeshData()[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
    //{
    //    GameResourceHelper helper(resource);
    //    mesh->getMeshData()[0]->getShaderInstance().getMaterial().setEffect(helper.getGameResource().getEffectCache().getEffect("simple_effect.fx"));
    //}
    //if (mesh->getRenderInstance().getMaterial().getEffect().getVertexShaderBlob() == 0 )
    //{
    //    mesh->getRenderInstance().getMaterial().getEffect().loadEffect("SimpleEffect2.fx", Effect::e5_0);
    //}
    //mesh->initialise(ShaderInstance());

    return mesh;
}
