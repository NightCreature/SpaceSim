#pragma once

#include <vector>

class Bbox;

class PhysicsManager
{
public:
    PhysicsManager() {}
    ~PhysicsManager() {}
    void AddColidableBbox(Bbox* bbox);

private:
    std::vector<Bbox*> m_staticBoundingBoxes;
};