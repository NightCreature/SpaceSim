#pragma once

#include <core/tinyxml2.h>
#include <filesystem>

class Resource;

class TexturePackLoader
{
public:
	void LoadPack(const std::filesystem::path& filePath, Resource* resource);
};