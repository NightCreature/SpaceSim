#pragma once

namespace FE
{

namespace Behaviours
{

using BehaviourTag = size_t;
class Behaviour;
typedef Behaviour* (*CreateBehaviourFP)();

inline BehaviourTag CreateUniqueTag()
{
    static BehaviourTag uniqueTag = 0;
    BehaviourTag retVal = uniqueTag;
    ++uniqueTag; //Up the type count by one
    return retVal;
}

class BehaviourType
{
public:
    BehaviourType() {}
    virtual ~BehaviourType() {}

    virtual const char* GetId() const = 0;
    virtual const BehaviourTag& GetBehaviourType() const = 0;
    virtual size_t GetSize() const = 0;

    virtual CreateBehaviourFP GetCreateFP() const = 0;
protected:
};

template<class Behaviour>
class DefaultBehaviourType : public BehaviourType
{
public:
    DefaultBehaviourType(const char* id, const BehaviourTag& tag) : m_id(id), m_tag(tag)
    {
        m_instance = this;
    }

    const char* GetId() const override { return m_id; }
    const BehaviourTag& GetBehaviourType() const override { return m_tag; }
    size_t GetSize() const override { return sizeof(Behaviour); }
    CreateBehaviourFP GetCreateFP() const override { return &Behaviour::Create; }

    static DefaultBehaviourType* m_instance;
private:
    const char* m_id;
    BehaviourTag m_tag;
};

template<class Behaviour>
DefaultBehaviourType<Behaviour>* DefaultBehaviourType<Behaviour>::m_instance;

}

}