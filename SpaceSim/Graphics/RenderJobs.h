#pragma 

#include "Core/Thread/Job.h"
#include "Core/Types/Types.h"
#include "Graphics/Model.h"
#include "Math/matrix44.h"

#include <string>


class MeshUpdateJob : public Job
{
public:
    MeshUpdateJob(Resource* resource, Model* model, RenderInstanceTree renderInstances, Matrix44 world, Matrix44 view, Matrix44 projection, std::string name)
private:
};