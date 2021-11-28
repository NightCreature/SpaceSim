#include "Graphics/texturemanager.h"

#include "Core/Resource/RenderResource.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/D3DDebugHelperFunctions.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/texture.h"

#include "assert.h"

TextureManager::~TextureManager()
{
}

void TextureManager::setMipMapSettings(bool canautomipmap, bool generatemipmaps)
{
	m_autogeneratemipmaps = canautomipmap;
	m_generatemipmaps = generatemipmaps;
}

bool TextureManager::find(const std::string& filename) const //Should be texture name without path here
{
    auto textureFileNameHash = hashString(filename);
	if (m_textures.find(textureFileNameHash) != m_textures.end())
		return true;
	return false;
}

const TextureInfo* TextureManager::getTexture(const std::string& filename) const
{
    auto textureFileNameHash = hashString(filename);
    return getTexture(textureFileNameHash);
}

const TextureInfo* TextureManager::getTexture(const size_t textureNameHash) const
{    
    TextureMap::const_iterator tmit = m_textures.find(textureNameHash);
    if (tmit != m_textures.end())
        return &(tmit->second);
    return nullptr;

}

//This might need to connect to the resource loader and just do a request here passing in all the data to the request
void TextureManager::addLoad(DeviceManager& deviceManager, const std::string& filename)
{
	assert (!filename.empty());
    assert(false);//shouldnt be calling this

    //Extract filename if file name contains a path as well, this is not always true need to deal with relative paths here too
    std::string texureName = getTextureNameFromFileName(filename);

	if (find(texureName))
	{
		//MSG_TRACE_CHANNEL("", "Texture: " << filename << " was already loaded. Skipping the loading of the second instance of this texture" << std::endl;
        return;
	}

	MSG_TRACE_CHANNEL("TEXTUREMANAGER", "Attempting to read in texture: %s", filename.c_str());
	Texture12 tex;
    if (!tex.loadTextureFromFile(deviceManager, filename))// , commandQeueuHandle, commandListHandle, handle))
	{
        MSG_TRACE_CHANNEL("ERROR", "Texture cannot be loaded: %s", filename.c_str());
    }

    auto textureFileNameHash = hashString(texureName);
    TextureInfo texInfo;
    texInfo.m_texture = tex;
    //texInfo.m_heapIndex = index; //Need to fix this this is how we can get the GPU and CPU handles again
	m_textures.insert(std::make_pair(textureFileNameHash, texInfo));
}

bool TextureManager::activateTexture(const std::string& filename)
{
	if (find(filename))
	{
		return true;
	}

	return false;
}

long TextureManager::getTexMemUsed() const
{
	long result = 0;
	TextureMap::const_iterator tmit = m_textures.begin();
	while (tmit != m_textures.end())
	{
		tmit++;
	}
	return result;
}
///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void TextureManager::cleanup()
{
    static int numberOfTimesCalled = 0;
    ++numberOfTimesCalled;
    TextureMap::iterator tmit = m_textures.begin();
    while (tmit != m_textures.end())
    { 
        tmit->second.m_texture.cleanup();
        ++tmit;
    }

    m_textures.clear();

    if (m_samplerState)
    {
        m_samplerState->Release();
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void TextureManager::Initialise(Resource* resource)
{
    auto writableResource = RenderResourceHelper(resource).getWriteableResource();
    auto& descriptorHeapManager = writableResource.getDescriptorHeapManager();
    m_textureHeap = descriptorHeapManager.CreateDescriptorHeap(256, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, true);
    m_samplerHeap = descriptorHeapManager.CreateDescriptorHeap(256, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, true);
}

///-------------------------------------------------------------------------
// @Create needed sampler states for the textures here
///-------------------------------------------------------------------------
bool TextureManager::createSamplerStates(const DeviceManager& deviceManager)
{
    UNUSEDPARAM(deviceManager);
    ////Setup sampler state here
    //D3D11_SAMPLER_DESC samplerStateDesc;
    //samplerStateDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    //samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    //samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    //samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    //samplerStateDesc.MipLODBias = 0.0f;
    //samplerStateDesc.MaxAnisotropy = 16;
    //samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    //samplerStateDesc.BorderColor[0] = 0.0f;
    //samplerStateDesc.BorderColor[1] = 0.0f;
    //samplerStateDesc.BorderColor[2] = 0.0f;
    //samplerStateDesc.BorderColor[3] = 0.0f;
    //samplerStateDesc.MinLOD = -3.402823466e+38F;
    //samplerStateDesc.MaxLOD = 3.402823466e+38F;
    //ID3D11Device* device = deviceManager.getDevice();
    //HRESULT hr = device->CreateSamplerState(&samplerStateDesc, &m_samplerState);
    //if (FAILED( hr ) )
    //{
    //    MSG_TRACE_CHANNEL("TEXTUREMANAGER", "Failed to create sampler state: 0x%x", hr )
    //    return false;
    //}

    //D3DDebugHelperFunctions::SetDebugChildName(m_samplerState, "TextureManger Sampler State");

    return true;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
Material::TextureSlotMapping TextureManager::deserialise( DeviceManager& deviceManager, const tinyxml2::XMLElement* node )
{
    (void*)node;
    const char* fileName = node->Attribute("file_name");
    if (fileName)
    {
        addLoad(deviceManager, fileName);
        Material::TextureSlotMapping::TextureSlot textureSlot = Material::TextureSlotMapping::Diffuse0;
        const tinyxml2::XMLAttribute* attribute = node->FindAttribute("texture_slot");
        if (attribute != nullptr)
        {
            textureSlot = static_cast<Material::TextureSlotMapping::TextureSlot>(attribute->UnsignedValue());
        }
        return Material::TextureSlotMapping(hashString(getTextureNameFromFileName(fileName)), textureSlot);
    }

    return Material::TextureSlotMapping(hashString(""), Material::TextureSlotMapping::Invalid);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void TextureManager::addTexture( const std::string& textureName, const Texture12& texture, size_t heapIndex )
{
    std::scoped_lock<std::mutex> lock(m_mutex);
    auto textureNameHash = hashString(textureName);
    if (!find(textureName))
    {
        TextureInfo info;
        info.m_texture = texture;
        info.m_heapIndex = heapIndex;
        m_textures.insert(std::make_pair(textureNameHash, info));
    }
}
