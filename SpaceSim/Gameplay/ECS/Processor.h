#pragma once

namespace EntityComponentSystem
{

//Base processor to process entities and their data
class Processor
{
public:
    Processor() {}
    virtual ~Processor() {}

    virtual void Initialise() = 0;
    virtual void Update(float elapsedTime) = 0;
};
}