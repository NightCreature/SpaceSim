#ifndef SQUARE_H
#define SQUARE_H
#include "vector2.h"

#include "BBox.h"
#include "model.h"

class Camera;
class Matrix44;
//Doesnt need to be  class only generation fucntion is enough
namespace Square
{
    struct SquareCreationParams
    {
        Vector2 m_lowerleft;
        Vector2 m_upperright;
        ShaderInstance* shaderInstance;
        Resource* resource;
    };

    struct CreatedSquare
    {
        Model* model;
        Bbox boundingBox;
    };

    CreatedSquare CreateSquare(const SquareCreationParams& params, const ShaderInstance& shaderInstance);
};
#endif