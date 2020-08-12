#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "Core/tinyxml2.h"
#include "Graphics/material.h"
#include "Graphics/Texture.h"
#include <d3d11.h>
#include <map>
#include <mutex>
#include <string>

class DeviceManager;

class TextureManager
{
public:
    TextureManager() {}
	~TextureManager();

    void cleanup();
    Material::TextureSlotMapping deserialise( const DeviceManager& deviceManager, const tinyxml2::XMLElement* node );
	void setMipMapSettings(const bool canautomipmap, const bool generatemipmaps);

	bool find(const std::string& filename) const;
	void addLoad(const DeviceManager& deviceManager, const std::string& filename);
	const Texture* getTexture(const std::string& filename) const;
    const Texture* getTexture(const size_t textureNameHash) const;
	long getTexMemUsed() const;

	bool activateTexture(const std::string& filename);

    bool createSamplerStates(const DeviceManager& deviceManager);
    ID3D11SamplerState* const getSamplerState() const { return m_samplerState; }

    void addTexture( const std::string& textureName, const Texture& texture );
protected:
private:
	bool m_generatemipmaps;
	bool m_autogeneratemipmaps;
    typedef std::pair<size_t, Texture> TexturePair;
	typedef std::map<size_t, Texture> TextureMap;
	TextureMap m_textures;	
    ID3D11SamplerState* m_samplerState;

	std::mutex m_mutex;
};
#endif