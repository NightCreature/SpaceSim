#include "AddNode.h"

#include "Core/StringOperations/StringHelperFunctions.h"

///-----------------------------------------------------------------------------
///! @brief This adds the two input pins together and passes the value to the output pin
///! @remark
///-----------------------------------------------------------------------------
void NodeGraph::AddNode::Invoke()
{
    size_t noInputPins = m_inputPins.size();
    if (noInputPins < 2 || noInputPins > 2)
    {
        MSG_TRACE_CHANNEL("Node Graph", "Node %s has too many or tpp few input pins", GetNodeName().c_str());
        return;
    }

    if (m_ouputPins.size() != 1)
    {
        MSG_TRACE_CHANNEL("Node Graph", "Node %s has too many or too few output pins", GetNodeName().c_str());
    }

    //This will not work
	//auto value1 = m_inputPins[0].GetValue();
	//auto value2 = m_inputPins[1].GetValue();
	//m_ouputPins[0].SetValue(value1 + value2);

}
