#pragma once

namespace NodeGraph
{
class PinBase; 

class GraphConnection
{
public:
    GraphConnection(const PinBase* lhs, const PinBase* rhs) : m_lhs(lhs), m_rhs(rhs) {}
    ~GraphConnection() = default;
private:
    const PinBase* m_lhs;
    const PinBase* m_rhs;
};
}