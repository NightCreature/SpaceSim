#include "TextBlock.h"

#include "Memory.h"

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/DebugHelperFunctions.h"
#include "UI/BitmapFont.h"

#include "Graphics/VertexBuffer.h"

#include "Core/Resource/RenderResource.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/ShaderCache.h"
#include "Graphics/EffectCache.h"
#include "Application/BaseApplication.h"

#include "Core/Types/TypeHelpers.h"
#include "Graphics/D3D12/CommandQueue.h"


namespace Text
{

///-----------------------------------------------------------------------------
///! @brief   Maybe this should move to a font cache
///! @remark
///-----------------------------------------------------------------------------
bool TextBlockCache::addFont(const std::string& fileName)
{
    BitmapFont font;
    font.openFont(fileName, m_resource); //This is not great because we have to load a font file each time to check if a font is present

    ASSERT_CHANNEL(nullptr == getBitmapFont(font.getFontInfo().m_fontNameHash), "Font Creation", "Font(%s) from file(%s) already exists!", font.getFontInfo().m_fontName.c_str(), fileName.c_str());
    if (nullptr != getBitmapFont(font.getFontInfo().m_fontNameHash))
    {
        return false;
    }

    m_fonts.push_back(font);

    return true;
}

    ///-----------------------------------------------------------------------------
///! @brief   Calculates where the text is in the rectangle specified, might scale the text to fit
///! @remark
///-----------------------------------------------------------------------------
bool TextBlockCache::addText(const std::string& text, const Vector4& textBox, Align m_alignment, size_t fontHash, float size, bool kerning)
{
    if (m_maxTextBlocks <= m_textBlocks.size())
    {
        return false;
    }

    size_t hash = hashString(text);
    size_t index = getTextIndex(hash);
    if (index < m_textBlocks.size())
    {
        return false; //already exists
    }

    TextBlockInfo info;
    info.m_text = text;
    info.m_textBlockSize = textBox;
    info.m_alignment = m_alignment;
    info.m_textHash = hash;
    info.m_textLenght = text.size();
    info.m_size = size;
    info.m_applyKerning = kerning;
    info.m_font = getBitmapFont(fontHash);
    m_textBlocks.push_back(info);
	if (m_textBlocks[m_textBlocks.size() - 1].ProcessText(m_resource))
	{
		return true;
	}

    return false;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
size_t TextBlockCache::getTextIndex(const std::string& text)
{
    return getTextIndex(hashString(text));
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
size_t TextBlockCache::getTextIndex(size_t textHash)
{
    for (size_t counter = 0; counter < m_textBlocks.size(); ++counter)
    {
        if (m_textBlocks[counter].m_textHash == textHash)
        {
            return counter;
        }
    }
    return static_cast<size_t>(-1);
}


///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
bool TextBlockCache::removeText(const std::string& text)
{
    return removeText(hashString(text));
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
bool TextBlockCache::removeText(size_t text)
{
    size_t counter = 0;
    for (; counter < m_textBlocks.size(); ++counter)
    {
        if (m_textBlocks[counter].m_textHash == text)
        {
            break;
        }
    }

    if (counter > m_textBlocks.size())
    {
        return false;
    }

    m_textBlocks[counter] = m_textBlocks[m_textBlocks.size()];
    m_textBlocks.pop_back();

    return true;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void TextBlockCache::removeAllTexts()
{
    std::vector<TextBlockInfo> empty;
    m_textBlocks.swap(empty);
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void TextBlockCache::ProvideRenderInstances()
{
	//Should only really do this for blocks that are active
	//for (auto& textBlock : m_textBlocks)
	//{
 //       //TODO FIX THIS SHOULD ACCESS CURRENT VIEW MATRIX SOMEHOW
	//	//textBlock.m_shaderInstance.getWVPConstants().m_view = Application::m_view;
	//}

	//std::copy(m_textBlocksToRender.begin(), m_textBlocksToRender.end(), renderInstances.end());
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
BitmapFont* TextBlockCache::getBitmapFont(size_t fontNameHash)
{
    for (BitmapFont& font : m_fonts)
    {
        if (font.getFontInfo().m_fontNameHash == fontNameHash)
        {
            return &font;
        }
    }

    return nullptr;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
bool TextBlockInfo::ProcessText(Resource* resource)
{
    float x = m_textBlockSize.x();
    float y = m_textBlockSize.y();
    float maxWidth = m_textBlockSize.w() - m_textBlockSize.y();
    float lineWidth = 0.f;
	float sizeScale = m_size / (float)m_font->getFontInfo().m_fontSize;
    //char* lastChar = nullptr;
    size_t lineNumber = 1;
    size_t wordNumber = 1;
    float wordWidth = 0.f;
    bool firstCharOfLine = true;
    
    const CommonFontInfo& commonFontInfo = m_font->getCommonFontInfo();

    for (size_t i = 0; i < m_text.size(); i++)
    {
        const Glyph& glyph = m_font->getGlyph(m_text[i]);
        float xOffset = glyph.m_xOffset * sizeScale;
        float yOffset = glyph.m_yOffset * sizeScale;
        float xAdvance = glyph.m_xAdvance * sizeScale;
        float width = glyph.m_width * sizeScale;
        float height = glyph.m_height * sizeScale;

        // Check vertical bounds
        if (y + yOffset + height > m_textBlockSize.z())
        {
            break;
        }

        // Newline
        if (m_text[i] == '\n' || m_text[i] == '\r' || (lineWidth + xAdvance >= maxWidth))
        {
            if (m_alignment == Align::left)
            {
                // Start at left
                x = m_textBlockSize.x();
            }
            if (m_alignment == Align::center)
            {
                // Start in center
                x = m_textBlockSize.x() + (maxWidth * 0.5f);
            }
            else if (m_alignment == Align::right)
            {
                // Start at right
                x = m_textBlockSize.w();
            }

            y += commonFontInfo.m_lineHeight * sizeScale;
            float offset = 0.f;

            if ((lineWidth + xAdvance >= maxWidth) && (wordNumber != 1))
            {
                // Next character extends past text box width
                // We have to move the last word down one line
                //char* newLineLastChar = nullptr;
                lineWidth = 0.f;
                for (size_t j = 0; j < m_glyphQuads.size(); j++)
                {
                    const Glyph& glyphToReset = m_font->getGlyph(static_cast<short>(m_glyphQuads[j].m_character));
                    if (m_alignment == Align::left)
                    {
                        // Move current word to the left side of the text box
                        if ((m_glyphQuads[j].m_lineNumber == lineNumber) &&
                            (m_glyphQuads[j].m_wordNumber == wordNumber))
                        {
                            m_glyphQuads[j].m_lineNumber++;
                            m_glyphQuads[j].m_wordNumber = 1;
                            m_glyphQuads[j].setX(x + (glyphToReset.m_xOffset * sizeScale), *this);
                            m_glyphQuads[j].setY(y + (glyphToReset.m_yOffset * sizeScale), *this);
                            x += glyphToReset.m_xAdvance * sizeScale;
                            lineWidth += glyphToReset.m_xAdvance * sizeScale;
                            if (m_applyKerning && j < m_glyphQuads.size() - 1)
                            {
                                const KerningInformation* kerningInfo = glyphToReset.getKerningInfoFor(m_glyphQuads[j + 1].m_character);
                                if (kerningInfo)
                                {
                                    x += kerningInfo->m_ammount * sizeScale;
                                    lineWidth += kerningInfo->m_ammount * sizeScale;
                                }
                            }
                        }
                    }
                    else if (m_alignment == Align::center)
                    {
                        if ((m_glyphQuads[j].m_lineNumber == lineNumber) &&
                            (m_glyphQuads[j].m_wordNumber == wordNumber))
                        {
                            // First move word down to next line
                            m_glyphQuads[j].m_lineNumber++;
                            m_glyphQuads[j].m_wordNumber = 1;
                            m_glyphQuads[j].setX(x + (glyphToReset.m_xOffset * sizeScale), *this);
                            m_glyphQuads[j].setY(y + (glyphToReset.m_yOffset * sizeScale), *this);
                            x += glyphToReset.m_xAdvance * sizeScale;
                            lineWidth += glyphToReset.m_xAdvance * sizeScale;
                            offset += glyphToReset.m_xAdvance * sizeScale * 0.5f;
                            if (m_applyKerning && j < m_glyphQuads.size() - 1)
                            {
                                const KerningInformation* kerningInfo = glyphToReset.getKerningInfoFor(m_glyphQuads[j + 1].m_character);
                                if (kerningInfo)
                                {
                                    x += kerningInfo->m_ammount * sizeScale;
                                    lineWidth += kerningInfo->m_ammount * sizeScale;
                                    offset += kerningInfo->m_ammount * 0.5f;
                                }
                            }
                        }
                    }
                    else if (m_alignment == Align::right)
                    {
                        if ((m_glyphQuads[j].m_lineNumber == lineNumber) &&
                            (m_glyphQuads[j].m_wordNumber == wordNumber))
                        {
                            // Move character down to next line
                            m_glyphQuads[j].m_lineNumber++;
                            m_glyphQuads[j].m_wordNumber = 1;
                            m_glyphQuads[j].setX(x + (glyphToReset.m_xOffset * sizeScale), *this);
                            m_glyphQuads[j].setY(y + (glyphToReset.m_yOffset * sizeScale), *this);
                            x += glyphToReset.m_xAdvance * sizeScale;
                            lineWidth += glyphToReset.m_xAdvance * sizeScale;
                            offset += glyphToReset.m_xAdvance * sizeScale;
                            if (m_applyKerning && j < m_glyphQuads.size() - 1)
                            {
                                const KerningInformation* kerningInfo = glyphToReset.getKerningInfoFor(m_glyphQuads[j + 1].m_character);
                                if (kerningInfo)
                                {
                                    x += kerningInfo->m_ammount * sizeScale;
                                    lineWidth += kerningInfo->m_ammount * sizeScale;
                                    offset += kerningInfo->m_ammount;
                                }
                            }
                        }
                    }
                    //newLineLastChar = &(m_glyphQuads[j].m_character);
                }

                // Make post-newline justifications
                if (m_alignment == Align::center || m_alignment == Align::right)
                {
                    // Justify the new line
                    for (size_t k = 0; k < m_glyphQuads.size(); k++)
                    {
                        if (m_glyphQuads[k].m_lineNumber == lineNumber + 1)
                        {
                            m_glyphQuads[k].setX(-offset, *this);
                        }
                    }
                    x -= offset;

                    // Rejustify the line it was moved from
                    for (size_t k = 0; k < m_glyphQuads.size(); k++)
                    {
                        if (m_glyphQuads[k].m_lineNumber == lineNumber)
                        {
                            m_glyphQuads[k].setX(offset, *this);
                        }
                    }
                }
            }
            else
            {
                // New line without any "carry-down" word
                firstCharOfLine = true;
                lineWidth = 0.f;
            }

            wordNumber = 1;
            lineNumber++;

        } // End new line check

        // Don't print these
        if (m_text[i] == '\n' || m_text[i] == '\r' || m_text[i] == '\t')
        {
            continue;
        }

        // Set starting cursor for m_alignment
        if (firstCharOfLine)
        {
            if (m_alignment == Align::left)
            {
                // Start at left
                x = m_textBlockSize.x();
            }
            if (m_alignment == Align::center)
            {
                // Start in center
                x = m_textBlockSize.x() + (maxWidth * 0.5f);
            }
            else if (m_alignment == Align::right)
            {
                // Start at right
                x = m_textBlockSize.z();
            }
        }

        // Adjust for kerning
        float kernAmount = 0.f;
        if (m_applyKerning && !firstCharOfLine && i < m_text.size())
        {
            const KerningInformation* kerningInfo = glyph.getKerningInfoFor(m_text[i + 1]);
            if (kerningInfo)
            {
                if (kerningInfo->m_secondId != -1)
                {
                    x += kerningInfo->m_ammount * sizeScale;
                    lineWidth += kerningInfo->m_ammount * sizeScale;
                    wordWidth += kerningInfo->m_ammount * sizeScale;
                }
            }
        }

        firstCharOfLine = false;

        // Create the vertices
        Vector4 topLeft = Vector4(
            x + xOffset, y + yOffset,
            (float)glyph.m_x / (float)commonFontInfo.m_widthScale,
            (float)glyph.m_y / (float)commonFontInfo.m_heightScale);
        Vector4 topRight = Vector4(
            topLeft.x() + width, y + yOffset,
            (float)(glyph.m_x + glyph.m_width) / (float)commonFontInfo.m_widthScale,
            (float)glyph.m_y / (float)commonFontInfo.m_heightScale);
        Vector4 bottomRight = Vector4(
            topLeft.x() + width, topLeft.y() + height,
            (float)(glyph.m_x + glyph.m_width) / (float)commonFontInfo.m_widthScale,
            (float)(glyph.m_y + glyph.m_height) / (float)commonFontInfo.m_heightScale);
        Vector4 bottomLeft = Vector4(
            x + xOffset, topLeft.y() + height,
            (float)glyph.m_x / (float)commonFontInfo.m_widthScale,
            (float)(glyph.m_y + glyph.m_height) / (float)commonFontInfo.m_heightScale);

        // Create the quad
        GlyphQuad q;
		q.m_vertexOffset = m_glyphVerts.size();
		GlyphVertex vertex;
		vertex.position = Vector3(-topLeft.x(), -topLeft.y(), 0.0f);
		vertex.uv = Vector2(topLeft.z(), topLeft.w());
        m_glyphVerts.push_back(vertex);
		vertex.position = Vector3(-bottomLeft.x(), -bottomLeft.y(), 0.0f);
		vertex.uv = Vector2(bottomLeft.z(), bottomLeft.w());
        m_glyphVerts.push_back(vertex);
		vertex.position = Vector3(-bottomRight.x(), -bottomRight.y(), 0.0f);
		vertex.uv = Vector2(bottomRight.z(), bottomRight.w());
        m_glyphVerts.push_back(vertex);
		vertex.position = Vector3(-topRight.x(), -topRight.y(), 0.0f);
		vertex.uv = Vector2(topRight.z(), topRight.w());
        m_glyphVerts.push_back(vertex);
        q.m_lineNumber = lineNumber;
        if (m_text[i] == ' ' && m_alignment == Align::right)
        {
            wordNumber++;
            wordWidth = 0.f;
        }
        q.m_wordNumber = wordNumber;
        wordWidth += xAdvance;
        q.m_wordWidth = wordWidth;
        q.m_bitmapChar = const_cast<Glyph*>(&glyph);
        q.m_sizeScale = sizeScale;
        q.m_character = m_text[i];
        m_glyphQuads.push_back(q);

        if (m_text[i] == ' ' && m_alignment == Align::left)
        {
            wordNumber++;
            wordWidth = 0.f;
        }

        x += xAdvance;
        lineWidth += xAdvance;
        //lastChar = &(m_text[i]);

        // Rejustify m_text
        if (m_alignment == Align::center)
        {
            // We have to recenter all Quads since we addded a 
            // new character
            float offset = xAdvance * 0.5f;
            if (m_applyKerning)
            {
                offset += kernAmount * 0.5f;
            }
            for (size_t j = 0; j < m_glyphQuads.size(); j++)
            {
                if (m_glyphQuads[j].m_lineNumber == lineNumber)
                {
                    m_glyphQuads[j].setX(-offset, *this);
                }
            }
            x -= offset;
        }
        else if (m_alignment == Align::right)
        {
            // We have to rejustify all Quads since we addded a 
            // new character
            float offset = 0.f;
            if (m_applyKerning)
            {
                offset += kernAmount;
            }
            for (size_t j = 0; j < m_glyphQuads.size(); j++)
            {
                if (m_glyphQuads[j].m_lineNumber == lineNumber)
                {
                    offset = xAdvance;
                    m_glyphQuads[j].setX(-xAdvance, *this);
                }
            }
            x -= offset;
        }
    }

	if (m_glyphVerts.empty())
	{
		return false; //Dont try to create a vb for an empty text block
	}

    CreateVertexBuffer(resource);
	CreateShaderSetup(resource);
	
	return true;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void TextBlockInfo::CreateVertexBuffer(Resource* resource)
{
    UNUSEDPARAM(resource);
	//RenderResourceHelper renderResourceHelper(resource);
	//VertexDeclarationDescriptor descriptor;
	//descriptor.position = 3;
	//descriptor.textureCoordinateDimensions.push_back(2);
 //   
 //   auto& commandQueue = renderResourceHelper.getWriteableResource().getCommandQueueManager().GetCommandQueue(renderResourceHelper.getResource().getResourceLoader().GetCommandQueueHandle());
 //   auto& commandList = commandQueue.GetCommandList();
 //   auto& srvCBVUAVHeap = renderResourceHelper.getWriteableResource().getDescriptorHeapManager().GetSRVCBVUAVHeap();

 //   VertexDataStreams dataStreams;
 //   //Fit the Glyph Vertex into the streams needs some rewriting
 //   MSG_ERROR_CHANNEL("TextBlockInfo", "Glyph Vertex has not been transformed yet created an empty buffer for text!");
	//vb.CreateBuffer(renderResourceHelper.getResource().getDeviceManager(), commandList, srvCBVUAVHeap, dataStreams);
	//m_geometryInstance.setPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//unsigned int glyphIndexBufer[] = 
	//{
	//	0, 1, 2,
	//	2, 3, 0
	//};

	//size_t numberIndecis = 6 * (m_glyphVerts.size() / 4);
	//unsigned int* indexBuffer = new unsigned int[numberIndecis];
	//for (size_t counter = 0; counter < m_glyphVerts.size() / 4; ++counter)
	//{
	//	for (size_t indexCounter = 0; indexCounter < 6; ++indexCounter)
	//	{
	//		indexBuffer[counter * 6 + indexCounter] = glyphIndexBufer[indexCounter] + (unsigned int)counter * 4;
	//	}
	//}
	//ib.Create(renderResourceHelper.getResource().getDeviceManager(), commandList, static_cast<unsigned int>(numberIndecis) * sizeof(unsigned int), indexBuffer);
	//ib.setNumberOfIndecis(static_cast<unsigned int>(numberIndecis));

	//m_geometryInstance.setVB(&vb);
	//m_geometryInstance.setIB(&ib);

	//delete [] indexBuffer;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void TextBlockInfo::CreateShaderSetup(Resource* resource)
{
	Material mat;
	RenderResourceHelper gameResource(resource);
    mat.setEffectHash(hashString("sdf_font_effect.xml"));
	mat.setBlendState(true);
	//Should fix this if we have more than one page somehow
	mat.addTextureReference(Material::TextureSlotMapping((unsigned int)hashString(m_font->getPages().m_pages[0].m_fileName.string()), Material::TextureSlotMapping::Diffuse0 ));//Requires we have a texture under the font name
	mat.setDiffuse(Color::yellow());
	mat.setTechnique(hashString("default"));
	//m_shaderInstance.setMaterial(mat);
	//WVPBufferContent& wvpConstants = m_shaderInstance.getWVPConstants();
    //TODO FIX ME SHOULD ASK RENDER SYSTEM FOR VIEW AND PROJECTION
	//wvpConstants.m_projection = Application::m_projection; //math::createOrthoGraphicProjection(1280.0f, 720.0f, 0.1f, 1000.0f);
	//wvpConstants.m_view = Application::m_view; //gameResource.getResource().getCameraManager().getCamera("text_block_camera")->getCamera();
	//wvpConstants.m_world.identity();
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void GlyphQuad::setY(float value, TextBlockInfo& textBlock)
{
	for (size_t counter = 0; counter < 4; ++counter)
	{
		Vector3 temp(0.f, value + (counter == 1 || counter == 2 ? value : 0.f), 0.f);
		textBlock.m_glyphVerts[counter + m_vertexOffset].position += temp;
	}
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void GlyphQuad::setX(float value, TextBlockInfo& textBlock)
{
	for (size_t counter = 0; counter < 4; ++counter)
	{
		Vector3 temp(value + (counter == 1 || counter == 2 ? value : 0.f), 0.f, 0.f);
		textBlock.m_glyphVerts[counter + m_vertexOffset].position += temp;;
	}
}

}