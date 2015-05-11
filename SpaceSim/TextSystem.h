#pragma once

//#include <EAWebKit/EAWebKit>
//
//class TextSystem : public EA::WebKit::ITextSystem
//{
//public:
//    TextSystem();
//    ~TextSystem();
//
//    virtual bool Init(void) override;
//    virtual bool Shutdown(void) override;
//
//    virtual bool GetGlyphs(IFont* pFont, const GlyphId* gIdArray, int glyphCount, GlyphDrawInfo* outGDI) override;
//    virtual EA::WebKit::IFont* GetFont(const EA::WebKit::TextStyle& textStyle, Char c = kCharInvalid) override;
//    virtual EA::WebKit::IFont* CreateNewFont(EA::WebKit::FontType fontType, const void* pSourceData, uint32_t nSourceSize, int nFaceIndex = 0) override;
//    virtual uint32_t AddFace(EA::IO::IStream* pStream) override;
//    virtual bool AddSubstitution(const char16_t* pFamily, const char16_t* pFamilySubstitution) override;
//    virtual bool SupportsFormat(EA::WebKit::FontFormat format) override;
//    virtual bool GetDrawInfoForComplexText(EA::WebKit::IFont* pFont, const EA::WebKit::Char* textRun, int textRunCount, float clipLeft, float clipRight, EA::WebKit::TextDrawInfo& outDrawInfo) override;
//    virtual void* CharacterBreakIterator(Char* pText, int length) override;
//    virtual void* WordBreakIterator(Char* pText, int length) override;
//    virtual void* LineBreakIterator(Char* pText, int length) override;
//    virtual void* SentenceBreakIterator(Char* pText, int length) override;
//    virtual int TextBreakFirst(void* pIterator) override;
//    virtual int TextBreakNext(void* pIterator) override;
//    virtual int TextBreakCurrent(void* pIterator) override;
//    virtual int TextBreakPrevious(void* pIterator) override;
//    virtual int TextBreakPreceding(void* pIterator, int position) override;
//    virtual int TextBreakFollowing(void* pIterator, int position) override;
//    virtual bool IsTextBreak(void* pIterator, int position) override;
//    virtual int FindNextWordFromIndex(Char* chars, int len, int position, bool forward) override;
//    virtual void FindWordBoundary(Char* chars, int len, int position, int* start, int* end) override;
//    virtual void* AcquireLineBreakIterator(Char* pText, int length) override;
//    virtual void ReleaseLineBreakIterator(void *pIterator) override;
//    virtual int32_t GetCombiningClass(Char c) override;
//    virtual Char GetMirrorChar(Char c) override;
//    virtual CharCategory GetCharCategory(Char c) override;
//    virtual BidiClass GetBidiClass(Char c) override;
//    virtual uint32_t ConvertCase(const Char* pTextInput, uint32_t nTextLength, Char* pTextOutput, uint32_t nTextOutputCapacity, CaseType caseType) override;
//    virtual uint32_t GetFamilyNameArrayCapacity(void) override;
//
//};

