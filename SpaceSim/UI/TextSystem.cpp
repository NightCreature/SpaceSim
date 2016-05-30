#include "TextSystem.h"

//
//TextSystem::TextSystem()
//{
//}
//
//
//TextSystem::~TextSystem()
//{
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//bool TextSystem::Init(void)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//bool TextSystem::Shutdown(void)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//bool TextSystem::GetGlyphs(IFont* pFont, const GlyphId* gIdArray, int glyphCount, GlyphDrawInfo* outGDI)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//EA::WebKit::IFont* TextSystem::GetFont(const EA::WebKit::TextStyle& textStyle, Char c /*= kCharInvalid*/)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//EA::WebKit::IFont* TextSystem::CreateNewFont(EA::WebKit::FontType fontType, const void* pSourceData, uint32_t nSourceSize, int nFaceIndex /*= 0*/)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//uint32_t TextSystem::AddFace(EA::IO::IStream* pStream)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//bool TextSystem::AddSubstitution(const char16_t* pFamily, const char16_t* pFamilySubstitution)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//bool TextSystem::SupportsFormat(EA::WebKit::FontFormat format)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//bool TextSystem::GetDrawInfoForComplexText(EA::WebKit::IFont* pFont, const EA::WebKit::Char* textRun, int textRunCount, float clipLeft, float clipRight, EA::WebKit::TextDrawInfo& outDrawInfo)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//void* TextSystem::CharacterBreakIterator(Char* pText, int length)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//void* TextSystem::WordBreakIterator(Char* pText, int length)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//void* TextSystem::LineBreakIterator(Char* pText, int length)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//void* TextSystem::SentenceBreakIterator(Char* pText, int length)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//int TextSystem::TextBreakFirst(void* pIterator)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//int TextSystem::TextBreakNext(void* pIterator)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//int TextSystem::TextBreakCurrent(void* pIterator)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//int TextSystem::TextBreakPrevious(void* pIterator)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//int TextSystem::TextBreakPreceding(void* pIterator, int position)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//int TextSystem::TextBreakFollowing(void* pIterator, int position)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//bool TextSystem::IsTextBreak(void* pIterator, int position)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//int TextSystem::FindNextWordFromIndex(Char* chars, int len, int position, bool forward)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//void TextSystem::FindWordBoundary(Char* chars, int len, int position, int* start, int* end)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//void* TextSystem::AcquireLineBreakIterator(Char* pText, int length)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//void TextSystem::ReleaseLineBreakIterator(void *pIterator)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//int32_t TextSystem::GetCombiningClass(Char c)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//EA::WebKit::Char TextSystem::GetMirrorChar(Char c)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//EA::WebKit::CharCategory TextSystem::GetCharCategory(Char c)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//EA::WebKit::BidiClass TextSystem::GetBidiClass(Char c)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//uint32_t TextSystem::ConvertCase(const Char* pTextInput, uint32_t nTextLength, Char* pTextOutput, uint32_t nTextOutputCapacity, CaseType caseType)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
//
////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//uint32_t TextSystem::GetFamilyNameArrayCapacity(void)
//{
//    throw std::logic_error("The method or operation is not implemented.");
//}
