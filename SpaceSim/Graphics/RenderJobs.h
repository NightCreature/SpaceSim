#pragma 

#include "Core/Thread/Job.h"
#include "Core/Types/Types.h"
#include "Graphics/Model.h"
#include "Math/matrix44.h"

#include <string>


class MeshUpdateJob : public Job
{
public:
    MeshUpdateJob(Resource* resource, Model* model, RenderInstanceTree renderInstances, Matrix44 world, Matrix44 view, Matrix44 projection, std::string name);
private:
};

class RenderJob : public Job
{
public:
    RenderJob(Resource* resource) : m_resource(resource) {}

protected:
    Resource* m_resource;
};

class CompileShaderJob : public RenderJob
{
public:
    CompileShaderJob(Resource* resource, const std::string& filename) : RenderJob(resource), m_fileName(filename) {}

    void Execute(size_t threadIndex) override;

private:
    std::string m_fileName;
};