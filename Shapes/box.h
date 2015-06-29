#ifndef BOX_H
#define BOX_H

#include "model.h"
#include "Vector3.h"
#include "face.h"

//Doesnt need to be  class only generation fucntion is enough
namespace Box
{
    struct CreationParams
    {
        ShaderInstance* shaderInstance;
        Resource* resource;
        Vector3		m_lowerleft;
        Vector3		m_upperright;
        bool        m_dynamic;
        bool		m_gentexcoords;
    };


    CreatedModel CreateBox(const CreationParams& params);
};
#endif