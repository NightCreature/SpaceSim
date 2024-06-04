#pragma once

namespace FE
{

class Transition
{
public:
	void SetTransitionID(const HashString& id) { m_transitionId = id; }
	const HashString& GetTransitionId() const { return m_transitionId; }


	bool IsBack() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}


private:
	HashString m_transitionId;
};

class EmptyTransition : public Transition
{

};

class TransitionBack : public Transition
{

};

}