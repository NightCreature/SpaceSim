#include "TexturePackLoader.h"

#include "Core/Resource/GameResource.h"
#include "Core/Thread/Job.h"

#include "Graphics/RenderJobs.h"

#include "Loader/ResourceLoadJobs.h"
#include "Loader/ResourceLoader.h"
#include "Loader/ResourceLoadRequests.h"
#include <memory>
#include "Core/Profiler/ProfilerMacros.h"

void TexturePackLoader::LoadPack(const std::filesystem::path& filePath, Resource* resource)
{
	PROFILE_FUNCTION();
	RenderResourceHelper helper(resource);
	const Paths* paths = helper.getResource().m_paths;

	tinyxml2::XMLDocument doc;
	auto fileName = paths->getEffectShaderPath() / filePath;
	if (doc.LoadFile(fileName.string().c_str()) != tinyxml2::XML_NO_ERROR)
	{
		MSG_TRACE_CHANNEL("Texture Pack Loader Error", "Failed to load %s\nWith error: %d", fileName.c_str(), doc.ErrorID());
		return;
	}

	tinyxml2::XMLElement* texturePackElement = doc.FirstChildElement()->FirstChildElement("TexturePack");
	if (texturePackElement == nullptr)
	{
		return;
	}

	
	std::vector<std::filesystem::path> textureList;
	for (const tinyxml2::XMLElement* textureElement = texturePackElement->FirstChildElement("Texture"); textureElement != nullptr; textureElement = textureElement->NextSiblingElement())
	{
		const tinyxml2::XMLAttribute* fileNameAttr = textureElement->FindAttribute("file_name");
		if (fileNameAttr != nullptr && fileNameAttr->Value() != nullptr)
		{
			//Need to do something specific here, load texture here, might need to make a job that processes a list of textures in one go
			textureList.push_back(fileNameAttr->Value());
		}
	}

    LoadRequest loadRequest(textureList);
    loadRequest.m_resourceType = "LoadTextureListRequest"_hash;
	ResourceLoader& loader = helper.getWriteableResource().getResourceLoader();
	loader.AddLoadRequest(std::move(loadRequest));
}


