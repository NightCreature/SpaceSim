#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include "NodeGraph/GraphNode.h"


namespace NodeGraph
{

class AddNode : public Node
{
public:
    AddNode() = default;
    ~AddNode() = default;

    virtual void Invoke() override;

    HASH_ELEMENT_DEFINITION(AddNode);
private:
};
}