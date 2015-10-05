#pragma once

#include "vector2.h"
#include "vector4.h"
#include "BitmapFont.h"
#include "Material.h"
#include "..\GeometryInstance.h"
#include "..\ShaderInstance.h"
#include "..\RenderInstance.h"
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

//typedef Vector4 GlyphVertex; //xy is position, zw is uv, 0 = TL, 1 = BL, 2 = BR, 3 = TR
struct GlyphVertex
{
	Vector3 position;
	Vector2 uv;
};

struct TextBlockInfo;

struct GlyphQuad
{
	size_t m_vertexOffset; //Offset into the vertex array of the TextblockInfo
    Glyph* m_bitmapChar;
    size_t m_lineNumber;
    size_t m_wordNumber;
    float m_sizeScale;
    float m_wordWidth;
    char m_character;

    void setY(float value, TextBlockInfo& textBlock);
    void setX(float value, TextBlockInfo& textBlock);
};

struct TextBlockInfo
{
	TextBlockInfo() : m_renderInstance(nullptr) {}
	std::vector<GlyphQuad> m_glyphQuads;
	std::vector<GlyphVertex> m_glyphVerts;
	std::string m_text; //We should not have this and have length of the string instead with the hash, though debug would be handy to have
	Vector4 m_textBlockSize; //xy is top left, zw is bottom right
	Align m_alignment;
	size_t m_textHash;
	size_t m_textLenght;
	float m_size;
	bool m_applyKerning;
	BitmapFont* m_font;
	VertexBuffer vb;
	IndexBuffer ib;
	ShaderInstance m_shaderInstance;
	GeometryInstance m_geometryInstance;
	RenderInstance* m_renderInstance;

	bool ProcessText(Resource* resource);
	RenderInstance* getRenderInstance() { return m_renderInstance; }
private:
	void CreateVertexBuffer(Resource* resource);
	void CreateShaderSetup(Resource* resource);

	void ProcessTextForReals(Resource* resource) { UNUSEDPARAM(resource);  }
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

	void ProvideRenderInstances(RenderInstanceTree& renderInstances);

    TextBlockInfo& getTextBlock(size_t index) { return m_textBlocks[index]; } //Rewrite this so its safe if the index is not there
private:
    BitmapFont* getBitmapFont(size_t fontNameHash);

    std::vector<TextBlockInfo> m_textBlocks;
	std::vector<RenderInstance*> m_textBlocksToRender;
    std::vector<BitmapFont> m_fonts;
    size_t m_maxTextBlocks;
    Resource* m_resource;
};

}