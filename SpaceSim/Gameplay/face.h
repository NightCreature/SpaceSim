#ifndef FACE_H
#define FACE_H

#include "Graphics/Model.h"
#include <vector>
#include <map>

class Resource;
class Matrix44;
//Doesnt need to be  class only generation fucntion is enough
namespace Face
{

    struct CreationParams //Needs potential modification
    {
        CreationParams() :
            //shaderInstance(nullptr), Should be effect it would like to use, this can be a resource id in the model manger now instead of an actual shader instance
            //resource(nullptr),
            fillvalue(0.0f),
            width(50),
            height(50),
            nrVerticesInX(10),
            nrVerticesInY(10),
            tesselate(false),
            fillx(false),
            filly(false),
            fillz(false),
            changeWindingOrder(false),
            invertNormal(false)
        {
        }

        //const ShaderInstance* shaderInstance;
        //Resource* resource;
        Material::MaterialParameters m_materialParameters; //This way we can create the material on the mesh group
        float fillvalue;
        float width;
        float height;
        size_t nrVerticesInX;
        size_t nrVerticesInY;
        bool tesselate;
        bool fillx;
        bool filly;
        bool fillz;
        bool changeWindingOrder;
        bool invertNormal;
    };

    CreatedModel CreateFace(const CreationParams& params, Resource* resource);

    static unsigned long long m_totalNumberOfVerts;
    static unsigned long long m_totalNumberOfPolygons;

    void createVertexData(const CreationParams& params, byte*& vertexData, Bbox& boundingBox, float corridorHeight, float corridorWidth, size_t rows, size_t columns);
    void createIndexData(unsigned int*& indecis, bool changeWindingOrder, size_t rows, size_t columns);
};
#endif