#include "TextBlock.h"

#include "..\StringHelperFunctions.h"
#include "BitmapFont.h"

namespace Text
{

//-----------------------------------------------------------------------------
//! @brief   Calculates where the text is in the rectangle specified, might scale the text to fit
//! @remark
//-----------------------------------------------------------------------------
bool TextBlockCache::addText(const std::string& text, const Vector4& textBox, Align alignment, float size, bool kerning)
{
    if (m_maxTextBlocks > m_textBlocks.size())
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
    info.m_alignment = alignment;
    info.m_textHash = hash;
    info.m_textLenght = text.size();
    info.m_size = size;
    info.m_applyKerning = kerning;
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
void TextBlockInfo::ProcessText()
{
    float width = m_textBlockSize.w() - m_textBlockSize.y();


    List<fontquad> quads = new List<fontquad>();
    StringBlock b = m_strings[index];
    string text = b.Text;
    float x = b.TextBox.X;
    float y = b.TextBox.Y;
    float maxWidth = b.TextBox.Width;
    Align alignment = b.Alignment;
    float lineWidth = 0f;
    float sizeScale = b.Size / (float)m_charSet.RenderedSize;
    char lastChar = new char();
    int lineNumber = 1;
    int wordNumber = 1;
    float wordWidth = 0f;
    bool firstCharOfLine = true;

    float z = 0f;
    float rhw = 1f;

    for (int i = 0; i < text.Length; i++)
    {
        BitmapCharacter c = m_charSet.Characters1];
        float xOffset = c.XOffset * sizeScale;
        float yOffset = c.YOffset * sizeScale;
        float xAdvance = c.XAdvance * sizeScale;
        float width = c.Width * sizeScale;
        float height = c.Height * sizeScale;

        // Check vertical bounds
        if (y + yOffset + height > b.TextBox.Bottom)
        {
            break;
        }

        // Newline
        if (text[i] == '\n' || text[i] == '\r' || (lineWidth + xAdvance >= maxWidth))
        {
            if (alignment == Align.Left)
            {
                // Start at left
                x = b.TextBox.X;
            }
            if (alignment == Align.Center)
            {
                // Start in center
                x = b.TextBox.X + (maxWidth / 2f);
            }
            else if (alignment == Align.Right)
            {
                // Start at right
                x = b.TextBox.Right;
            }

            y += m_charSet.LineHeight * sizeScale;
            float offset = 0f;

            if ((lineWidth + xAdvance >= maxWidth) && (wordNumber != 1))
            {
                // Next character extends past text box width
                // We have to move the last word down one line
                char newLineLastChar = new char();
                lineWidth = 0f;
                for (int j = 0; j < quads.Count; j++)
                {
                    if (alignment == Align.Left)
                    {
                        // Move current word to the left side of the text box
                        if ((quads[j].LineNumber == lineNumber) &&
                            (quads[j].WordNumber == wordNumber))
                        {
                            quads[j].LineNumber++;
                            quads[j].WordNumber = 1;
                            quads[j].X = x + (quads[j].BitmapCharacter.XOffset * sizeScale);
                            quads[j].Y = y + (quads[j].BitmapCharacter.YOffset * sizeScale);
                            x += quads[j].BitmapCharacter.XAdvance * sizeScale;
                            lineWidth += quads[j].BitmapCharacter.XAdvance * sizeScale;
                            if (b.Kerning)
                            {
                                m_nextChar = quads[j].Character;
                                Kerning kern = m_charSet.Characters[newLineLastChar].KerningList.Find(FindKerningNode);
                                if (kern != null)
                                {
                                    x += kern.Amount * sizeScale;
                                    lineWidth += kern.Amount * sizeScale;
                                }
                            }
                        }
                    }
                    else if (alignment == Align.Center)
                    {
                        if ((quads[j].LineNumber == lineNumber) &&
                            (quads[j].WordNumber == wordNumber))
                        {
                            // First move word down to next line
                            quads[j].LineNumber++;
                            quads[j].WordNumber = 1;
                            quads[j].X = x + (quads[j].BitmapCharacter.XOffset * sizeScale);
                            quads[j].Y = y + (quads[j].BitmapCharacter.YOffset * sizeScale);
                            x += quads[j].BitmapCharacter.XAdvance * sizeScale;
                            lineWidth += quads[j].BitmapCharacter.XAdvance * sizeScale;
                            offset += quads[j].BitmapCharacter.XAdvance * sizeScale / 2f;
                            float kerning = 0f;
                            if (b.Kerning)
                            {
                                m_nextChar = quads[j].Character;
                                Kerning kern = m_charSet.Characters[newLineLastChar].KerningList.Find(FindKerningNode);
                                if (kern != null)
                                {
                                    kerning = kern.Amount * sizeScale;
                                    x += kerning;
                                    lineWidth += kerning;
                                    offset += kerning / 2f;
                                }
                            }
                        }
                    }
                    else if (alignment == Align.Right)
                    {
                        if ((quads[j].LineNumber == lineNumber) &&
                            (quads[j].WordNumber == wordNumber))
                        {
                            // Move character down to next line
                            quads[j].LineNumber++;
                            quads[j].WordNumber = 1;
                            quads[j].X = x + (quads[j].BitmapCharacter.XOffset * sizeScale);
                            quads[j].Y = y + (quads[j].BitmapCharacter.YOffset * sizeScale);
                            lineWidth += quads[j].BitmapCharacter.XAdvance * sizeScale;
                            x += quads[j].BitmapCharacter.XAdvance * sizeScale;
                            offset += quads[j].BitmapCharacter.XAdvance * sizeScale;
                            float kerning = 0f;
                            if (b.Kerning)
                            {
                                m_nextChar = quads[j].Character;
                                Kerning kern = m_charSet.Characters[newLineLastChar].KerningList.Find(FindKerningNode);
                                if (kern != null)
                                {
                                    kerning = kern.Amount * sizeScale;
                                    x += kerning;
                                    lineWidth += kerning;
                                    offset += kerning;
                                }
                            }
                        }
                    }
                    newLineLastChar = quads[j].Character;
                }

                // Make post-newline justifications
                if (alignment == Align.Center || alignment == Align.Right)
                {
                    // Justify the new line
                    for (int k = 0; k < quads.Count; k++)
                    {
                        if (quads[k].LineNumber == lineNumber + 1)
                        {
                            quads[k].X -= offset;
                        }
                    }
                    x -= offset;

                    // Rejustify the line it was moved from
                    for (int k = 0; k < quads.Count; k++)
                    {
                        if (quads[k].LineNumber == lineNumber)
                        {
                            quads[k].X += offset;
                        }
                    }
                }
            }
            else
            {
                // New line without any "carry-down" word
                firstCharOfLine = true;
                lineWidth = 0f;
            }

            wordNumber = 1;
            lineNumber++;

        } // End new line check

        // Don't print these
        if (text[i] == '\n' || text[i] == '\r' || text[i] == '\t')
        {
            continue;
        }

        // Set starting cursor for alignment
        if (firstCharOfLine)
        {
            if (alignment == Align.Left)
            {
                // Start at left
                x = b.TextBox.Left;
            }
            if (alignment == Align.Center)
            {
                // Start in center
                x = b.TextBox.Left + (maxWidth / 2f);
            }
            else if (alignment == Align.Right)
            {
                // Start at right
                x = b.TextBox.Right;
            }
        }

        // Adjust for kerning
        float kernAmount = 0f;
        if (b.Kerning && !firstCharOfLine)
        {
            m_nextChar = (char)text[i];
            Kerning kern = m_charSet.Characters[lastChar].KerningList.Find(FindKerningNode);
            if (kern != null)
            {
                kernAmount = kern.Amount * sizeScale;
                x += kernAmount;
                lineWidth += kernAmount;
                wordWidth += kernAmount;
            }
        }

        firstCharOfLine = false;

        // Create the vertices
        TransformedColoredTextured topLeft = new TransformedColoredTextured(
            x + xOffset, y + yOffset, z, rhw, b.Color.ToArgb(),
            (float)c.X / (float)m_charSet.Width,
            (float)c.Y / (float)m_charSet.Height);
        TransformedColoredTextured topRight = new TransformedColoredTextured(
            topLeft.X + width, y + yOffset, z, rhw, b.Color.ToArgb(),
            (float)(c.X + c.Width) / (float)m_charSet.Width,
            (float)c.Y / (float)m_charSet.Height);
        TransformedColoredTextured bottomRight = new TransformedColoredTextured(
            topLeft.X + width, topLeft.Y + height, z, rhw, b.Color.ToArgb(),
            (float)(c.X + c.Width) / (float)m_charSet.Width,
            (float)(c.Y + c.Height) / (float)m_charSet.Height);
        TransformedColoredTextured bottomLeft = new TransformedColoredTextured(
            x + xOffset, topLeft.Y + height, z, rhw, b.Color.ToArgb(),
            (float)c.X / (float)m_charSet.Width,
            (float)(c.Y + c.Height) / (float)m_charSet.Height);

        // Create the quad
        FontQuad q = new FontQuad(topLeft, topRight, bottomLeft, bottomRight);
        q.LineNumber = lineNumber;
        if (text[i] == ' ' && alignment == Align.Right)
        {
            wordNumber++;
            wordWidth = 0f;
        }
        q.WordNumber = wordNumber;
        wordWidth += xAdvance;
        q.WordWidth = wordWidth;
        q.BitmapCharacter = c;
        q.SizeScale = sizeScale;
        q.Character = text[i];
        quads.Add(q);

        if (text[i] == ' ' && alignment == Align.Left)
        {
            wordNumber++;
            wordWidth = 0f;
        }

        x += xAdvance;
        lineWidth += xAdvance;
        lastChar = text[i];

        // Rejustify text
        if (alignment == Align.Center)
        {
            // We have to recenter all Quads since we addded a 
            // new character
            float offset = xAdvance / 2f;
            if (b.Kerning)
            {
                offset += kernAmount / 2f;
            }
            for (int j = 0; j < quads.Count; j++)
            {
                if (quads[j].LineNumber == lineNumber)
                {
                    quads[j].X -= offset;
                }
            }
            x -= offset;
        }
        else if (alignment == Align.Right)
        {
            // We have to rejustify all Quads since we addded a 
            // new character
            float offset = 0f;
            if (b.Kerning)
            {
                offset += kernAmount;
            }
            for (int j = 0; j < quads.Count; j++)
            {
                if (quads[j].LineNumber == lineNumber)
                {
                    offset = xAdvance;
                    quads[j].X -= xAdvance;
                }
            }
            x -= offset;
        }
    }
}

}