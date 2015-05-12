#pragma once

#include "vector4.h"
#include <string>
#include <vector>

namespace Text
{

struct Glyph;

enum Align
{
    right,
    center,
    left
};

struct GlyphQuad
{
    Vector4 m_vertices[4]; //xy is position, zw is uv
    Glyph* m_bitmapChar;
    size_t m_lineNumber;
    size_t m_wordNumber;
    float m_sizeScale;
    float m_wordWidth;
    char m_character;
};

struct TextBlockInfo
{
    std::vector<GlyphQuad> m_glyphQuads;
    std::string m_text; //We should not have this and have length of the string instead with the hash, though debug would be handy to have
    Vector4 m_textBlockSize; //xy is top left, zw is bottom right
    Align m_alignment;
    size_t m_textHash;
    size_t m_textLenght; 
    float m_size;
    bool m_applyKerning;

    void ProcessText();

};

class TextBlockCache
{
public:
    TextBlockCache(size_t maxTextblocks) : m_maxTextBlocks(maxTextblocks) { m_textBlocks.reserve(maxTextblocks); }
    ~TextBlockCache() {}

    bool addText(const std::string& text, const Vector4& textBox, Align alignment, float size, bool applyKerning);
    size_t getTextIndex(const std::string& text);
    size_t getTextIndex(size_t textHash);
    bool removeText(const std::string& text);
    bool removeText(size_t text);
    void removeAllTexts();
private:
    std::vector<TextBlockInfo> m_textBlocks;
    size_t m_maxTextBlocks;
};

}