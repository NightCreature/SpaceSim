#pragma once 

#include "Core/Types/Types.h"
#include "Core/StringOperations/HashString.h"
#include "Math/VectorTypes.h"

#include <D3D12.h>
#include <variant>

enum class ResourceType : size_t
{
	Buffer,
	Texture,
	None
};

struct BufferResourceProprties
{
	size_t m_size; //Bytes
	uint32 m_usageFlags = 0;
};

struct TextureResourceProprties
{
	Vector3s m_dimensions; //Width, Height, Depth
	DXGI_FORMAT m_format = DXGI_FORMAT_B8G8R8A8_UNORM;
	uint32 m_samples = 1;
	uint32 m_levels = 1;
	uint32 m_layers = 1;
	uint32 m_imageFlags = 0;
};

struct RenderResourceProperties
{
	RenderResourceProperties(const std::string_view& name) : m_name(name) {}

	template<class T>
	void SetData(const T& data)
	{
		m_data = data;
		m_type = static_cast<ResourceType>(m_data.index());
	}

	//The list of types here has to be in the same order as the enum values
	std::variant<BufferResourceProprties, TextureResourceProprties> m_data;
	HashString m_name;
	ResourceType m_type = ResourceType::None;
};