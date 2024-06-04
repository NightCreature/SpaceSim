#pragma once

#include "Core/Thread/Job.h"

#include <vector>

class Task
{
public:
    Task();
    ~Task();

    void AddJob(const Job* job);
private:

    std::vector<Job*> m_jobs;
};

