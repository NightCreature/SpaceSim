#pragma once

#include "UI/Core/Behaviours/Behaviour.h"
#include "UI/Core/Localisation/LocalisationId.h"

namespace FE
{

namespace Behaviours
{

class StaticText : public Behaviour
{
public:
	StaticText() = default;
	~StaticText() override {}

	void Serialise(const tinyxml2::XMLElement* element) override;
	void Update(float deltaT, const InputState& input) override;
	bool HandleInput(const InputState& state) override;
	const Transition* GetTransitionLink() override;

	void Activate() override;
	void Deactivate() override;

	static Behaviour* Create() { return new StaticText(); }
public:
	LocalisationId m_locId;
};

}

}