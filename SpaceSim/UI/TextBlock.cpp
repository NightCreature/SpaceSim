#include "TextBlock.h"

#include "..\StringHelperFunctions.h"
#include "..\DebugHelperFunctions.h"
#include "BitmapFont.h"

namespace Text
{

//-----------------------------------------------------------------------------
//! @brief   Maybe this should move to a font cache
//! @remark
//-----------------------------------------------------------------------------
bool TextBlockCache::addFont(const std::string& fileName, Resource* resource)
{
    BitmapFont font;
    font.openFont(fileName, resource); //This is not great because we have to load a font file each time to check if a font is present

    ASSERT_CHANNEL(nullptr == getBitmapFont(font.getFontInfo().m_fontNameHash), "Font Creation", "Font(%s) from file(%s) already exists!", font.getFontInfo().m_fontName.c_str(), fileName.c_str());
    if (nullptr != getBitmapFont(font.getFontInfo().m_fontNameHash))
    {
        return false;
    }

    m_fonts.push_back(font);

    return true;
}

    //-----------------------------------------------------------------------------
//! @brief   Calculates where the text is in the rectangle specified, might scale the text to fit
//! @remark
//-----------------------------------------------------------------------------
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

    return false;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
size_t TextBlockCache::getTextIndex(const std::string& text)
{
    return getTextIndex(hashString(text));
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool TextBlockCache::removeText(const std::string& text)
{
    return removeText(hashString(text));
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void TextBlockCache::removeAllTexts()
{
    std::vector<TextBlockInfo> empty;
    m_textBlocks.swap(empty);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void TextBlockInfo::ProcessText()
{
    float x = m_textBlockSize.x();
    float y = m_textBlockSize.y();
    float maxWidth = m_textBlockSize.w() - m_textBlockSize.y();
    float lineWidth = 0.f;
    float sizeScale = m_size / m_font->getFontInfo().m_fontSize;
    char* lastChar = nullptr;
    int lineNumber = 1;
    int wordNumber = 1;
    float wordWidth = 0.f;
    bool firstCharOfLine = true;
    
    const CommonFontInfo& commonFontInfo = m_font->getCommonFontInfo();

    for (int i = 0; i < m_text.size(); i++)
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
                char* newLineLastChar = nullptr;
                lineWidth = 0.f;
                for (int j = 0; j < m_glyphQuads.size(); j++)
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
                            m_glyphQuads[j].setX(x + (glyphToReset.m_xOffset * sizeScale));
                            m_glyphQuads[j].setY(y + (glyphToReset.m_yOffset * sizeScale));
                            x += glyphToReset.m_xAdvance * sizeScale;
                            lineWidth += glyphToReset.m_xAdvance * sizeScale;
                            if (m_applyKerning)
                            {
                                const KerningInformation& kerningInfo = glyphToReset.getKerningInfoFor(m_glyphQuads[j + 1].m_character);
                                x += kerningInfo.m_ammount * sizeScale;
                                lineWidth += kerningInfo.m_ammount * sizeScale;
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
                            m_glyphQuads[j].setX(x + (glyphToReset.m_xOffset * sizeScale));
                            m_glyphQuads[j].setY(y + (glyphToReset.m_yOffset * sizeScale));
                            x += glyphToReset.m_xAdvance * sizeScale;
                            lineWidth += glyphToReset.m_xAdvance * sizeScale;
                            offset += glyphToReset.m_xAdvance * sizeScale * 0.5f;
                            if (m_applyKerning)
                            {
                                const KerningInformation& kerningInfo = glyphToReset.getKerningInfoFor(m_glyphQuads[j + 1].m_character);
                                x += kerningInfo.m_ammount * sizeScale;
                                lineWidth += kerningInfo.m_ammount * sizeScale;
                                offset += kerningInfo.m_ammount * 0.5f;
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
                            m_glyphQuads[j].setX(x + (glyphToReset.m_xOffset * sizeScale));
                            m_glyphQuads[j].setY(y + (glyphToReset.m_yOffset * sizeScale));
                            x += glyphToReset.m_xAdvance * sizeScale;
                            lineWidth += glyphToReset.m_xAdvance * sizeScale;
                            offset += glyphToReset.m_xAdvance * sizeScale;
                            if (m_applyKerning)
                            {
                                const KerningInformation& kerningInfo = glyphToReset.getKerningInfoFor(m_glyphQuads[j + 1].m_character);
                                x += kerningInfo.m_ammount * sizeScale;
                                lineWidth += kerningInfo.m_ammount * sizeScale;
                                offset += kerningInfo.m_ammount;
                            }
                        }
                    }
                    newLineLastChar = &(m_glyphQuads[j].m_character);
                }

                // Make post-newline justifications
                if (m_alignment == Align::center || m_alignment == Align::right)
                {
                    // Justify the new line
                    for (int k = 0; k < m_glyphQuads.size(); k++)
                    {
                        if (m_glyphQuads[k].m_lineNumber == lineNumber + 1)
                        {
                            m_glyphQuads[k].setX(-offset);
                        }
                    }
                    x -= offset;

                    // Rejustify the line it was moved from
                    for (int k = 0; k < m_glyphQuads.size(); k++)
                    {
                        if (m_glyphQuads[k].m_lineNumber == lineNumber)
                        {
                            m_glyphQuads[k].setX(offset);
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
        if (m_applyKerning && !firstCharOfLine)
        {
            const KerningInformation& kerningInfo = glyph.getKerningInfoFor(m_text[i + 1]);
            if (kerningInfo.m_secondId != -1)
            {
                x += kerningInfo.m_ammount * sizeScale;
                lineWidth += kerningInfo.m_ammount * sizeScale;
                wordWidth += kerningInfo.m_ammount * sizeScale;
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
        q.m_vertices[0] = topLeft;
        q.m_vertices[1] = bottomLeft; 
        q.m_vertices[2] = bottomRight; 
        q.m_vertices[3] = topRight;
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
        lastChar = &(m_text[i]);

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
            for (int j = 0; j < m_glyphQuads.size(); j++)
            {
                if (m_glyphQuads[j].m_lineNumber == lineNumber)
                {
                    m_glyphQuads[j].setX(-offset);
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
            for (int j = 0; j < m_glyphQuads.size(); j++)
            {
                if (m_glyphQuads[j].m_lineNumber == lineNumber)
                {
                    offset = xAdvance;
                    m_glyphQuads[j].setX(-xAdvance);
                }
            }
            x -= offset;
        }
    }
}

}