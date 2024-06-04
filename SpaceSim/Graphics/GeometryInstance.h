#pragma once 

#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Core/Types/TypeHelpers.h"
#include "Logging/LoggingMacros.h"
#include <vector>

//Wrapper to hold a vertex and index buffer reference
class GeometryInstance
{
public:
    GeometryInstance() : m_vb(nullptr), m_ib(nullptr), m_primitiveType(4/*Triangle list*/) {}
    GeometryInstance(VertexBuffer* vb, IndexBuffer* ib, unsigned int primitiveType = 4/*Triangle list*/) : m_vb(vb), m_ib(ib), m_primitiveType(primitiveType) {}
    ~GeometryInstance()
    {
        if (m_ib !=  nullptr)
        {
            delete m_ib;
            m_ib = nullptr;
        }
        if (m_vb != nullptr)
        {
            delete m_vb;
            m_vb = nullptr;
        }
    }

    void cleanup()
    {
        if (m_vb != nullptr)
        {
            MSG_TRACE("Cleaning VB in Geometry Instance");
            delete m_vb;
        }

        if (m_ib != nullptr)
        {
            delete m_ib;
        }
    }

	VertexBuffer* getVB() const { return m_vb; }
    VertexBuffer& getVB() { return *m_vb; }
	void setVB(VertexBuffer* vb) { m_vb = vb; }
	IndexBuffer* getIB() const { return m_ib; }
    IndexBuffer& getIB() { return *m_ib; }
	void setIB(IndexBuffer* ib)  { m_ib = ib; }
    unsigned int getPrimitiveType() const { return m_primitiveType; }
    void setPrimitiveType(unsigned int primitiveType) { m_primitiveType = primitiveType; }
private:
    VertexBuffer* m_vb;
    IndexBuffer* m_ib;
    unsigned int m_primitiveType;
};