
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"
#include "..\SpaceSim\Effect.h"
#include "D3D11.h"
#include "..\SpaceSim\tinyxml2.h"
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>

class DeviceManager;
class TextureManager;
class LightManager;

class Material
{
public:
	Material();
	Material( float shininess, const Color& ambient = Color::darkgray(), const Color& specular = Color::white(), const Color& emissive = Color::black(), const Color& diffuse = Color::white());
    Material(const Material& material);
	~Material()
    {
        //if (m_blendState)
        //{
        //    m_blendState->Release();
        //    m_blendState = 0;
        //}
    }

    void deserialise(Resource* resource, const DeviceManager& deviceManager, const TextureManager& textureManger, const LightManager& lightManager, const tinyxml2::XMLElement* node);

	Color getAmbient() const {return Color(m_materialCB.m_ambient);}
    Color getDiffuse() const { return Color(m_materialCB.m_diffuse); }
    Color getSpecular() const { return Color(m_materialCB.m_specular); }
    Color getEmissive() const { return Color(m_materialCB.m_emissive); }
    float getShininess() const { return m_materialCB.m_shininess; }
    void setAmbient(const Color& ambient) { m_materialCB.m_ambient[0] = ambient.r(); m_materialCB.m_ambient[1] = ambient.g(); m_materialCB.m_ambient[2] = ambient.b(); m_materialCB.m_ambient[3] = ambient.a(); }
    void setDiffuse(const Color& diffuse) { m_materialCB.m_diffuse[0] = diffuse.r(); m_materialCB.m_diffuse[1] = diffuse.g(); m_materialCB.m_diffuse[2] = diffuse.b(); m_materialCB.m_diffuse[3] = diffuse.a(); }
    void setSpecular(const Color& specular) { m_materialCB.m_specular[0] = specular.r(); m_materialCB.m_specular[1] = specular.g(); m_materialCB.m_specular[2] = specular.b(); m_materialCB.m_specular[3] = specular.a(); }
    void setEmissive(const Color& emissive) { m_materialCB.m_emissive[0] = emissive.r(); m_materialCB.m_emissive[1] = emissive.g(); m_materialCB.m_emissive[2] = emissive.b(); m_materialCB.m_emissive[3] = emissive.a(); }
    void setShininess(const float shininess) { m_materialCB.m_shininess = shininess; }
    void setEffect( const Effect* effect) { m_effect = const_cast<Effect*>(effect); }
    void addTextureReference( unsigned int textureHash) { m_texture.push_back(textureHash); }
    const std::vector<unsigned int>& getTextureHashes() const { return m_texture; }

    static const unsigned int m_hash;
    const MaterialContent& getMaterialCB() const { return m_materialCB; }
    void setBlendState(bool alphaBlendEnabled) { m_alphaBlend = alphaBlendEnabled; }
    bool getBlendState() const { return m_alphaBlend; }
    
    const Effect* getEffect() const { return m_effect; }
    Effect* getEffect() { return m_effect; }
protected:
private:
    Effect* m_effect;
    MaterialContent m_materialCB;
    std::vector<unsigned int> m_texture;//Order is sampler order as well
    bool m_alphaBlend;
};
#endif