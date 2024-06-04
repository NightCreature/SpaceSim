#pragma once

#include "UI/Rendering/Element.h"

namespace FE::Rendering
{

class ElementRenderObjectFactory;

class SelectElement : public Element
{
public:

	void Update() override;
	void PopulateCommandList(Resource* resource, CommandList& list) override;
	void HandleMessage(Message& msg) override;
private:
	ElementRenderObjectFactory* m_renderableFactory = nullptr;
};

}