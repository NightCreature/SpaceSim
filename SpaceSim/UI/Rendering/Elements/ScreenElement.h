#pragma once

#include "UI/Rendering/Element.h"

namespace FE::Rendering::Elements 
{

//This has to read some data file based on 
class ScreenElement : public Element
{
public:

	void Initialise(const std::string_view& filePathForScreenData) {}

	void Update() override
	{
	}

	void PopulateCommandList(Resource* resource, CommandList& list) override
	{
	}

	void HandleMessage(Message& msg) override
	{
	}

	void Deserialise(tinyxml2::XMLElement& element) override
	{
	}
private:
};

}