#pragma once

namespace FE
{

namespace States
{

using StateTag = size_t;
class State;
typedef State* (*CreateStateFP)(Resource* resource);

inline StateTag CreateUniqueTag()
{
    static StateTag uniqueTag = 0;
    StateTag retVal = uniqueTag;
    ++uniqueTag; //Up the type count by one
    return retVal;
}

class StateType
{
public:
    StateType() {}
    virtual ~StateType() {}

    virtual const char* GetId() const = 0;
    virtual const StateTag& GetStateType() const = 0;
    virtual size_t GetSize() const = 0;

    virtual CreateStateFP GetCreateFP() const = 0;
protected:
};

template<class State>
class DefaultStateType : public StateType
{
public:
    DefaultStateType(const char* id, const StateTag& tag) : m_id(id), m_tag(tag)
    {
        m_instance = this;
    }

    const char* GetId() const override { return m_id; }
    const StateTag& GetStateType() const override { return m_tag; }
    size_t GetSize() const override { return sizeof(State); }
    CreateStateFP GetCreateFP() const override { return &State::Create; }

    static DefaultStateType* m_instance;
private:
    const char* m_id;
    StateTag m_tag;
};

template<class State>
DefaultStateType<State>* DefaultStateType<State>::m_instance;

}

}