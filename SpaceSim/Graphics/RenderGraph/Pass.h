#pragma once

#include "Graphics/RenderGraph/ResourceDescriptor.h"
#include "Core/StringOperations/HashString.h"

#include <d3d12.h>
#include <string>
#include <variant>
#include <vector>

using RenderResourcePropertiesArray = std::vector<RenderResourceProperties>;

struct PassResources 
{
    RenderResourcePropertiesArray m_passInputs;
    RenderResourcePropertiesArray m_passOutputs;
};

struct PassParameters
{};

class RenderPassInterface
{
public:
	RenderPassInterface(const HashString& name) : m_name(name) {}

	virtual void Execute() = 0;

	void AddInput(const RenderResourceProperties& inputResource)
	{
		if (!Contains(m_passResources.m_passInputs, inputResource))
		{
			m_passResources.m_passInputs.push_back(inputResource);
		}
	}
	void AddOuput(const RenderResourceProperties& outputResource)
	{
		if (!Contains(m_passResources.m_passOutputs, outputResource))
		{
			m_passResources.m_passOutputs.push_back(outputResource);
		}
	}

	const RenderResourcePropertiesArray& GetInputs() const { return m_passResources.m_passInputs; }
	const RenderResourcePropertiesArray& GetOutputs() const { return m_passResources.m_passOutputs; }

	const HashString& GetName() const { return m_name; }

	bool operator==(size_t nameHash) { return m_name == nameHash; }
	bool operator==(const HashString& nameHash) { return m_name == nameHash; }
protected:
	template<class Container, class Element>
	bool Contains(const Container& container, const Element& element) const
	{
		const auto& iterator = std::find_if(cbegin(container), cend(container), [name = element.m_name](const Element& resourceProperties) {return name == resourceProperties.m_name; });
		return iterator != cend(container);
	}

	PassResources m_passResources;
	HashString m_name;
	size_t m_flags;
	bool m_isCompute; //Could be in flags
};

template<class Function>
class RenderPass :public RenderPassInterface
{
public:
    RenderPass(const Function& func, const HashString& name) : RenderPassInterface(name), m_function(func) {}

	void Execute() override { m_function(m_passResources); }
private:
	Function m_function;
};
