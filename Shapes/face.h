#ifndef FACE_H
#define FACE_H

#include "Model.h"
#include <vector>
#include <map>

class Resource;
class Matrix44;
//Doesnt need to be  class only generation fucntion is enough
namespace Face
{
    struct CreatedFace
    {
        Model* model;
        Bbox boundingBox;
    };

    struct CreationParams
    {
        ShaderInstance* shaderInstance;
        Resource* resource;
        float fillvalue;
        bool fillx;
        bool filly;
        bool fillz;
        bool changeWindingOrder;
        bool invertNormal;
    };

    CreatedFace CreateFace(const CreationParams& params);

    static unsigned long long m_totalNumberOfVerts;
    static unsigned long long m_totalNumberOfPolygons;

    void createVertexData( bool fillx, float fillvalue, bool filly, bool fillz, byte*& vertexData, bool invertNormal, Bbox& boundingBox );
    void createIndexData( unsigned int*& indecis, bool changeWindingOrder );
};
#endif