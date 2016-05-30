#pragma once

#include "Graphics/texture.h"

#include <string>
#include <vector>

class Resource;

namespace Text
{

//-----------------------------------------------------------------------------
//! @brief   structure that represents the kerning data for glyphs
//! @remark
//-----------------------------------------------------------------------------
struct KerningInformation
{
    KerningInformation() : m_secondId(-1), m_ammount(0.0f) {}
    KerningInformation(short id, float ammount) : m_secondId(id), m_ammount(ammount) {}
    ~KerningInformation() {}

    short m_secondId;
    float m_ammount;

    static const size_t firstIdHash;
    static const size_t secondIdHash;
    static const size_t ammountHash;
};


//-----------------------------------------------------------------------------
//! @brief   structure that represents a glyph from a bitmap font
//! @remark
//-----------------------------------------------------------------------------
struct Glyph
{
public:
    Glyph() : m_id(-1), m_x(0.0f), m_y(0.0f), m_width(0.0f), m_height(0.0f), m_xOffset(0.0f), m_yOffset(0.0f), m_xAdvance(0.0f) {}
    Glyph(short id, float x, float y, float width, float height, float xOffset, float yOffset, float xAdvance)
        : m_id(id), m_x(x), m_y(y), m_width(width), m_height(height), m_xOffset(xOffset), m_yOffset(yOffset), m_xAdvance(xAdvance) {}

    void addKerningInfo(const KerningInformation& kernInfo) { m_kerningInformation.emplace_back(kernInfo); }
    size_t findKernIndex(short id) const
    { 
        size_t index = static_cast<size_t>(-1);

        for (size_t counter = 0; counter < m_kerningInformation.size(); ++counter )
        {
            if (m_kerningInformation[counter].m_secondId == id)
            {
                index = counter;
                counter = m_kerningInformation.size();
            }
        }

        return index;
    }
    const KerningInformation* getKerningInfoFor(short id) const 
    {
        size_t kernIndex = findKernIndex(id);
        return m_kerningInformation.empty() || kernIndex > m_kerningInformation.size() ? nullptr : &m_kerningInformation[kernIndex];
    }

    short m_id;
    float m_x; //pos in texture, needed to find the uv coordinate
    float m_y; //pos in texture, needed to find the uv coordinate
    float m_width; //width in texture, needed to find the uv coordinate
    float m_height; //height in texture, needed to find the uv coordinate
    float m_xOffset; //Add this to the current cursor to find the left edge of the glyph square
    float m_yOffset;
    float m_xAdvance; //Add this to the current cursor to find the next cursor position, should add the kerning value if there is one for next cursor position
    std::vector<KerningInformation> m_kerningInformation;

    static const size_t idHash;
    static const size_t xHash;
    static const size_t yHash;
    static const size_t widthHash;
    static const size_t heightHash;
    static const size_t xOffsetHash;
    static const size_t yOffsetHash;
    static const size_t xAdvanceHash;
    static const size_t pageHash;
    static const size_t channelHash;
};

struct FontInfo
{
    std::string m_fontName;
    std::string m_charSet;
    size_t m_fontSize;
    size_t m_horizontalStretchPercentage;
    size_t m_fontNameHash;
    short m_antiAliased;
    bool m_bold;
    bool m_italic;
    bool m_unicode;
    bool m_smooth;
    bool m_outline;

    static const size_t fontNameHash;
    static const size_t sizeHash;
    static const size_t boldHash;
    static const size_t italicHash;
    static const size_t charsetHash;
    static const size_t unicodeHash;
    static const size_t horizontalStretchHash;
    static const size_t smootHash;
    static const size_t antiAliasedHash;
    static const size_t outlineHash;
};

struct CommonFontInfo
{
    short m_lineHeight;
    short m_base;
    short m_widthScale; //Holds the width of the texture size
    short m_heightScale; //Holds the height of the texture size
    short m_numberOfPages;
    bool m_packed;
    bool m_alphaChannel;
    bool m_redChannel;
    bool m_greenChannel;
    bool m_blueChannel;

    static const size_t lineHeightHash;
    static const size_t baseHash;
    static const size_t widthScaleHash;
    static const size_t heightScaleHash;
    static const size_t pagesHash;
    static const size_t packedHash;
    static const size_t alphaChannelHash;
    static const size_t redChannelHash;
    static const size_t greenChannelHash;
    static const size_t blueChannelHash;
};


struct Pages
{
    struct PageInfo
    {
        short m_id;
        std::string m_fileName;

        static const size_t idHash;
        static const size_t fileHash;
    };

    std::vector<PageInfo> m_pages;
};

//-----------------------------------------------------------------------------
//! @brief   Class that represents a bitmap font or SDF font
//! @remark
//-----------------------------------------------------------------------------
class BitmapFont
{
public:
    BitmapFont() {}
    ~BitmapFont() { m_glyphs.clear(); }

    bool openFont(const std::string& bmpFile, Resource* resource);
    const Glyph& getGlyph(short id) const;
    const FontInfo& getFontInfo() const { return m_fontInformation; }
    const CommonFontInfo& getCommonFontInfo() const { return m_commonFontInformation; }
	const Pages& getPages() const { return m_pagesInformation;  }
    bool isSdfFont() const { return m_isSDFFont; }
private:
    Glyph* findGlyph(short id);

    Glyph m_invalidGlyph;
    std::vector<Glyph> m_glyphs;
    FontInfo m_fontInformation;
    CommonFontInfo m_commonFontInformation;
    Pages m_pagesInformation;
    bool m_isSDFFont;
};

}