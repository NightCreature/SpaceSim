#include "Loader/ResourceLoadJobs.h"

#include "Core/Resource/RenderResource.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/texture.h"
#include "Graphics/texturemanager.h"

void LoadTextureJob::Execute(size_t threadIndex)
{
    if (m_fileName.empty())
        return;

    RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();

    //Extract filename if file name contains a path as well, this is not always true need to deal with relative paths here too
    std::string textureName = getTextureNameFromFileName(m_fileName);

    TextureManager& texManager = renderResource.getTextureManager();
    if (texManager.find(textureName))
    {
        return;
    }

    Texture texture;
    if (!texture.loadTextureFromFile(renderResource.getDeviceManager(), m_fileName))
    {
        MSG_TRACE_CHANNEL("ERROR", "Texture cannot be loaded: %s on thread: %d", m_fileName.c_str(), threadIndex);
        return;
    }

    texManager.addTexture(textureName, texture);
}
