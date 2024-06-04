#pragma once

#include "Core/StringOperations/HashString.h"

#include "Graphics/RenderGraph/Pass.h"

#include <vector>

class RenderGraph
{
public:
	template<class Func>
	RenderPassInterface& AddPass(const HashString& name, Func passFunction)
	{
		RenderPassInterface* returnValue = nullptr;
		auto iterator = std::find_if(begin(m_passes), end(m_passes), [name](const RenderPassInterface* pass) { return pass->GetName() == name; });
		if (iterator == end(m_passes))
		{
			returnValue = new RenderPass<Func>(passFunction, name);
			m_passes.push_back(returnValue);
		}
		else
		{
			returnValue = *iterator;
		}

		return *returnValue;
	}


private:
	std::vector<RenderPassInterface*> m_passes;
};

