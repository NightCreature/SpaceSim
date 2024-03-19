#pragma once

#include <memory>

namespace FE
{

namespace Rendering
{

using ElementTag = size_t;
class Element;
typedef Element* (*CreateElementFP)();

inline ElementTag CreateUniqueTag()
{
    static ElementTag uniqueTag = 0;
    ElementTag retVal = uniqueTag;
    ++uniqueTag; //Up the type count by one
    return retVal;
}

class ElementType
{
public:
    ElementType() {}
    virtual ~ElementType() {}

    virtual const char* GetId() const = 0;
    virtual const ElementTag& GetElementType() const = 0;
    virtual size_t GetSize() const = 0;

    virtual CreateElementFP GetCreateFP() const = 0;
protected:
};

template<class Element>
class DefaultElementType : public ElementType
{
public:
    DefaultElementType(const char* id, const ElementTag& tag) : m_id(id), m_tag(tag)
    {
        m_instance = this;
    }

    const char* GetId() const override { return m_id; }
    const ElementTag& GetElementType() const override { return m_tag; }
    size_t GetSize() const override { return sizeof(Element); }
    CreateElementFP GetCreateFP() const override { return &Element::Create; }
    std::unique_ptr <Element> GetCreateUP() const override { return std::make_unique<Element>(Element::Create()); }

    static DefaultElementType* m_instance;
private:
    const char* m_id;
    ElementTag m_tag;
};

template<class Element>
DefaultElementType<Element>* DefaultElementType<Element>::m_instance;

}

}