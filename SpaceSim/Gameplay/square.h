#ifndef SQUARE_H
#define SQUARE_H
#include "Math/vector2.h"

#include "Gameplay/BBox.h"
#include "Graphics/model.h"

class Camera;
class Matrix44;
//Doesnt need to be  class only generation fucntion is enough
namespace Square
{
    struct SquareCreationParams : public Model::CreationParams
    {
        CommandList* m_commandList;
        Material mat;
        Vector2 m_lowerleft;
        Vector2 m_upperright;
    };

    CreatedModel CreateSquare(const SquareCreationParams& params);
};
#endif