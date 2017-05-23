
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Graphics/Color.h"
#include "Graphics/Effect.h"
#include "D3D11.h"
#include "Core/tinyxml2.h"
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

    struct TextureSlotMapping
    {
        //Might need to add which uv channel to use for which slot
        enum TextureSlot
        {
            Ambient0,
            Ambient1,
            Ambient2,
            Ambient3,
            Ambient4,
            Ambient5,
            Ambient6,
            Ambient7,
            Diffuse0,
            Diffuse1,
            Diffuse2,
            Diffuse3,
            Diffuse4,
            Diffuse5,
            Diffuse6,
            Diffuse7,
            Specular0,
            Specular1,
            Specular2,
            Specular3,
            Specular4,
            Specular5,
            Specular6,
            Specular7,
            Emmisive0,
            Emmisive1,
            Emmisive2,
            Emmisive3,
            Emmisive4,
            Emmisive5,
            Emmisive6,
            Emmisive7,
            Normals,
            ReflectionMap,
            ShadowMap,

            NumSlots,
            Invalid = -1
        };

        static_assert(NumSlots < 128, "Defining more than 128 input texture slots, DX11 doesn't support this");

        TextureSlotMapping(unsigned int textureHash, TextureSlot textureSlot) : m_textureHash(textureHash), m_textureSlot(textureSlot) {}

        unsigned int m_textureHash;
        TextureSlot m_textureSlot;
    };

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
    void setMaterialContent(const MaterialContent& materialContent) { m_materialCB = materialContent; }
    void setEffect( const Effect* effect) { m_effect = const_cast<Effect*>(effect); }
    void addTextureReference( const TextureSlotMapping& textureSlot) { m_texture.push_back(textureSlot); }
    const std::vector<TextureSlotMapping>& getTextureHashes() const { return m_texture; }

    static const unsigned int m_hash;
    const MaterialContent& getMaterialCB() const { return m_materialCB; }
    void setBlendState(bool alphaBlendEnabled) { m_alphaBlend = alphaBlendEnabled; }
    bool getBlendState() const { return m_alphaBlend; }
    
    const Effect* getEffect() const { return m_effect; }
    Effect* getEffect() { return m_effect; }
	unsigned int getTechnique() const { return m_techniqueHash; }
	void setTechnique(unsigned int techniqueHash) { m_techniqueHash = techniqueHash; }

    struct MaterialParameters
    {
        MaterialParameters() : m_alphaBlend(false)
        {
            memset(m_textureNames, 0, sizeof(char) * Material::TextureSlotMapping::NumSlots * 256);
        }

        char m_textureNames[Material::TextureSlotMapping::NumSlots][256];
        //std::vector< TextureData > m_texture;
        MaterialContent m_materialContent;
        unsigned int m_effectHash;
        unsigned int m_techniqueHash;
        bool m_alphaBlend;
    };

    static MaterialParameters GetMaterialParameters(const tinyxml2::XMLElement* childElement);
protected:
private:
    unsigned int m_techniqueHash;
    Effect* m_effect; //Could be a resource Id now seeing that the model and shaders only live on the render side, would make the material a sharable state object :)
    MaterialContent m_materialCB;
    std::vector<TextureSlotMapping> m_texture;//Order is sampler order as well
    bool m_alphaBlend;
};
#endif