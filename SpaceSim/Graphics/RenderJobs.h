#pragma once

#include "Core/Thread/Job.h"
#include "Core/Types/Types.h"
#include "Graphics/Model.h"
#include "Math/matrix44.h"

#include <string>

struct ThreadContext;

class MeshUpdateJob : public Job
{
public:
    MeshUpdateJob(Resource* resource, Model* model, Matrix44 world, Matrix44 view, Matrix44 projection, std::string name);
private:
};

class CompileShaderJob : public Job
{
public:
    CompileShaderJob(const std::string& filename) : m_fileName(filename) {}

    bool Execute(ThreadContext* context) override;

private:
    std::string m_fileName;
};