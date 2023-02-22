#include "Graphics/ShaderPack.h"
#include "Core/Resource/GameResource.h"
#include "Graphics/EffectCache.h"
#include "Core/Paths.h"
#include "RenderJobs.h"
#include "Core/FileSystem/FileSystem.h"

///-------------------------------------------------------------------------
// @brief This needs to be multi threaded the new compiler is really slow
///-------------------------------------------------------------------------
bool ShaderPack::loadShaderPack( std::string shaderPack )
{
    OPTICK_EVENT();
    RenderResourceHelper helper(m_resource);
    const Paths* paths = helper.getResource().m_paths;
    EffectCache& effectCache = helper.getWriteableResource().getEffectCache();

    tinyxml2::XMLDocument doc;
    auto fileName = paths->getEffectShaderPath() / shaderPack;
    if (doc.LoadFile(fileName.string().c_str()) != tinyxml2::XML_NO_ERROR)
    {
        MSG_TRACE_CHANNEL("ShaderPack Error", "Failed to load %s\nWith error: %d", fileName.c_str(), doc.ErrorID() )
        return false;
    }

    tinyxml2::XMLElement* shaderPackElement = doc.FirstChildElement()->FirstChildElement("ShaderPack");
    if (shaderPackElement == nullptr)
    {
        return false;
    }
    

    auto& jobQueue = helper.getWriteableResource().getJobQueue();

    for (const tinyxml2::XMLElement* effectElement = shaderPackElement->FirstChildElement( "Effect" ); effectElement != nullptr; effectElement = effectElement->NextSiblingElement())
    {
        const tinyxml2::XMLAttribute* fileNameAttr = effectElement->FindAttribute("file_name");
        if (fileNameAttr != nullptr)
        {
            //MSG_TRACE_CHANNEL("ShaderPack", "Loading Effect: %s", fileNameAttr->Value());
            //This should be a job
            effectCache.createEffect(m_resource, fileNameAttr->Value());
            //jobQueue.AddJob(new CompileShaderJob(m_resource, fileNameAttr->Value()));
        }
    }

    return true;
}
