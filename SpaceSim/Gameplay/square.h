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
    struct SquareCreationParams : public Model::CreationParams
    {

        Vector2 m_lowerleft;
        Vector2 m_upperright;
    };

    CreatedModel CreateSquare(const SquareCreationParams& params);
};
#endif