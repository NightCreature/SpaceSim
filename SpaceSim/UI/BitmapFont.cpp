#include "BitmapFont.h"

#include "..\DebugHelperFunctions.h"
#include "..\DeviceManager.h"
#include "..\GameResource.h"
#include "texturemanager.h"

#include "..\tinyxml2.h"

namespace BitmapFont
{

const size_t KerningInformation::firstIdHash = hashString("first");
const size_t KerningInformation::secondIdHash = hashString("second");
const size_t KerningInformation::ammountHash = hashString("amount");

const size_t Glyph::idHash = hashString("id");
const size_t Glyph::xHash = hashString("x");
const size_t Glyph::yHash = hashString("y");
const size_t Glyph::widthHash = hashString("width");
const size_t Glyph::heightHash = hashString("height");
const size_t Glyph::xOffsetHash = hashString("xoffset");
const size_t Glyph::yOffsetHash = hashString("yoffset");
const size_t Glyph::xAdvanceHash = hashString("xadvance");
const size_t Glyph::pageHash = hashString("page");
const size_t Glyph::channelHash = hashString("channel");

const size_t FontInfo::fontNameHash = hashString("face");
const size_t FontInfo::sizeHash = hashString("size");
const size_t FontInfo::boldHash = hashString("bold");
const size_t FontInfo::italicHash = hashString("italic");
const size_t FontInfo::charsetHash = hashString("charset");
const size_t FontInfo::unicodeHash = hashString("unicode");
const size_t FontInfo::horizontalStretchHash = hashString("stretchH");
const size_t FontInfo::smootHash = hashString("smooth");
const size_t FontInfo::antiAliasedHash = hashString("aa");
const size_t FontInfo::outlineHash = hashString("outline");

const size_t CommonFontInfo::lineHeightHash = hashString("lineHeight");
const size_t CommonFontInfo::baseHash = hashString("base");
const size_t CommonFontInfo::widthScaleHash = hashString("scaleW");
const size_t CommonFontInfo::heightScaleHash = hashString("scaleH");
const size_t CommonFontInfo::pagesHash = hashString("pages");
const size_t CommonFontInfo::packedHash = hashString("packed");
const size_t CommonFontInfo::alphaChannelHash = hashString("alphaChnl");
const size_t CommonFontInfo::redChannelHash = hashString("redChnl");
const size_t CommonFontInfo::greenChannelHash = hashString("greenChnl");
const size_t CommonFontInfo::blueChannelHash = hashString("blueChnl");

const size_t Pages::PageInfo::idHash = hashString("id");
const size_t Pages::PageInfo::fileHash = hashString("file");

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool BitmapFont::openFont(const std::string& bmpFile, Resource* resource)
{
    using namespace tinyxml2;

    std::string currentPath = extractPathFromFileName(bmpFile);
    GameResourceHelper gameResource(resource);

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
            size_t hashAttribute = hashString(attribute->Name());
            if (FontInfo::fontNameHash == hashAttribute)
            {
                m_fontInformation.m_fontName = attribute->Value();
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
            size_t hashAttribute = hashString(attribute->Name());
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
                size_t hashAttribute = hashString(attribute->Name());
                if (Pages::PageInfo::idHash == hashAttribute)
                {
                    pageInfo.m_id = static_cast<short>(attribute->IntValue());
                }
                else if (Pages::PageInfo::fileHash == hashAttribute)
                {
                    pageInfo.m_fileName = attribute->Value();
                    gameResource.getWritableGameResource().getTextureManager().addLoad(gameResource.getGameResource().getDeviceManager(), currentPath + pageInfo.m_fileName);
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
            size_t hashAttribute = hashString(attribute->Name());
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
                size_t hashAttribute = hashString(attribute->Name());
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

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
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