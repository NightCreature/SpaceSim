#pragma once

#include "vector4.h"
#include "BitmapFont.h"
#include "Material.h"
#include "..\VertexBuffer.h"
#include <string>
#include <vector>

class Resource;

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
    Vector4 m_vertices[4]; //xy is position, zw is uv, 0 = TL, 1 = BL, 2 = BR, 3 = TR
    Glyph* m_bitmapChar;
    size_t m_lineNumber;
    size_t m_wordNumber;
    float m_sizeScale;
    float m_wordWidth;
    char m_character;

    void setY(float value)
    {
        for (size_t counter = 0; counter < 4; ++counter)
        {
            Vector4 temp(0.f, value + counter == 1 || counter == 2 ? value : 0.f, 0.f, 0.f);
            m_vertices[counter] += temp;
        }
    }

    void setX(float value)
    {
        for (size_t counter = 0; counter < 4; ++counter)
        {
            Vector4 temp(value + counter == 1 || counter == 2 ? value : 0.f, 0.f, 0.f, 0.f);
            m_vertices[counter] += temp;
        }
    }
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
    BitmapFont* m_font;
    Material m_material;
    VertexBuffer m_vertexBuffer;

    void ProcessText(Resource* resource);

private:
    void CreateVertexBuffer(Resource* resource);

};

class TextBlockCache
{
public:
    TextBlockCache(size_t maxTextblocks, Resource* resource) : m_maxTextBlocks(maxTextblocks), m_resource(resource) { m_textBlocks.reserve(maxTextblocks); }
    ~TextBlockCache() {}

    bool addFont(const std::string& fileName);
    bool addText(const std::string& text, const Vector4& textBox, Align alignment, size_t fontHash, float size, bool applyKerning);
    size_t getTextIndex(const std::string& text);
    size_t getTextIndex(size_t textHash);
    bool removeText(const std::string& text);
    bool removeText(size_t text);
    void removeAllTexts();

    TextBlockInfo& getTextBlock(size_t index) { return m_textBlocks[index]; } //Rewrite this so its safe if the index is not there
private:
    BitmapFont* getBitmapFont(size_t fontNameHash);

    std::vector<TextBlockInfo> m_textBlocks;
    std::vector<BitmapFont> m_fonts;
    size_t m_maxTextBlocks;
    Resource* m_resource;
};

}