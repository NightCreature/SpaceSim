/*
   AngelCode Bitmap Font Generator
   Copyright (c) 2004-2021 Andreas Jonsson
  
   This software is provided 'as-is', without any express or implied 
   warranty. In no event will the authors be held liable for any 
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any 
   purpose, including commercial applications, and to alter it and 
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you 
      must not claim that you wrote the original software. If you use
      this software in a product, an acknowledgment in the product 
      documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and 
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source 
      distribution.
  
   Andreas Jonsson
   andreas@angelcode.com
*/

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <map>
#include <sstream>
using std::map;

#include "dynamic_funcs.h"
#include "ac_string_util.h"
#include "acutil_unicode.h"
#include "fontgen.h"

#include "unicode.h"

// These are the defined character sets from the Unicode standard
// http://www.unicode.org/charts/PDF/
const UnicodeSubset_t UnicodeSubsets[] = {
{"Latin"                                  , 0x000000, 0x00007F}, // 128
{"Latin-1 Supplement"                     , 0x000080, 0x0000FF}, // 128
{"Latin Extended-A"                       , 0x000100, 0x00017F}, // 128
{"Latin Extended-B"                       , 0x000180, 0x00024F}, // 208
{"IPA Extensions"                         , 0x000250, 0x0002AF}, // 96
{"Spacing Modifier Letters"               , 0x0002B0, 0x0002FF}, // 80
{"Combining Diacritical Marks"            , 0x000300, 0x00036F}, // 112
{"Greek"                                  , 0x000370, 0x0003FF}, // 144
{"Cyrillic"                               , 0x000400, 0x0004FF}, // 256
{"Cyrillic Supplement"                    , 0x000500, 0x00052F}, // 48
{"Armenian"                               , 0x000530, 0x00058F}, // 96
{"Hebrew"                                 , 0x000590, 0x0005FF}, // 112
{"Arabic"                                 , 0x000600, 0x0006FF}, // 256
{"Syriac"                                 , 0x000700, 0x00074F}, // 80
{"Arabic Supplement"                      , 0x000750, 0x00077F}, // 48
{"Thaana"                                 , 0x000780, 0x0007BF}, // 64
{"N'Ko"                                   , 0x0007C0, 0x0007FF}, // 64
{"Samaritan"                              , 0x000800, 0x00083F}, // 64
{"Mandaic"                                , 0x000840, 0x00085F}, // 32
{"Syriac Supplement"                      , 0x000860, 0x00086F}, // 16
{"{undefined}"                            , 0x000870, 0x00089F}, // 48
{"Arabic Extended-A"                      , 0x0008A0, 0x0008FF}, // 96
{"Devanagari"                             , 0x000900, 0x00097F}, // 128
{"Bengali and Assamese"                   , 0x000980, 0x0009FF}, // 128
{"Gurmukhi"                               , 0x000A00, 0x000A7F}, // 128
{"Gujarati"                               , 0x000A80, 0x000AFF}, // 128
{"Oriya (Odia)"                           , 0x000B00, 0x000B7F}, // 128
{"Tamil"                                  , 0x000B80, 0x000BFF}, // 128
{"Telugu"                                 , 0x000C00, 0x000C7F}, // 128
{"Kannada"                                , 0x000C80, 0x000CFF}, // 128
{"Malayalam"                              , 0x000D00, 0x000D7F}, // 128
{"Sinhala"                                , 0x000D80, 0x000DFF}, // 128
{"Thai"                                   , 0x000E00, 0x000E7F}, // 128
{"Lao"                                    , 0x000E80, 0x000EFF}, // 128
{"Tibetan"                                , 0x000F00, 0x000FFF}, // 256
{"Myanmar"                                , 0x001000, 0x00109F}, // 160
{"Georgian"                               , 0x0010A0, 0x0010FF}, // 96
{"Hangul Jamo"                            , 0x001100, 0x0011FF}, // 256
{"Ethiopic"                               , 0x001200, 0x00137F}, // 384
{"Ethiopic Supplement"                    , 0x001380, 0x00139F}, // 32
{"Cherokee"                               , 0x0013A0, 0x0013FF}, // 96
{"Unified Canadian Aboriginal Syllabics"  , 0x001400, 0x00167F}, // 640
{"Ogham"                                  , 0x001680, 0x00169F}, // 32
{"Runic"                                  , 0x0016A0, 0x0016FF}, // 96
{"Tagalog"                                , 0x001700, 0x00171F}, // 32
{"Hanunoo"                                , 0x001720, 0x00173F}, // 32
{"Buhid"                                  , 0x001740, 0x00175F}, // 32
{"Tagbanwa"                               , 0x001760, 0x00177F}, // 32
{"Khmer"                                  , 0x001780, 0x0017FF}, // 128
{"Mongolian"                              , 0x001800, 0x0018AF}, // 176
{"UCAS Extended"                          , 0x0018B0, 0x0018FF}, // 80
{"Limbu"                                  , 0x001900, 0x00194F}, // 80
{"Tai Le"                                 , 0x001950, 0x00197F}, // 48
{"New Tai Lue"                            , 0x001980, 0x0019DF}, // 96
{"Khmer Symbols"                          , 0x0019E0, 0x0019FF}, // 32
{"Buginese"                               , 0x001A00, 0x001A1F}, // 32
{"Tai Tham"                               , 0x001A20, 0x001AAF}, // 144
{"Combining Diacritical Marks Extended"   , 0x001AB0, 0x001AFF}, // 80
{"Balinese"                               , 0x001B00, 0x001B7F}, // 128
{"Sundanese"                              , 0x001B80, 0x001BBF}, // 64
{"Batak"                                  , 0x001BC0, 0x001BFF}, // 64
{"Lepcha"                                 , 0x001C00, 0x001C4F}, // 80
{"Ol Chiki"                               , 0x001C50, 0x001C7F}, // 48
{"Cyrillic Extended-C"                    , 0x001C80, 0x001C8F}, // 16
{"Georgian Extended"                      , 0x001C90, 0x001CBF}, // 48
{"Sundanese Supplement"                   , 0x001CC0, 0x001CCF}, // 16
{"Vedic Extensions"                       , 0x001CD0, 0x001CFF}, // 48
{"Phonetic Extensions"                    , 0x001D00, 0x001D7F}, // 128
{"Phonetic Extensions Supplement"         , 0x001D80, 0x001DBF}, // 64
{"Combining Diacritical Marks Supplement" , 0x001DC0, 0x001DFF}, // 64
{"Latin Extended Additional"              , 0x001E00, 0x001EFF}, // 256
{"Greek Extended"                         , 0x001F00, 0x001FFF}, // 256
{"General Punctuation"                    , 0x002000, 0x00206F}, // 112
{"Superscripts and Subscripts"            , 0x002070, 0x00209F}, // 48
{"Currency Symbols"                       , 0x0020A0, 0x0020CF}, // 48
{"Combining Diacritical Marks for Symbols", 0x0020D0, 0x0020FF}, // 48
{"Letterlike Symbols"                     , 0x002100, 0x00214F}, // 80
{"Number Forms"                           , 0x002150, 0x00218F}, // 64
{"Arrows"                                 , 0x002190, 0x0021FF}, // 112
{"Mathematical Operators"                 , 0x002200, 0x0022FF}, // 256
{"Miscellaneous Technical"                , 0x002300, 0x0023FF}, // 256
{"Control Pictures"                       , 0x002400, 0x00243F}, // 64
{"Optical Character Recognition (OCR)"    , 0x002440, 0x00245F}, // 32
{"Enclosed Alphanumerics"                 , 0x002460, 0x0024FF}, // 160
{"Box Drawing"                            , 0x002500, 0x00257F}, // 128
{"Block Elements"                         , 0x002580, 0x00259F}, // 32
{"Geometric Shapes"                       , 0x0025A0, 0x0025FF}, // 96
{"Miscellaneous Symbols"                  , 0x002600, 0x0026FF}, // 256
{"Dingbats"                               , 0x002700, 0x0027BF}, // 192
{"Miscellaneous Mathematical Symbols-A"   , 0x0027C0, 0x0027EF}, // 48
{"Supplemental Arrows-A"                  , 0x0027F0, 0x0027FF}, // 16
{"Braille Patterns"                       , 0x002800, 0x0028FF}, // 256
{"Supplemental Arrows-B"                  , 0x002900, 0x00297F}, // 128
{"Miscellaneous Mathematical Symbols-B"   , 0x002980, 0x0029FF}, // 128
{"Supplemental Mathematical Operators"    , 0x002A00, 0x002AFF}, // 256
{"Additional Arrows"                      , 0x002B00, 0x002BFF}, // 256
{"Glagolitic"                             , 0x002C00, 0x002C5F}, // 96
{"Latin Extended-C"                       , 0x002C60, 0x002C7F}, // 32
{"Coptic"                                 , 0x002C80, 0x002CFF}, // 128
{"Georgian Supplement"                    , 0x002D00, 0x002D2F}, // 48
{"Tifinagh"                               , 0x002D30, 0x002D7F}, // 80
{"Ethiopic Extended"                      , 0x002D80, 0x002DDF}, // 96
{"Cyrillic Extended-A"                    , 0x002DE0, 0x002DFF}, // 32
{"Supplemental Punctuation"               , 0x002E00, 0x002E7F}, // 128
{"CJK Radicals Supplement"                , 0x002E80, 0x002EFF}, // 128
{"CJK Radicals / Kangxi Radicals"         , 0x002F00, 0x002FDF}, // 224
{"{undefined}"                            , 0x002FE0, 0x002FEF}, // 16
{"Ideographic Description Characters"     , 0x002FF0, 0x002FFF}, // 16
{"CJK Symbols and Punctuation"            , 0x003000, 0x00303F}, // 64
{"Hiragana"                               , 0x003040, 0x00309F}, // 96
{"Katakana"                               , 0x0030A0, 0x0030FF}, // 96
{"Bopomofo"                               , 0x003100, 0x00312F}, // 48
{"Hangul Compatibility Jamo"              , 0x003130, 0x00318F}, // 96
{"Kanbun"                                 , 0x003190, 0x00319F}, // 16
{"Bopomofo Extended"                      , 0x0031A0, 0x0031BF}, // 32
{"CJK Strokes"                            , 0x0031C0, 0x0031EF}, // 48
{"Katakana Phonetic Extensions"           , 0x0031F0, 0x0031FF}, // 16
{"Enclosed CJK Letters and Months"        , 0x003200, 0x0032FF}, // 256
{"CJK Compatibility"                      , 0x003300, 0x0033FF}, // 256
{"CJK  Extension A"                       , 0x003400, 0x004DBF}, // 6592
{"Yijing Hexagram Symbols"                , 0x004DC0, 0x004DFF}, // 64
{"CJK Unified Ideographs (Han)"           , 0x004E00, 0x009FFF}, // 20992
{"Yi Syllables"                           , 0x00A000, 0x00A48F}, // 1168
{"Yi Radicals"                            , 0x00A490, 0x00A4CF}, // 64
{"Lisu"                                   , 0x00A4D0, 0x00A4FF}, // 48
{"Vai"                                    , 0x00A500, 0x00A63F}, // 320
{"Cyrillic Extended-B"                    , 0x00A640, 0x00A69F}, // 96
{"Bamum"                                  , 0x00A6A0, 0x00A6FF}, // 96
{"Modifier Tone Letters"                  , 0x00A700, 0x00A71F}, // 32
{"Latin Extended-D"                       , 0x00A720, 0x00A7FF}, // 224
{"Syloti Nagri"                           , 0x00A800, 0x00A82F}, // 48
{"Common Indic Number Forms"              , 0x00A830, 0x00A83F}, // 16
{"Phags-Pa"                               , 0x00A840, 0x00A87F}, // 64
{"Saurashtra"                             , 0x00A880, 0x00A8DF}, // 96
{"Devanagari Extended"                    , 0x00A8E0, 0x00A8FF}, // 32
{"Kayah Li"                               , 0x00A900, 0x00A92F}, // 48
{"Rejang"                                 , 0x00A930, 0x00A95F}, // 48
{"Hangul Jamo Extended-A"                 , 0x00A960, 0x00A97F}, // 32
{"Javanese"                               , 0x00A980, 0x00A9DF}, // 96
{"Myanmar Extended-B"                     , 0x00A9E0, 0x00A9FF}, // 32
{"Cham"                                   , 0x00AA00, 0x00AA5F}, // 96
{"Myanmar Extended-A"                     , 0x00AA60, 0x00AA7F}, // 32
{"Tai Viet"                               , 0x00AA80, 0x00AADF}, // 96
{"Meetei Mayek Extensions"                , 0x00AAE0, 0x00AAFF}, // 32
{"Ethiopic Extended-A"                    , 0x00AB00, 0x00AB2F}, // 48
{"Latin Extended-E"                       , 0x00AB30, 0x00AB6F}, // 64
{"Cherokee Supplement"                    , 0x00AB70, 0x00ABBF}, // 80
{"Meetei Mayek"                           , 0x00ABC0, 0x00ABFF}, // 64
{"Hangul Syllables"                       , 0x00AC00, 0x00D7AF}, // 11184
{"Hangul Jamo Extended-B"                 , 0x00D7B0, 0x00D7FF}, // 80
{"High Surrogates"                        , 0x00D800, 0x00DBFF}, // 1024
{"Low Surrogates"                         , 0x00DC00, 0x00DFFF}, // 1024
{"Private Use Area"                       , 0x00E000, 0x00F8FF}, // 6400
{"CJK Compatibility Ideographs"           , 0x00F900, 0x00FAFF}, // 512
{"Alphabetic Presentation Forms"          , 0x00FB00, 0x00FB4F}, // 80
{"Arabic Presentation Forms-A"            , 0x00FB50, 0x00FDFF}, // 688
{"Variation Selectors"                    , 0x00FE00, 0x00FE0F}, // 16
{"Vertical Forms"                         , 0x00FE10, 0x00FE1F}, // 16
{"Combining Half Marks"                   , 0x00FE20, 0x00FE2F}, // 16
{"CJK Compatibility Forms"                , 0x00FE30, 0x00FE4F}, // 32
{"Small Form Variants"                    , 0x00FE50, 0x00FE6F}, // 32
{"Arabic Presentation Forms-B"            , 0x00FE70, 0x00FEFF}, // 144
{"Halfwidth and Fullwidth Forms"          , 0x00FF00, 0x00FFEF}, // 240
{"Specials"                               , 0x00FFF0, 0x00FFFF}, // 16
{"Linear B Syllabary"                     , 0x010000, 0x01007F}, // 128
{"Linear B Ideograms"                     , 0x010080, 0x0100FF}, // 128
{"Aegean Numbers"                         , 0x010100, 0x01013F}, // 64
{"Ancient Greek Numbers"                  , 0x010140, 0x01018F}, // 80
{"Roman Symbols"                          , 0x010190, 0x0101CF}, // 64
{"Phaistos Disc"                          , 0x0101D0, 0x0101FF}, // 48
{"{undefined}"                            , 0x010200, 0x01027F}, // 128
{"Lycian"                                 , 0x010280, 0x01029F}, // 32
{"Carian"                                 , 0x0102A0, 0x0102DF}, // 64
{"Coptic Epact Numbers"                   , 0x0102E0, 0x0102FF}, // 32
{"Old Italic"                             , 0x010300, 0x01032F}, // 48
{"Gothic"                                 , 0x010330, 0x01034F}, // 32
{"Old Permic"                             , 0x010350, 0x01037F}, // 48
{"Ugaritic"                               , 0x010380, 0x01039F}, // 32
{"Old Persian"                            , 0x0103A0, 0x0103DF}, // 64
{"{undefined}"                            , 0x0103E0, 0x0103FF}, // 32
{"Deseret"                                , 0x010400, 0x01044F}, // 80
{"Shavian"                                , 0x010450, 0x01047F}, // 48
{"Osmanya"                                , 0x010480, 0x0104AF}, // 48
{"Osage"                                  , 0x0104B0, 0x0104FF}, // 80
{"Elbasan"                                , 0x010500, 0x01052F}, // 48
{"Caucasian Albanian"                     , 0x010530, 0x01056F}, // 64
{"{undefined}"                            , 0x010570, 0x0105FF}, // 144
{"Linear A"                               , 0x010600, 0x01077F}, // 384
{"{undefined}"                            , 0x010780, 0x0107FF}, // 128
{"Cypriot Syllabary"                      , 0x010800, 0x01083F}, // 64
{"Aramaic, Imperial"                      , 0x010840, 0x01085F}, // 32
{"Palmyrene"                              , 0x010860, 0x01087F}, // 32
{"Nabataean"                              , 0x010880, 0x0108AF}, // 48
{"{undefined}"                            , 0x0108B0, 0x0108DF}, // 48
{"Hatran"                                 , 0x0108E0, 0x0108FF}, // 32
{"Phoenician"                             , 0x010900, 0x01091F}, // 32
{"Lydian"                                 , 0x010920, 0x01093F}, // 32
{"{undefined}"                            , 0x010940, 0x01097F}, // 64
{"Meroitic Hieroglyphs"                   , 0x010980, 0x01099F}, // 32
{"Meroitic Cursive"                       , 0x0109A0, 0x0109FF}, // 96
{"Kharoshthi"                             , 0x010A00, 0x010A5F}, // 96
{"Old South Arabian"                      , 0x010A60, 0x010A7F}, // 32
{"Old North Arabian"                      , 0x010A80, 0x010A9F}, // 32
{"{undefined}"                            , 0x010AA0, 0x010ABF}, // 32
{"Manichaean"                             , 0x010AC0, 0x010AFF}, // 64
{"Avestan"                                , 0x010B00, 0x010B3F}, // 64
{"Parthian, Inscriptional"                , 0x010B40, 0x010B5F}, // 32
{"Pahlavi, Inscriptional"                 , 0x010B60, 0x010B7F}, // 32
{"Pahlavi, Psalter"                       , 0x010B80, 0x010BAF}, // 48
{"{undefined}"                            , 0x010BB0, 0x010BFF}, // 80
{"Old Turkic"                             , 0x010C00, 0x010C4F}, // 80
{"{undefined}"                            , 0x010C50, 0x010C7F}, // 48
{"Old Hungarian"                          , 0x010C80, 0x010CFF}, // 128
{"Hanifi Rohingya"                        , 0x010D00, 0x010D3F}, // 64
{"{undefined}"                            , 0x010D40, 0x010E5F}, // 288
{"Rumi Numeral Symbols"                   , 0x010E60, 0x010E7F}, // 32
{"Yezidi"                                 , 0x010E80, 0x010EBF}, // 64
{"{undefined}"                            , 0x010EC0, 0x010EFF}, // 64
{"Old Sogdian"                            , 0x010F00, 0x010F2F}, // 48
{"Sogdian"                                , 0x010F30, 0x010F6F}, // 64
{"{undefined}"                            , 0x010F70, 0x010FAF}, // 64
{"Chorasmian"                             , 0x010FB0, 0x010FDF}, // 48
{"Elymaic"                                , 0x010FE0, 0x010FFF}, // 32
{"Brahmi"                                 , 0x011000, 0x01107F}, // 128
{"Kaithi"                                 , 0x011080, 0x0110CF}, // 80
{"Sora Sompeng"                           , 0x0110D0, 0x0110FF}, // 48
{"Chakma"                                 , 0x011100, 0x01114F}, // 80
{"Mahajani"                               , 0x011150, 0x01117F}, // 48
{"Sharada"                                , 0x011180, 0x0111DF}, // 96
{"Sinhala Archaic Numbers"                , 0x0111E0, 0x0111FF}, // 32
{"Khojki"                                 , 0x011200, 0x01124F}, // 80
{"{undefined}"                            , 0x011250, 0x01127F}, // 48
{"Multani"                                , 0x011280, 0x0112AF}, // 48
{"Khudawadi"                              , 0x0112B0, 0x0112FF}, // 80
{"Grantha"                                , 0x011300, 0x01137F}, // 128
{"{undefined}"                            , 0x011380, 0x0113FF}, // 128
{"Newa"                                   , 0x011400, 0x01147F}, // 128
{"Tirhuta"                                , 0x011480, 0x0114DF}, // 96
{"{undefined}"                            , 0x0114E0, 0x01157F}, // 160
{"Siddham"                                , 0x011580, 0x0115FF}, // 128
{"Modi"                                   , 0x011600, 0x01165F}, // 96
{"Mongolian Supplement"                   , 0x011660, 0x01167F}, // 32
{"Takri"                                  , 0x011680, 0x0116CF}, // 80
{"{undefined}"                            , 0x0116D0, 0x0116FF}, // 48
{"Ahom"                                   , 0x011700, 0x01173F}, // 64
{"{undefined}"                            , 0x011740, 0x0117FF}, // 192
{"Dogra"                                  , 0x011800, 0x01184F}, // 80
{"{undefined}"                            , 0x011850, 0x01189F}, // 80
{"Warang Citi"                            , 0x0118A0, 0x0118FF}, // 96
{"Dives Akuru"                            , 0x011900, 0x01195F}, // 96
{"{undefined}"                            , 0x011960, 0x01199F}, // 64
{"Nandinagari"                            , 0x0119A0, 0x0119FF}, // 96
{"Zanabazar Square"                       , 0x011A00, 0x011A4F}, // 80
{"Soyombo"                                , 0x011A50, 0x011AAF}, // 96
{"{undefined}"                            , 0x011AB0, 0x011ABF}, // 16
{"Pau Cin Hau"                            , 0x011AC0, 0x011AFF}, // 64
{"{undefined}"                            , 0x011B00, 0x011BFF}, // 256
{"Bhaiksuki"                              , 0x011C00, 0x011C6F}, // 112
{"Marchen"                                , 0x011C70, 0x011CBF}, // 80
{"{undefined}"                            , 0x011CC0, 0x011CFF}, // 64
{"Masaram Gondi"                          , 0x011D00, 0x011D5F}, // 96
{"Gunjala Gondi"                          , 0x011D60, 0x011DAF}, // 80
{"{undefined}"                            , 0x011DB0, 0x011EDF}, // 304
{"Makasar"                                , 0x011EE0, 0x011EFF}, // 32
{"{undefined}"                            , 0x011F00, 0x011FAF}, // 176
{"Lisu Supplement"                        , 0x011FB0, 0x011FBF}, // 16
{"Tamil Supplement"                       , 0x011FC0, 0x011FFF}, // 64
{"Cuneiform"                              , 0x012000, 0x0123FF}, // 1024
{"Cuneiform Numbers and Punctuation"      , 0x012400, 0x01247F}, // 128
{"Early Dynastic Cuneiform"               , 0x012480, 0x01254F}, // 208
{"{undefined}"                            , 0x012550, 0x012FFF}, // 2736
{"Egyptian Hieroglyphs"                   , 0x013000, 0x01342F}, // 1072
{"Egyptian Hieroglyph Format Controls"    , 0x013430, 0x01343F}, // 16
{"{undefined}"                            , 0x013440, 0x0143FF}, // 4032
{"Anatolian Hieroglyphs"                  , 0x014400, 0x01467F}, // 640
{"{undefined}"                            , 0x014680, 0x0167FF}, // 8576
{"Bamum Supplement"                       , 0x016800, 0x016A3F}, // 576
{"Mro"                                    , 0x016A40, 0x016A6F}, // 48
{"{undefined}"                            , 0x016A70, 0x016ACF}, // 96
{"Bassa Vah"                              , 0x016AD0, 0x016AFF}, // 48
{"Pahawh Hmong"                           , 0x016B00, 0x016B8F}, // 144
{"{undefined}"                            , 0x016B90, 0x016E3F}, // 688
{"Medefaidrin"                            , 0x016E40, 0x016E9F}, // 96
{"{undefined}"                            , 0x016EA0, 0x016EFF}, // 96
{"Miao"                                   , 0x016F00, 0x016F9F}, // 160
{"{undefined}"                            , 0x016FA0, 0x016FDF}, // 64
{"Ideographic Symbols and Punctuation"    , 0x016FE0, 0x016FFF}, // 32
{"Tangut"                                 , 0x017000, 0x0187FF}, // 6144
{"Tangut Components"                      , 0x018800, 0x018AFF}, // 768
{"Khitan Small Script"                    , 0x018B00, 0x018CFF}, // 512
{"Tangut Supplement"                      , 0x018D00, 0x018D08}, // 9
{"{undefined}"                            , 0x018D09, 0x01AFFF}, // 8951
{"Kana Supplement"                        , 0x01B000, 0x01B0FF}, // 256
{"Kana Extended-A"                        , 0x01B100, 0x01B12F}, // 48
{"Small Kana Extension"                   , 0x01B130, 0x01B16F}, // 64
{"Nushu"                                  , 0x01B170, 0x01B2FF}, // 400
{"{undefined}"                            , 0x01B300, 0x01BBFF}, // 2304
{"Duployan"                               , 0x01BC00, 0x01BC9F}, // 160
{"Shorthand Format Controls"              , 0x01BCA0, 0x01BCAF}, // 16
{"{undefined}"                            , 0x01BCB0, 0x01CFFF}, // 4944
{"Byzantine Musical Symbols"              , 0x01D000, 0x01D0FF}, // 256
{"Musical Symbols"                        , 0x01D100, 0x01D1FF}, // 256
{"Ancient Greek Musical Notation"         , 0x01D200, 0x01D24F}, // 80
{"{undefined}"                            , 0x01D250, 0x01D2DF}, // 144
{"Mayan Numerals"                         , 0x01D2E0, 0x01D2FF}, // 32
{"Tai Xuan Jing Symbols"                  , 0x01D300, 0x01D35F}, // 96
{"Counting Rod Numerals"                  , 0x01D360, 0x01D37F}, // 32
{"{undefined}"                            , 0x01D380, 0x01D3FF}, // 128
{"Mathematical Alphanumeric Symbols"      , 0x01D400, 0x01D7FF}, // 1024
{"Sutton SignWriting"                     , 0x01D800, 0x01DAAF}, // 688
{"{undefined}"                            , 0x01DAB0, 0x01DFFF}, // 1360
{"Glagolitic Supplement"                  , 0x01E000, 0x01E02F}, // 48
{"{undefined}"                            , 0x01E030, 0x01E0FF}, // 208
{"Nyiakeng Puachue Hmong"                 , 0x01E100, 0x01E14F}, // 80
{"{undefined}"                            , 0x01E150, 0x01E2BF}, // 368
{"Wancho"                                 , 0x01E2C0, 0x01E2FF}, // 64
{"{undefined}"                            , 0x01E300, 0x01E7FF}, // 1280
{"Mende Kikakui"                          , 0x01E800, 0x01E8DF}, // 224
{"{undefined}"                            , 0x01E8E0, 0x01E8FF}, // 32
{"Adlam"                                  , 0x01E900, 0x01E95F}, // 96
{"{undefined}"                            , 0x01E960, 0x01EC6F}, // 784
{"Indic Siyaq Numbers"                    , 0x01EC70, 0x01ECBF}, // 80
{"{undefined}"                            , 0x01ECC0, 0x01ECFF}, // 64
{"Ottoman Siyaq Numbers"                  , 0x01ED00, 0x01ED4F}, // 80
{"{undefined}"                            , 0x01ED50, 0x01EDFF}, // 176
{"Arabic Mathematical Alphabetic Symbols" , 0x01EE00, 0x01EEFF}, // 256
{"{undefined}"                            , 0x01EF00, 0x01EFFF}, // 256
{"Mahjong Tiles"                          , 0x01F000, 0x01F02F}, // 48
{"Domino Tiles"                           , 0x01F030, 0x01F09F}, // 112
{"Playing Cards"                          , 0x01F0A0, 0x01F0FF}, // 96
{"Enclosed Alphanumeric Supplement"       , 0x01F100, 0x01F1FF}, // 256
{"Enclosed Ideographic Supplement"        , 0x01F200, 0x01F2FF}, // 256
{"Miscellaneous Symbols And Pictographs"  , 0x01F300, 0x01F5FF}, // 768
{"Emoticons"                              , 0x01F600, 0x01F64F}, // 80
{"Ornamental Dingbats"                    , 0x01F650, 0x01F67F}, // 48
{"Transport and Map Symbols"              , 0x01F680, 0x01F6FF}, // 128
{"Alchemical Symbols"                     , 0x01F700, 0x01F77F}, // 128
{"Geometric Shapes Extended"              , 0x01F780, 0x01F7FF}, // 128
{"Supplemental Arrows-C"                  , 0x01F800, 0x01F8FF}, // 256
{"Supplemental Symbols and Pictographs"   , 0x01F900, 0x01F9FF}, // 256
{"Chess Symbols"                          , 0x01FA00, 0x01FA6F}, // 112
{"Symbols and Pictographs Extended-A"     , 0x01FA70, 0x01FAFF}, // 144
{"Symbols for Legacy Computing"           , 0x01FB00, 0x01FBFF}, // 256
{"{undefined}"                            , 0x01FC00, 0x01FFFF}, // 1024
{"CJK Extension B"                        , 0x020000, 0x02A6DF}, // 42720
{"{undefined}"                            , 0x02A6E0, 0x02A6FF}, // 32
{"CJK Extension C"                        , 0x02A700, 0x02B73F}, // 4160
{"CJK Extension D"                        , 0x02B740, 0x02B81F}, // 224
{"CJK Extension E"                        , 0x02B820, 0x02CEAF}, // 5776
{"CJK Extension F"                        , 0x02CEB0, 0x02EBE0}, // 7473
{"{undefined}"                            , 0x02EBE1, 0x02F7FF}, // 3103
{"CJK Compatibility Ideographs Supplement", 0x02F800, 0x02FA1F}, // 544
{"{undefined}"                            , 0x02FA20, 0x02FFFF}, // 1504
{"CJK Extension G"                        , 0x030000, 0x03134A}, // 4939
{"{undefined}"                            , 0x03134B, 0x0DFFFF}, // 715957
{"Tags"                                   , 0x0E0000, 0x0E007F}, // 128
{"{undefined}"                            , 0x0E0080, 0x0E00FF}, // 128
{"Variation Selectors Supplement"         , 0x0E0100, 0x0E01EF}, // 240
{"{undefined}"                            , 0x0E01F0, 0x0EFFFF}, // 65040
{"Supplementary Private Use Area-A"       , 0x0F0000, 0x0FFFFD}, // 65534
{"{undefined}"                            , 0x0FFFFE, 0x0FFFFF}, // 2
{"Supplementary Private Use Area-B"       , 0x100000, 0x10FFFD}, // 65534
};

const int numUnicodeSubsets = sizeof(UnicodeSubsets)/sizeof(UnicodeSubset_t);

int GetSubsetFromChar(unsigned int chr)
{
	for( int n = 0; n < numUnicodeSubsets; n++ )
	{
		if( chr >= (unsigned)UnicodeSubsets[n].beginChar && chr <= (unsigned)UnicodeSubsets[n].endChar )
			return n;
	}

	return -1;
}

string GetCharSetName(int charSet)
{
	string str;

	switch( charSet )
	{
	case ANSI_CHARSET:
		str = "ANSI";
		break;
	case DEFAULT_CHARSET:
		str = "DEFAULT";
		break;
	case SYMBOL_CHARSET:
		str = "SYMBOL";
		break;
	case SHIFTJIS_CHARSET:
		str = "SHIFTJIS";
		break;
	case HANGUL_CHARSET:
		str = "HANGUL";
		break;
	case GB2312_CHARSET:
		str = "GB2312";
		break;
	case CHINESEBIG5_CHARSET:
		str = "CHINESEBIG5";
		break;
	case OEM_CHARSET:
		str = "OEM";
		break;
	case 130: // JOHAB_CHARSET
		str = "JOHAB";
		break;
	case 177: // HEBREW_CHARSET
		str = "HEBREW";
		break;
	case 178: // ARABIC_CHARSET
		str = "ARABIC";
		break;
	case 161: // GREEK_CHARSET
		str = "GREEK";
		break;
	case 162: // TURKISH_CHARSET
		str = "TURKISH";
		break;
	case 163: // VIETNAMESE_CHARSET
		str = "VIETNAMESE";
		break;
	case 222: // THAI_CHARSET
		str = "THAI";
		break;
	case 238: // EASTEUROPE_CHARSET
		str = "EASTEUROPE";
		break;
	case 204: // RUSSIAN_CHARSET
		str = "RUSSIAN";
		break;
	case 77:  // MAC_CHARSET
		str = "MAC";
		break;
	case 186: // BALTIC_CHARSET
		str = "BALTIC";
		break;

	default:
		str = acStringFormat("%d", charSet);
	}
	
	return str;
}

int GetCharSet(const char *charSetName)
{
	string str = charSetName;
	int set = 0;

	if( str == "ANSI" )             set = 0;
	else if( str == "DEFAULT" )     set = 1;
	else if( str == "SYMBOL" )      set = 2;
	else if( str == "SHIFTJIS" )    set = 128;
	else if( str == "HANGUL" )      set = 129;
	else if( str == "GB2312" )      set = 134;
	else if( str == "CHINESEBIG5" ) set = 136;
	else if( str == "OEM" )         set = 255;
	else if( str == "JOHAB" )       set = 130;
	else if( str == "HEBREW" )      set = 177;
	else if( str == "ARABIC" )      set = 178;
	else if( str == "GREEK" )       set = 161;
	else if( str == "TURKISH" )     set = 162;
	else if( str == "VIETNAMESE" )  set = 163;
	else if( str == "THAI" )        set = 222;
	else if( str == "EASTEUROPE" )  set = 238;
	else if( str == "RUSSIAN" )     set = 204;
	else if( str == "MAC" )         set = 77;
	else if( str == "BALTIC" )      set = 186;
	else set = acStringScanInt(charSetName, 10, 0);

	return set;
}

int EnumUnicodeGlyphs(HDC dc, map<unsigned int, unsigned int> &unicodeToGlyphMap)
{
	unicodeToGlyphMap.clear();

	// This function is only called as fallback in case EnumTrueTypeCMAP didn't
	// work. so we can assume that the font doesn't have 32bit Unicode support
	for (unsigned int ch = 0; ch < 0xFFFF; ch++)
	{
		// Skip the range for surrogate pairs
		if (ch == 0xD800)
		{
			ch = 0xDFFF;
			continue;
		}

		WCHAR buf[] = { WCHAR(ch) };
		WORD idx; 
		int r = fGetGlyphIndicesW(dc, buf, 1, &idx, GGI_MARK_NONEXISTING_GLYPHS);
		if (r != GDI_ERROR && idx != 0xFFFF && idx != 0)
			unicodeToGlyphMap[ch] = idx;
	}

	return 0;
}

/*
// I've retired this code. Instead the EnumTrueTypeCMAP or EnumUnicodeGlyphs shall be used
// to map all the existing glyphs in one pass, and then the lookup is done using that map.
// The ScriptShape wasn't very reliable anyway, and returned a lot of characters as existing
// even though they didn't.
int GetUnicodeGlyphIndex(HDC dc, SCRIPT_CACHE *sc, UINT ch)
{
	SCRIPT_CACHE mySc = 0;
	if( sc == 0 ) sc = &mySc;

	if( ch < 0x10000 )
	{
		// GetGlyphIndices seems to work better than ScriptShape in 
		// the base plane. It reports less missing characters as existing
		WCHAR buf[] = {WCHAR(ch)};
		WORD idx;
		int r = fGetGlyphIndicesW(dc, buf, 1, &idx, GGI_MARK_NONEXISTING_GLYPHS);
		if( r == GDI_ERROR || idx == 0xFFFF )
			return -1;

		return idx;
	}

	// Convert the unicode character to a UTF16 encoded 
	// buffer that Uniscribe understands
	WCHAR buf[2];
	int length = acUtility::EncodeUTF16(ch, (unsigned char*)buf, 0);

	// Call ScriptItemize to analyze the unicode string 
	// to find it's logical pieces
	SCRIPT_ITEM items[2];
	int nitems;
	HRESULT hr;
	hr = ScriptItemize(buf, length/2, 2, 0, 0, items, &nitems);
	if( FAILED(hr) )
		return -1;

	// Call ScriptShape to determine the glyphs that will 
	// be used to render each character
	WORD glyphs[10];
	WORD cluster[10];
	int nglyphs;
	SCRIPT_VISATTR attr[10];
	hr = ScriptShape(dc, sc, &buf[0], length/2, 10, &items[0].a, glyphs, cluster, attr, &nglyphs);
	if( FAILED(hr) )
		return -1;

	if( mySc ) 
		ScriptFreeCache(&mySc);

	// Was the glyph found?
	return glyphs[0];
}
*/
/*
int DoesUnicodeCharExist(HDC dc, SCRIPT_CACHE *sc, UINT ch)
{
	int idx = GetUnicodeGlyphIndex(dc, sc, ch);
	
	if( idx < 0 ) 
		return 0;

	SCRIPT_FONTPROPERTIES props;
	props.cBytes = sizeof(props);
	ScriptGetFontProperties(dc, sc, &props);	
	
	if( idx != props.wgDefault )
		return 1;

	return 0;
}
*/

int GetGlyphABCWidths(HDC dc, SCRIPT_CACHE *sc, UINT glyph, ABC *abc)
{
	SCRIPT_CACHE mySc = 0;
	if( sc == 0 ) sc = &mySc;

	HRESULT hr = ScriptGetGlyphABCWidth(dc, sc, glyph, abc);
	if( FAILED(hr) )
	{
		if( mySc ) ScriptFreeCache(&mySc);
		return -1;
	}

	if( mySc ) ScriptFreeCache(&mySc);
	return 0;
}

//=================================================================================
// The functions below are all for extracting kerning data from the GPOS table
//
// Reference: http://www.microsoft.com/typography/otspec/gpos.htm
//            http://www.microsoft.com/typography/otspec/otff.htm
//            http://partners.adobe.com/public/developer/opentype/index_table_formats2.html
//

UINT GetClassFromClassDef(BYTE *classDef, WORD glyphId)
{
	// Go through the class def to determine in which class the glyph belongs
	WORD classFormat = GETUSHORT(classDef);
	if( classFormat == 1 )
	{
		WORD startGlyph = GETUSHORT(classDef+2);
		WORD glyphCount = GETUSHORT(classDef+4);

		if( startGlyph <= glyphId && glyphId - startGlyph < glyphCount )
			return GETUSHORT(classDef+6+2*(glyphId - startGlyph));
	}
	else if( classFormat == 2 )
	{
		WORD rangeCount = GETUSHORT(classDef+2);
		for( UINT n = 0; n < rangeCount; n++ )
		{
			WORD start = GETUSHORT(classDef+4+6*n);
			WORD end   = GETUSHORT(classDef+6+6*n);
			if( start <= glyphId && end >= glyphId )
				return GETUSHORT(classDef+8+6*n);
		}
	}

	return 0;
}

vector<UINT> GetGlyphsFromClassDef(BYTE *classDef, UINT classId)
{
	// Find the class, and return all the glyphs that are part of it
	vector<UINT> glyphs;

	WORD classFormat = GETUSHORT(classDef);
	if( classFormat == 1 )
	{
		WORD startGlyph = GETUSHORT(classDef+2);
		WORD glyphCount = GETUSHORT(classDef+4);

		for( UINT n = 0; n < glyphCount; n++ )
		{
			if( GETUSHORT(classDef+6+2*n) == classId )
				glyphs.push_back(startGlyph + n);
		}
	}
	else if( classFormat == 2 )
	{
		WORD rangeCount = GETUSHORT(classDef+2);
		for( UINT n = 0; n < rangeCount; n++ )
		{
			WORD start = GETUSHORT(classDef+4+6*n);
			WORD end   = GETUSHORT(classDef+6+6*n);
			if( GETUSHORT(classDef+8+6*n) == classId )
			{
				for( UINT g = start; g <= end; g++ )
					glyphs.push_back(g);
			}
		}
	}

	return glyphs;
}

float DetermineDesignUnitToFontUnitFactor(HDC dc)
{
	OUTLINETEXTMETRIC tm;
	GetOutlineTextMetrics(dc, sizeof(tm), &tm);

	DWORD head = TAG('h','e','a','d');
	UINT size = GetFontData(dc, head, 0, 0, 0);
	if( size != GDI_ERROR )
	{
		vector<BYTE> buffer;
		buffer.resize(size);
		GetFontData(dc, head, 0, &buffer[0], size);

		SHORT xMin = GETUSHORT(&buffer[36]);
		SHORT yMin = GETUSHORT(&buffer[38]);
		SHORT xMax = GETUSHORT(&buffer[40]);
		SHORT yMax = GETUSHORT(&buffer[42]);

		float factor = float(tm.otmrcFontBox.top-tm.otmrcFontBox.bottom)/float(yMax-yMin);
		return factor;
	}

	return 1;
}

void AddKerningPairToList(HDC dc, UINT glyphId1, UINT glyphId2, int kerning, vector<KERNINGPAIR> &pairs, float scaleFactor, map<UINT,vector<UINT>> &glyphIdToChar)
{
	assert(kerning != 0);

	for( UINT a = 0; a < glyphIdToChar[glyphId1].size(); a++ )
	{
		for( UINT b = 0; b < glyphIdToChar[glyphId2].size(); b++ )
		{
			// Add the kerning pair to the list
			KERNINGPAIR pair;
			pair.wFirst      = glyphIdToChar[glyphId1][a];
			pair.wSecond     = glyphIdToChar[glyphId2][b];
			if( pair.wFirst == 0 || pair.wSecond == 0 )
				return;

			// Convert from design units to the selected font size
			float kern = kerning*scaleFactor;
			if( kern < 0 )
				pair.iKernAmount = int(kern-0.5f);
			else
				pair.iKernAmount = int(kern+0.5f);

			// Skip 0 kernings
			if( pair.iKernAmount == 0 )
				return;

			if( pairs.capacity() == pairs.size() && pairs.size() > 1 )
				pairs.reserve(pairs.size() * 2);
			pairs.push_back(pair);
		}
	}
}

UINT GetSizeOfValueType(WORD valueType)
{
	UINT size = 0;
	// TODO: Are these the only flags?
	if( valueType & 1 ) // x placement
		size += 2;
	if( valueType & 2 ) // y placement
		size += 2;
	if( valueType & 4 ) // x advance
		size += 2;
	if( valueType & 8 ) // y advance
		size += 2;
	if( valueType & 16 ) // offset to device x placement
		size += 2;
	if( valueType & 32 ) // offset to device y placement
		size += 2;
	if( valueType & 64 ) // offset to device x advance
		size += 2;
	if( valueType & 128 ) // offset to device y advance
		size += 2;
	return size;
}

short GetXAdvance(BYTE *value, WORD valueType)
{
	if( !(valueType & 4) ) 
		return 0;

	UINT offset = 0;
	if( valueType & 1 ) offset += 2;
	if( valueType & 2 ) offset += 2;

	return GETSHORT(value+offset);
}

void ProcessPairAdjustmentFormat1(HDC dc, BYTE *subTable, vector<KERNINGPAIR> &pairs, map<UINT,vector<UINT>> &glyphIdToChar, float scaleFactor)
{
	// Defines kerning between two individual glyphs

	WORD coverageOffset = GETUSHORT(subTable+2);
	WORD valueFormat1   = GETUSHORT(subTable+4);
	assert( valueFormat1 == 4 ); // TODO: Must support others
	WORD valueFormat2   = GETUSHORT(subTable+6);
	assert( valueFormat2 == 0 ); // TODO: Must support others
	WORD pairSetCount   = GETUSHORT(subTable+8);

	UINT valuePairSize = GetSizeOfValueType(valueFormat1) + GetSizeOfValueType(valueFormat2);

	// The first glyph id in the pair is found in the coverage table
	// The second glyph id in the pair is found in the PairSet records
	
	BYTE *coverage = subTable + coverageOffset;
	WORD coverageFormat = GETUSHORT(coverage);
	if( coverageFormat == 1 )
	{
		WORD glyphCount = GETUSHORT(coverage+2);
		assert(glyphCount == pairSetCount);
		for( DWORD g = 0; g < glyphCount; g++ )
		{
			WORD glyphId1 = GETUSHORT(coverage+4+2*g);
			if( glyphIdToChar[glyphId1].size() == 0 )
				continue;

			// For each of the glyph ids we need to search the 
			// PairSets for the matching kerning pairs
			WORD pairSetOffset = GETUSHORT(subTable+10+g*2);
			BYTE *pairSet = subTable + pairSetOffset;
			WORD pairValueCount = GETUSHORT(pairSet);
			for( UINT p = 0; p < pairValueCount; p++ )
			{
				BYTE *pairValue = pairSet + 2 + p*(2+valuePairSize);

				WORD glyphId2 = GETUSHORT(pairValue);
				short xAdv1 = GetXAdvance(pairValue+2, valueFormat1);

				if( xAdv1 != 0 )
				{
					AddKerningPairToList(dc, glyphId1, glyphId2, xAdv1, pairs, scaleFactor, glyphIdToChar);
				}
			}
		}
	}
	else
	{
		WORD rangeCount = GETUSHORT(coverage+2);

		for (UINT n = 0; n < rangeCount; n++)
		{
			WORD start = GETUSHORT(coverage + 4 + n * 6);
			WORD end = GETUSHORT(coverage + 6 + n * 6);
			WORD startCoverageIndex = GETUSHORT(coverage + 8 + n * 6);

			for (UINT g = start; g <= end; g++)
			{
				WORD glyphId1 = g;
				if (glyphIdToChar[glyphId1].size() == 0)
					continue;
				
				// For each of the glyph ids we need to search the 
				// PairSets for the matching kerning pairs
				WORD pairSetOffset = GETUSHORT(subTable + 10 + (g-start+startCoverageIndex) * 2);
				BYTE* pairSet = subTable + pairSetOffset;
				WORD pairValueCount = GETUSHORT(pairSet);
				for (UINT p = 0; p < pairValueCount; p++)
				{
					BYTE* pairValue = pairSet + 2 + p * (2 + valuePairSize);

					WORD glyphId2 = GETUSHORT(pairValue);
					short xAdv1 = GetXAdvance(pairValue + 2, valueFormat1);

					if (xAdv1 != 0)
					{
						AddKerningPairToList(dc, glyphId1, glyphId2, xAdv1, pairs, scaleFactor, glyphIdToChar);
					}
				}
			}
		}
	}
}

void ProcessPairAdjustmentFormat2(HDC dc, BYTE *subTable, vector<KERNINGPAIR> &pairs, map<UINT,vector<UINT>> &glyphIdToChar, float scaleFactor)
{
	// Defines kerning between two classes of glyphs

	WORD coverageOffset  = GETUSHORT(subTable+2);
	WORD valueFormat1    = GETUSHORT(subTable+4);
	assert( valueFormat1 == 4 ); // TODO: must support others
	WORD valueFormat2    = GETUSHORT(subTable+6);
	assert( valueFormat2 == 0 ); // TODO: must support others
	WORD classDefOffset1 = GETUSHORT(subTable+8);
	WORD classDefOffset2 = GETUSHORT(subTable+10);
	WORD classCount1     = GETUSHORT(subTable+12);
	WORD classCount2     = GETUSHORT(subTable+14);

	WORD valuePairSize = GetSizeOfValueType(valueFormat1) + GetSizeOfValueType(valueFormat2);

	// The first glyph id in the pair is found in the coverage table
	// The second glyph id is determined from the class definitions

	vector<WORD> glyph1;

	BYTE *coverage = subTable + coverageOffset;
	WORD coverageFormat = GETUSHORT(coverage);
	if( coverageFormat == 1 )
	{
		WORD glyphCount = GETUSHORT(coverage+2);
		glyph1.reserve(glyphCount);

		for( DWORD g = 0; g < glyphCount; g++ )
		{
			WORD glyphId = GETUSHORT(coverage+4+2*g);
			if( glyphIdToChar[glyphId].size() )
				glyph1.push_back(glyphId);
		}
	}
	else
	{
		WORD rangeCount = SWAP16(*(WORD*)(coverage+2));

		// Expand the ranges into the glyph1 array
		for( UINT n = 0; n < rangeCount; n++ )
		{
			WORD start = GETUSHORT(coverage+4+n*6);
			WORD end   = GETUSHORT(coverage+6+n*6);
			WORD startCoverageIndex = GETUSHORT(coverage+8+n*6);

			// TODO: Reserve space for all the glyph ids to reduce number of individual allocations

			for( UINT g = start; g <= end; g++ )
			{
				if( glyphIdToChar[g].size() )
					glyph1.push_back(g);
			}
		}
	}

	for( UINT g = 0; g < glyph1.size(); g++ )
	{
		// What class is this glyph?
		// Need a function for obtaining the class from a ClassDef
		UINT c1 = GetClassFromClassDef(subTable + classDefOffset1, glyph1[g]);

		assert( c1 < classCount1 );
	
		if( c1 < classCount1 )
		{
			BYTE *c1List = subTable + 16 + c1*classCount2*valuePairSize;

			// For each of the classes 
			for( UINT c2 = 0; c2 < classCount2; c2++ )
			{
				// Enumerate the glyphs that are part of the classes
				vector<UINT> glyph2 = GetGlyphsFromClassDef(subTable + classDefOffset2, c2);

				BYTE *valuePair = c1List + valuePairSize*c2;

				short xAdv1 = GetXAdvance(valuePair, valueFormat1);
				if( xAdv1 != 0 )
				{
					// Add a kerning pair for each combination of glyphs in each of the classes
					for( UINT n = 0; n < glyph2.size(); n++ )
					{
						AddKerningPairToList(dc, glyph1[g], glyph2[n], xAdv1, pairs, scaleFactor, glyphIdToChar);
					}
				}
			}
		}
	}
}

void ProcessKernFeature(HDC dc, BYTE *featureRecord, BYTE *featureList, BYTE *lookupList, vector<KERNINGPAIR> &pairs, map<UINT,vector<UINT>> &glyphIdToChar, float scaleFactor)
{
	WORD offset = GETUSHORT(featureRecord+4);

	BYTE *feature = featureList + offset;
	WORD featureParams = GETUSHORT(feature);
	WORD lookupCount = GETUSHORT(feature+2);
	WORD allLookupCount = GETUSHORT(lookupList);
	
	for( DWORD i = 0; i < lookupCount; i++ )
	{
		WORD lookupIndex = GETUSHORT(feature+4+i*2);

		// Determine the features that apply (look for kerning pairs)

		// Find the adjustments in the lookup table
		if( lookupIndex < allLookupCount )
		{
			WORD lookupOffset = GETUSHORT(lookupList + 2 + lookupIndex*2);

			BYTE *lookup = lookupList + lookupOffset;

			WORD lookupType = GETUSHORT(lookup);
			WORD lookupFlag = GETUSHORT(lookup+2);
			WORD subTableCount = GETUSHORT(lookup+4);

			for( UINT s = 0; s < subTableCount; s++ )
			{
				WORD offset = GETUSHORT(lookup + 6 + s*2);
				BYTE *subTable = lookup + offset;

				WORD realLookupType = lookupType;

				if( lookupType == 9 ) // extension positioning
				{
					WORD posFormat = GETUSHORT(subTable);
					assert( posFormat == 1 ); // reserved
					WORD extensionLookupType = GETUSHORT(subTable+2);
					DWORD extensionOffset = GETUINT(subTable+4);

					realLookupType = extensionLookupType;
					subTable = subTable + extensionOffset;
				}

				if( realLookupType == 2 ) // pair adjustment
				{
					WORD posFormat    = GETUSHORT(subTable);
					if( posFormat == 1 )
						ProcessPairAdjustmentFormat1(dc, subTable, pairs, glyphIdToChar, scaleFactor);
					else if( posFormat == 2 )
						ProcessPairAdjustmentFormat2(dc, subTable, pairs, glyphIdToChar, scaleFactor);
					else
						assert(false);
				}
			}
		}
	}
}

void GetKerningPairsFromGPOS(HDC dc, vector<KERNINGPAIR> &pairs, vector<UINT> &chars, const CFontGen *gen)
{
	// Determine the factor for scaling down the values from the design units to the font size
	float scaleFactor = DetermineDesignUnitToFontUnitFactor(dc);

	// TODO: support non unicode as well
	// Build a glyphId to char map. Multiple characters may use  
	// the same glyph, e.g. space, 32, and hard space, 160.
	map<UINT,vector<UINT>> glyphIdToChar;
	for( UINT n = 0; n < chars.size(); n++ )
	{
		int glyphId = gen->GetUnicodeGlyph(chars[n]);
		if( glyphId >= 0 )
			glyphIdToChar[glyphId].push_back(chars[n]);
	}

	// Load the GPOS table from the TrueType font file
	vector<BYTE> buffer;
	DWORD GPOS = TAG('G','P','O','S');
	DWORD size = GetFontData(dc, GPOS, 0, 0, 0);
	if( size != GDI_ERROR )
	{
		buffer.resize(size);
		size = GetFontData(dc, GPOS, 0, &buffer[0], size);
	}
	if( size == GDI_ERROR || size == 0 )
		return;

	// Get the GPOS header info
	DWORD version          = GETUINT(&buffer[0]);
	assert( version == 0x00010000 );
	WORD scriptListOffset  = GETUSHORT(&buffer[4]);
	WORD featureListOffset = GETUSHORT(&buffer[6]);
	WORD lookupListOffset  = GETUSHORT(&buffer[8]);

	BYTE *scriptList  = &buffer[0] + scriptListOffset;
	BYTE *featureList = &buffer[0] + featureListOffset;
	BYTE *lookupList  = &buffer[0] + lookupListOffset;

	// Locate the default script in the script list table
	WORD scriptCount = GETUSHORT(scriptList);
	WORD offset = 0;
	for( UINT c = 0; c < scriptCount; c++ )
	{
		BYTE *scriptRecord = scriptList + 2 + c*6;
		DWORD tag = *(DWORD*)scriptRecord;
		if( tag == TAG('D','F','L','T') )
		{
			offset = GETUSHORT(scriptRecord+4);
			break;
		}
	}

	if( offset == 0 )
		return;

	// Use the default language
	BYTE *script = scriptList + offset;
	WORD defaultLangSysOffset = GETUSHORT(script);
	WORD langSysCount = GETUSHORT(script+2);
	if( defaultLangSysOffset == 0 )
		return;

	BYTE *langSys = script + defaultLangSysOffset;
		
	WORD lookupOrder = GETUSHORT(langSys);
	assert( lookupOrder == 0 ); // reserved for future use
	WORD reqFeatureIndex = GETUSHORT(langSys+2); // Can be 0xFFFF if not used
	WORD featureCount    = GETUSHORT(langSys+4);

	// Find all kerning pairs from all the features that apply
	WORD allFeatureCount = GETUSHORT(featureList);
	for( UINT c = 0; c < featureCount; c++ )
	{
		WORD featureIndex = GETUSHORT(langSys+6+c*2);

		if( featureIndex < allFeatureCount )
		{
			BYTE *featureRecord = featureList + 2 + 6*featureIndex;

			DWORD tag = *(DWORD*)(featureRecord);
			if( tag == TAG('k','e','r','n') )
			{
				ProcessKernFeature(dc, featureRecord, featureList, lookupList, pairs, glyphIdToChar, scaleFactor);
			}
		}
	}
}

//=================================================================================
// The functions below are all for extracting kerning data from the KERN table
//
// Reference: http://www.microsoft.com/typography/otspec/kern.htm
//            http://www.microsoft.com/typography/otspec/otff.htm
//            http://partners.adobe.com/public/developer/opentype/index_table_formats2.html
//


void GetKerningPairsFromKERN(HDC dc, vector<KERNINGPAIR> &pairs, vector<UINT> &chars, const CFontGen *gen)
{
	// Determine the factor for scaling down the values from the design units to the font size
	float scaleFactor = DetermineDesignUnitToFontUnitFactor(dc);

	// TODO: support non unicode as well
	// Build a glyphId to char map. Multiple characters may use  
	// the same glyph, e.g. space, 32, and hard space, 160.
	map<UINT,vector<UINT>> glyphIdToChar;
	for( UINT n = 0; n < chars.size(); n++ )
	{
		int glyphId = gen->GetUnicodeGlyph(chars[n]);
		if( glyphId >= 0 )
			glyphIdToChar[glyphId].push_back(chars[n]);
	}

	// Load the KERN table from the TrueType font file
	vector<BYTE> buffer;
	DWORD KERN = TAG('k','e','r','n');
	DWORD size = GetFontData(dc, KERN, 0, 0, 0);
	if( size != GDI_ERROR )
	{
		buffer.resize(size);
		size = GetFontData(dc, KERN, 0, &buffer[0], size);
	}
	if( size == GDI_ERROR || size == 0 )
		return;

	// Get the KERN header info
	WORD version = GETUSHORT(&buffer[0]);
	assert( version == 0x0000 );
	WORD nTables = GETUSHORT(&buffer[2]);

	UINT pos = 4;
	for( unsigned int n = 0; n < nTables; n++ )
	{
		WORD version  = GETUSHORT(&buffer[pos+0]);
		assert( version == 0x0000 );
		WORD length   = GETUSHORT(&buffer[pos+2]);
		WORD coverage = GETUSHORT(&buffer[pos+4]);

		// We currently only support horizontal text, and don't care about vertical adjustments 
		if( (coverage & 1) == 1 && (coverage & 4) == 0 )
		{
			// Check if this table holds kerning values
			if( (coverage & 2) == 0 )
			{
				BYTE format = (coverage>>8);
				if( format == 0 )
				{
					WORD nPairs        = GETUSHORT(&buffer[pos+6]);
					WORD searchRange   = GETUSHORT(&buffer[pos+8]);
					WORD entrySelector = GETUSHORT(&buffer[pos+10]);
					WORD rangeShift    = GETUSHORT(&buffer[pos+12]);

					// Read each pair
					for( int c = 0; c < nPairs; c++ )
					{
						WORD  left  = GETUSHORT(&buffer[pos+14+c*6]);
						WORD  right = GETUSHORT(&buffer[pos+16+c*6]);
						short value = GETSHORT(&buffer[pos+18+c*6]);

						if( value )
							AddKerningPairToList(dc, left, right, value, pairs, scaleFactor, glyphIdToChar);
					}
				}
				else if( format == 2 )
				{
					// TODO: This format is not properly supported by Windows according to 
					//       above references, so I'll only implement it when I find a need
					//       for it.
					assert( false );
				}
				else
				{
					// These other formats are undefined
					assert( false );
				}
			}
			else
			{
				// TODO: Tables with minimum should be used to limit the accumulated adjustment
			}
		}

		pos += length;
	}
}

//=================================================================================
//
// ref: http://www.microsoft.com/typography/otspec/otff.htm
// ref: https://www.microsoft.com/typography/otspec/cmap.htm

int EnumTrueTypeCMAP(HDC dc, map<unsigned int, unsigned int> &unicodeToGlyphMap)
{
	// Remove old mappings
	unicodeToGlyphMap.clear();

	// Load the CMAP table from the TrueType font file
	vector<BYTE> buffer;
	DWORD CMAP = TAG('c', 'm', 'a', 'p');
	DWORD size = GetFontData(dc, CMAP, 0, 0, 0);
	if (size != GDI_ERROR)
	{
		buffer.resize(size);
		size = GetFontData(dc, CMAP, 0, &buffer[0], size);
	}
	if (size == GDI_ERROR || size == 0)
		return -1;

	// Get the CMAP header info
	WORD version = GETUSHORT(&buffer[0]);
	assert(version == 0x0000);
	WORD nTables = GETUSHORT(&buffer[2]);

	UINT pos = 4;
	for (unsigned int n = 0; n < nTables; n++)
	{
		// Look for the Windows platform CMAP subtable
		WORD platformID = GETUSHORT(&buffer[pos + 0]);
		WORD encodingID = GETUSHORT(&buffer[pos + 2]);
		ULONG offset = GETUINT(&buffer[pos + 4]);
		pos += 8;

		// platformID 1 is for Mac OS
		// platformID 3 is for Windows
		if (platformID == 3)
		{
			// For now this code only supports the following encodings:
			// 1 for Unicode UCS-2 (16bit)
			// 10 for Unicode UCS-4 (32bit)
			if (encodingID == 1 || encodingID == 10)
			{
				// Jump to the start of the subtable
				WORD format = GETUSHORT(&buffer[offset]);

				if (format == 0) // byte encoding table
					assert(false);
				else if (format == 2) // high-byte mapping through table
					assert(false);
				else if (format == 4) // segment mapping to delta values
				{
					// This is the standard format used by Microsoft
					// This format is for example used in the 'Arial' true type font

					// This format supports all Unicode characters below 0xFFFF.
					// The range U+D800 - U+DFFF is reserved for surrogates and cannot have any characters
					WORD length = GETUSHORT(&buffer[offset + 2]);
					WORD language = GETUSHORT(&buffer[offset + 4]);
					assert(language == 0);
					WORD segCountX2 = GETUSHORT(&buffer[offset + 6]);
					WORD searchRange = GETUSHORT(&buffer[offset + 8]);
					WORD entrySelector = GETUSHORT(&buffer[offset + 10]);
					WORD rangeShift = GETUSHORT(&buffer[offset + 12]);

					WORD segCount = segCountX2 / 2;
					WORD *endCount = (WORD*)&buffer[offset + 14];
					WORD *startCount = (WORD*)&buffer[offset + 16 + segCountX2];
					SHORT *idDelta = (SHORT*)&buffer[offset + 16 + segCountX2 * 2];
					WORD *idRangeOffset = (WORD*)&buffer[offset + 16 + segCountX2 * 3];
					// glyphIdArray is used indirectly so we don't need to take its address
					//WORD *glyphIdArray = (WORD*)&buffer[offset + 16 + segCountX2 * 4];
					
					// Iterate over each segment to identify the characters to glyph id mappings
					for (unsigned int s = 0; s < segCount; s++)
					{
						WORD start = SWAP16(startCount[s]);
						WORD end = SWAP16(endCount[s]);

						if (start == 0xFFFF && end == 0xFFFF)
						{
							// We've reached the end
							assert(s == segCount - 1);
							break;
						}

						WORD rangeOffset = SWAP16(idRangeOffset[s]);
						SHORT delta = SWAP16(idDelta[s]);
						for (unsigned int ch = start; ch <= end; ch++)
						{
							WORD glyphId = 0;
							if (idRangeOffset[s] != 0)
							{
								glyphId = SWAP16(*(&idRangeOffset[s] + rangeOffset / 2 + (ch - start)));
								if (glyphId != 0)
									glyphId += delta;
							}
							else
							{
								glyphId = ch + delta;
							}
							if (glyphId != 0)
								unicodeToGlyphMap[ch] = glyphId;
						}
					}
				}
				else if (format == 6) // trimmed table mapping
					assert(false);
				else if (format == 8) // mixed 16-bit and 32-bit coverage
					assert(false);
				else if (format == 10) // trimmed array
					assert(false);
				else if (format == 12) // segmented coverage
				{
					// This is the standard format used by Microsoft
					// This format is for example used in the 'Cambria Math' and 'DejaVu Sans' true type fonts

					ULONG length = GETUINT(&buffer[offset + 4]);
					ULONG language = GETUINT(&buffer[offset + 8]);
					assert(language == 0);
					ULONG nGroups = GETUINT(&buffer[offset + 12]);
					
					// Iterate over each group to identify the characters to glyph id mappings
					offset += 16;
					for (unsigned int s = 0; s < nGroups; s++)
					{
						ULONG start = GETUINT(&buffer[offset + 0]);
						ULONG end = GETUINT(&buffer[offset + 4]);
						ULONG startGlyphId = GETUINT(&buffer[offset + 8]);

						for (unsigned int ch = start; ch <= end; ch++)
						{
							ULONG glyphId = ch - start + startGlyphId;
							unicodeToGlyphMap[ch] = glyphId;
						}

						offset += 12;
					}
				}
				else if (format == 13) // many-to-one range mappings
					assert(false);
				else if (format == 14) // unicode variation sequences
					assert(false);
				else
					assert(false);
			}
		}
	}

	return 0;
}

void ConvertWCharToUtf8(const WCHAR *buf, std::string &utf8)
{
	char bufUTF8[1024];
	WideCharToMultiByte(CP_UTF8, 0, buf, -1, bufUTF8, 1024, NULL, NULL);
	utf8 = bufUTF8;
}

void ConvertUtf8ToWChar(const std::string &utf8, WCHAR *buf, size_t bufSize)
{
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, buf, int(bufSize));
}

// ref: https://stackoverflow.com/questions/11387564/get-a-font-filepath-from-name-and-style-in-c-windows
// ref: https://stackoverflow.com/questions/16769758/get-a-font-filename-based-on-the-font-handle-hfont
std::string GetFontFileName(const std::string &faceName, bool bold, bool italic)
{
	static const LPWSTR fontRegistryPath = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";

	HKEY hKey;
	LONG result;
	WCHAR buf[512];
	ConvertUtf8ToWChar(faceName, buf, 512);
	std::wstring wsFaceName(buf);

	// Open Windows font registry key
	result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, fontRegistryPath, 0, KEY_READ, &hKey);
	if (result != ERROR_SUCCESS) 
		return "";

	DWORD maxValueNameSize, maxValueDataSize;
	result = RegQueryInfoKeyW(hKey, 0, 0, 0, 0, 0, 0, 0, &maxValueNameSize, &maxValueDataSize, 0, 0);
	if (result != ERROR_SUCCESS)
		return "";

	DWORD valueIndex = 0;
	LPWSTR valueName = new WCHAR[maxValueNameSize];
	LPBYTE valueData = new BYTE[maxValueDataSize];
	DWORD valueNameSize, valueDataSize, valueType;
	std::wstring wsFontFile;

	// Look for a matching font name
	do 
	{
		wsFontFile.clear();
		valueDataSize = maxValueDataSize;
		valueNameSize = maxValueNameSize;

		result = RegEnumValueW(hKey, valueIndex, valueName, &valueNameSize, 0, &valueType, valueData, &valueDataSize);

		valueIndex++;

		if (result != ERROR_SUCCESS || valueType != REG_SZ)
			continue;

		std::wstring wsValueName(valueName, valueNameSize);

		// Found a match
		// TODO: The face name is not always the first part of the regkey
		//       e.g. The font "Microsoft JhengHei UI" matches the regkey "Microsoft JhengHei & Microsoft JhengHei UI (TrueType)"
		// TODO: It is not enough that the face name is part of the regkey name
		//       e.g. The font "DejaVu Sans" shouldn't match "DejaVu Sans Condensed (TrueType)"
		if (wcsstr(wsValueName.c_str(), wsFaceName.c_str()) != 0)
		{
			// TODO: Make sure the font data actually match that of the file
			wsFontFile.assign((LPWSTR)valueData, valueDataSize);
			break;
		}
	} while (result != ERROR_NO_MORE_ITEMS);

	delete[] valueName;
	delete[] valueData;

	RegCloseKey(hKey);

	if (wsFontFile.empty())
		return "";

	// Build full font file path
	WCHAR winDir[MAX_PATH];
	GetWindowsDirectoryW(winDir, MAX_PATH);

	std::wstringstream ss;
	ss << winDir << "\\Fonts\\" << wsFontFile;
	wsFontFile = ss.str();

	std::string out;
	ConvertWCharToUtf8(wsFontFile.c_str(), out);

	return out;
}

