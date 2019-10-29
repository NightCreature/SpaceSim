#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"

#include <string>
#include <vector>
#include "GraphConnection.h"

namespace NodeGraph
{

class PinBase;
class PinDefinition;

class Node
{
public:
    Node() {}
    ~Node() {}

    virtual void Invoke() = 0;

    size_t GetHash() const { return m_nodeHash; }

    std::string GetNodeName() { return m_name; }

    void AddConnection(const GraphConnection& connection) { m_connections.push_back(connection); }

    void AddInputPin(const PinDefinition& definition);
    void RemoveInputPin(const PinBase* pin);
    void AddOutputPin(const PinDefinition& definition);
    void RemoveOutputPin(const PinBase* pin);

    HASH_ELEMENT_DEFINITION(Node);
protected:
    std::vector<PinBase*> m_inputPins;
    std::vector<PinBase*> m_ouputPins;
    std::vector<GraphConnection> m_connections;

    std::string m_name;

    size_t m_nodeHash;
};

}
