#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "Core/tinyxml2.h"
#include "Graphics/material.h"
#include "Graphics/Texture12.h"
#include <d3d11.h>
#include <map>
#include <mutex>
#include <string>
#include "D3D12/DescriptorHeapManager.h"

class DeviceManager;
class Resource;

struct TextureInfo
{
	Texture12 m_texture;
	size_t m_heapIndex;
};

class TextureManager
{
public:
    TextureManager() {}
	~TextureManager();

    void cleanup();
	void Initialise(Resource* resource);
    Material::TextureSlotMapping deserialise( DeviceManager& deviceManager, const tinyxml2::XMLElement* node );
	void setMipMapSettings(const bool canautomipmap, const bool generatemipmaps);

	bool find(const std::string& filename) const;
	void addLoad(DeviceManager& deviceManager, const std::string& filename);
	const TextureInfo* getTexture(const std::string& filename) const;
    const TextureInfo* getTexture(const size_t textureNameHash) const;
	long getTexMemUsed() const;

	bool activateTexture(const std::string& filename);

    bool createSamplerStates(const DeviceManager& deviceManager);
    ID3D11SamplerState* const getSamplerState() const { return m_samplerState; }

    void addTexture( const std::string& textureName, const Texture12& texture, size_t heapIndex);

	DescriptorHeap& GetTextureDescriptorHeap() { return m_textureHeap; }
	DescriptorHeap& GetSamplerDescriptorHeap() { return m_samplerHeap; }
        
protected:
private:
	bool m_generatemipmaps;
	bool m_autogeneratemipmaps;
    typedef std::pair<size_t, TextureInfo> TexturePair;
	typedef std::map<size_t, TextureInfo> TextureMap;
	using Texture12Map = std::map<size_t, Texture12>;
	TextureMap m_textures;	
    ID3D11SamplerState* m_samplerState;

	DescriptorHeap m_textureHeap;
	DescriptorHeap m_samplerHeap;

	std::mutex m_mutex;
};
#endif