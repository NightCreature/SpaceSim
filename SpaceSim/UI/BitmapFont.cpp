#include "BitmapFont.h"

#include "Graphics/DebugHelperFunctions.h"
#include "Graphics/DeviceManager.h"
#include "Core/Resource/GameResource.h"
#include "Graphics/texturemanager.h"

#include "Core/tinyxml2.h"
#include "Loader/ResourceLoader.h"
#include <filesystem>
#include "Loader/ResourceLoadJobs.h"

namespace Text
{

const size_t KerningInformation::firstIdHash = "first"_hash;
const size_t KerningInformation::secondIdHash = "second"_hash;
const size_t KerningInformation::ammountHash = "amount"_hash;

const size_t Glyph::idHash = "id"_hash;
const size_t Glyph::xHash = "x"_hash;
const size_t Glyph::yHash = "y"_hash;
const size_t Glyph::widthHash = "width"_hash;
const size_t Glyph::heightHash = "height"_hash;
const size_t Glyph::xOffsetHash = "xoffset"_hash;
const size_t Glyph::yOffsetHash = "yoffset"_hash;
const size_t Glyph::xAdvanceHash = "xadvance"_hash;
const size_t Glyph::pageHash = "page"_hash;
const size_t Glyph::channelHash = "channel"_hash;

const size_t FontInfo::fontNameHash = "face"_hash;
const size_t FontInfo::sizeHash = "size"_hash;
const size_t FontInfo::boldHash = "bold"_hash;
const size_t FontInfo::italicHash = "italic"_hash;
const size_t FontInfo::charsetHash = "charset"_hash;
const size_t FontInfo::unicodeHash = "unicode"_hash;
const size_t FontInfo::horizontalStretchHash = "stretchH"_hash;
const size_t FontInfo::smootHash = "smooth"_hash;
const size_t FontInfo::antiAliasedHash = "aa"_hash;
const size_t FontInfo::outlineHash = "outline"_hash;

const size_t CommonFontInfo::lineHeightHash = "lineHeight"_hash;
const size_t CommonFontInfo::baseHash = "base"_hash;
const size_t CommonFontInfo::widthScaleHash = "scaleW"_hash;
const size_t CommonFontInfo::heightScaleHash = "scaleH"_hash;
const size_t CommonFontInfo::pagesHash = "pages"_hash;
const size_t CommonFontInfo::packedHash = "packed"_hash;
const size_t CommonFontInfo::alphaChannelHash = "alphaChnl"_hash;
const size_t CommonFontInfo::redChannelHash = "redChnl"_hash;
const size_t CommonFontInfo::greenChannelHash = "greenChnl"_hash;
const size_t CommonFontInfo::blueChannelHash = "blueChnl"_hash;

const size_t Pages::PageInfo::idHash = "id"_hash;
const size_t Pages::PageInfo::fileHash = "file"_hash;

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
bool BitmapFont::openFont(const std::string& bmpFile, Resource* resource)
{
    using namespace tinyxml2;

    std::string currentPath = extractPathFromFileName(bmpFile);
    RenderResourceHelper gameResource(resource);

    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(bmpFile.c_str());
    ASSERT_CHANNEL(error == XML_NO_ERROR, "BitmapFont", "Failed to load bitmap font definition file %s", bmpFile.c_str());

    tinyxml2::XMLElement* root = document.FirstChildElement();
    ASSERT_CHANNEL(root, "BitmapFont", "Failed to get root element for bitmap font defintion file %s", bmpFile.c_str());


    //<font>
    //  <info face = "Arial" size = "100" bold = "0" italic = "0" charset = "" unicode = "1" stretchH = "100" smooth = "1" aa = "2" padding = "0,0,0,0" spacing = "1,1" outline = "0" / >
    //  <common lineHeight = "100" base = "81" scaleW = "512" scaleH = "512" pages = "1" packed = "0" alphaChnl = "1" redChnl = "0" greenChnl = "0" blueChnl = "0" / >
    //  <pages>
    //    <page id = "0" file = "arialhighres.fnt.conv.tga" / >
    //  < / pages>
    //  <chars count = "100">
    //    <char id = "32" x = "506" y = "0" width = "0.125" height = "0.125" xoffset = "0" yoffset = "0" xadvance = "25" page = "0" chnl = "15" / >
    //  <kernings count="92">
    //    <kerning first = "32" second = "65" amount = "-4.875" / >

    tinyxml2::XMLElement* infoElement = root->FirstChildElement("info");
    tinyxml2::XMLElement* commonElement = root->FirstChildElement("common");
    tinyxml2::XMLElement* pageElement = root->FirstChildElement("pages");
    tinyxml2::XMLElement* charElement = root->FirstChildElement("chars");
    tinyxml2::XMLElement* kernElement = root->FirstChildElement("kernings");

    ASSERT_CHANNEL(infoElement && commonElement && pageElement && charElement, "BitmapFont", "Failed to get required elements for bitmap font defintion file %s", bmpFile.c_str());

    if (infoElement)
    {
        for (const tinyxml2::XMLAttribute* attribute = infoElement->FirstAttribute(); attribute; attribute = attribute->Next())
        {
            size_t hashAttribute = Hashing::hashString(attribute->Name());
            if (FontInfo::fontNameHash == hashAttribute)
            {
                m_fontInformation.m_fontName = attribute->Value();
                m_fontInformation.m_fontNameHash = Hashing::hashString(attribute->Value());
            }
            else if (FontInfo::sizeHash == hashAttribute)
            {
                m_fontInformation.m_fontSize = static_cast<size_t>(attribute->IntValue());
            }
            else if (FontInfo::boldHash == hashAttribute)
            {
                m_fontInformation.m_bold = attribute->BoolValue();
            }
            else if (FontInfo::italicHash == hashAttribute)
            {
                m_fontInformation.m_italic = attribute->BoolValue();
            }
            else if (FontInfo::charsetHash == hashAttribute)
            {
                m_fontInformation.m_charSet = attribute->Value();
            }
            else if (FontInfo::unicodeHash == hashAttribute)
            {
                m_fontInformation.m_unicode = attribute->BoolValue();
            }
            else if (FontInfo::horizontalStretchHash == hashAttribute)
            {
                m_fontInformation.m_horizontalStretchPercentage = static_cast<size_t>(attribute->IntValue());
            }
            else if (FontInfo::smootHash == hashAttribute)
            {
                m_fontInformation.m_smooth = attribute->BoolValue();
            }
            else if (FontInfo::antiAliasedHash == hashAttribute)
            {
                m_fontInformation.m_antiAliased = static_cast<short>(attribute->IntValue());
            }
            else if (FontInfo::outlineHash == hashAttribute)
            {
                m_fontInformation.m_outline = attribute->BoolValue();
            }
        }
    }

    if (commonElement)
    {
        for (const tinyxml2::XMLAttribute* attribute = commonElement->FirstAttribute(); attribute; attribute = attribute->Next())
        {
            size_t hashAttribute = Hashing::hashString(attribute->Name());
            if (CommonFontInfo::lineHeightHash == hashAttribute)
            {
                m_commonFontInformation.m_lineHeight = static_cast<short>(attribute->IntValue());
            }
            else if (CommonFontInfo::baseHash == hashAttribute)
            {
                m_commonFontInformation.m_base = static_cast<short>(attribute->IntValue());
            }
            else if (CommonFontInfo::widthScaleHash == hashAttribute)
            {
                m_commonFontInformation.m_widthScale = static_cast<short>(attribute->IntValue());
            }
            else if (CommonFontInfo::heightScaleHash == hashAttribute)
            {
                m_commonFontInformation.m_heightScale = static_cast<short>(attribute->IntValue());
            }
            else if (CommonFontInfo::pagesHash == hashAttribute)
            {
                m_commonFontInformation.m_numberOfPages = static_cast<short>(attribute->IntValue());
            }
            else if (CommonFontInfo::packedHash == hashAttribute)
            {
                m_commonFontInformation.m_packed = attribute->BoolValue();
            }
            else if (CommonFontInfo::alphaChannelHash == hashAttribute)
            {
                m_commonFontInformation.m_alphaChannel = attribute->BoolValue();
            }
            else if (CommonFontInfo::redChannelHash == hashAttribute)
            {
                m_commonFontInformation.m_redChannel = attribute->BoolValue();
            }
            else if (CommonFontInfo::greenChannelHash == hashAttribute)
            {
                m_commonFontInformation.m_greenChannel = attribute->BoolValue();
            }
            else if (CommonFontInfo::blueChannelHash == hashAttribute)
            {
                m_commonFontInformation.m_blueChannel = attribute->BoolValue();
            }
        }
    }

    if (pageElement)
    {
        for (tinyxml2::XMLElement* page = pageElement->FirstChildElement(); page; page = page->NextSiblingElement())
        {
            Pages::PageInfo pageInfo;
            for (const tinyxml2::XMLAttribute* attribute = page->FirstAttribute(); attribute; attribute = attribute->Next())
            {
                size_t hashAttribute = Hashing::hashString(attribute->Name());
                if (Pages::PageInfo::idHash == hashAttribute)
                {
                    pageInfo.m_id = static_cast<short>(attribute->IntValue());
                }
                else if (Pages::PageInfo::fileHash == hashAttribute)
                {
                    pageInfo.m_fileName = attribute->Value();

                    std::filesystem::path path = currentPath / pageInfo.m_fileName;
                    LoadRequest loadRequest(path.string());
                    loadRequest.m_gameObjectId = 0;
                    loadRequest.m_resourceType = "LOAD_TEXTURE"_hash;
                    
                    gameResource.getWriteableResource().getResourceLoader().AddLoadRequest(std::move(loadRequest));
                }
            }
            m_pagesInformation.m_pages.push_back(pageInfo);
        }
    }

    for (tinyxml2::XMLElement* glyphElement = charElement->FirstChildElement(); glyphElement; glyphElement = glyphElement->NextSiblingElement())
    {
        Glyph glyph;
        for (const tinyxml2::XMLAttribute* attribute = glyphElement->FirstAttribute(); attribute; attribute = attribute->Next())
        {
            size_t hashAttribute = Hashing::hashString(attribute->Name());
            if (Glyph::idHash == hashAttribute)
            {
                glyph.m_id = static_cast<short>(attribute->IntValue());
            }
            else if (Glyph::xHash == hashAttribute)
            {
                glyph.m_x = attribute->FloatValue();
            }
            else if (Glyph::yHash == hashAttribute)
            {
                glyph.m_y = attribute->FloatValue();
            }
            else if (Glyph::widthHash == hashAttribute)
            {
                glyph.m_width = attribute->FloatValue();
            }
            else if (Glyph::heightHash == hashAttribute)
            {
                glyph.m_height = attribute->FloatValue();
            }
            else if (Glyph::xOffsetHash == hashAttribute)
            {
                glyph.m_xOffset = attribute->FloatValue();
            }
            else if (Glyph::yOffsetHash == hashAttribute)
            {
                glyph.m_yOffset = attribute->FloatValue();
            }
            else if (Glyph::xAdvanceHash == hashAttribute)
            {
                glyph.m_xAdvance = attribute->FloatValue();
            }
        }
        m_glyphs.emplace_back(glyph);
    }

    if (kernElement)
    {
        //Read kerning pairs
        for (tinyxml2::XMLElement* kernInfoElement = kernElement->FirstChildElement(); kernInfoElement; kernInfoElement = kernInfoElement->NextSiblingElement())
        {
            KerningInformation kernInfo;
            Glyph* glyph = nullptr;
            for (const tinyxml2::XMLAttribute* attribute = kernInfoElement->FirstAttribute(); attribute; attribute = attribute->Next())
            {
                size_t hashAttribute = Hashing::hashString(attribute->Name());
                if (KerningInformation::firstIdHash == hashAttribute)
                {
                    glyph = findGlyph(static_cast<short>(attribute->IntValue()));
                }
                else if (KerningInformation::secondIdHash == hashAttribute)
                {
                    kernInfo.m_secondId = static_cast<short>(attribute->IntValue());
                }
                else if (KerningInformation::ammountHash == hashAttribute)
                {
                    kernInfo.m_ammount = attribute->FloatValue();
                }
            }

            if (glyph)
            {
                glyph->addKerningInfo(kernInfo);
            }
        }
    }

    return true;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
const Glyph& BitmapFont::getGlyph(short id) const
{
    for (auto& glyph : m_glyphs)
    {
        if (glyph.m_id == id)
        {
            return glyph;
        }
    }

    return m_invalidGlyph;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
Glyph* BitmapFont::findGlyph(short id)
{

    for (auto& glyph : m_glyphs)
    {
        if (glyph.m_id == id)
        {
            return &glyph;
        }
    }

    return nullptr;
}

}