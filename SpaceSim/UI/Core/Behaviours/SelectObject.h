#pragma once

#include "Core/StringOperations/HashString.h"
#include "UI/Core/Behaviours/Behaviour.h"
#include "Core/Types/TypeHelpers.h"

namespace FE::Behaviours
{

class SelectObject : public Behaviour
{
public:
	SelectObject() = default;
	~SelectObject() override {}

	void Serialise(const tinyxml2::XMLElement* element) override;
	void Update(float deltaT, const InputState& input) override 
	{
		UNUSEDPARAM(deltaT);
		UNUSEDPARAM(input);
	}
	bool HandleInput(const InputState& state) override { UNUSEDPARAM(state); return false; }
	const Transition* GetTransitionLink() override { return nullptr; }

	void Activate() override;
	void Deactivate() override;

	static Behaviour* Create() { return new SelectObject(); }
private:
	HashString m_object;
};

}