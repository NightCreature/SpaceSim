//#include "settingsparser.h"
//#include <iostream>
//#include <Windows.h>
//#include <sstream>
//
//using namespace std;
//
//SettingsParser::SettingsParser(std::string filename)
//{
//	m_filename = filename;
//}
//
//int SettingsParser::readInt(const std::string section, const std::string key, const int defaultvalue)
//{
//	return GetPrivateProfileIntA(section.c_str(), key.c_str(), defaultvalue, m_filename.c_str());
//}
//
//float SettingsParser::readFloat(const std::string section, const std::string key, const float defaultvalue)
//{
//	std::stringstream dv;
//	dv << defaultvalue;
//	GetPrivateProfileStringA(section.c_str(), key.c_str(), dv.str().c_str(), m_buffer, m_numcharinbuf, m_filename.c_str());
//	float returnvalue;
//	returnvalue = (float)atof(m_buffer);
//	return returnvalue;
//}
//
//bool SettingsParser::readBool(const std::string section, const std::string key, const bool defaultvalue)
//{
//	int dv;
//	dv = defaultvalue ? 1 : 0;
//	int rv = GetPrivateProfileIntA(section.c_str(), key.c_str(), dv, m_filename.c_str());
//	bool returnvalue;
//	returnvalue = rv ? true : false;
//	return returnvalue;
//}
//
//std::string SettingsParser::readString(const std::string section, const std::string key, const std::string defaultvalue)
//{
//	memset(m_buffer, 0x00, 255);
//	GetPrivateProfileStringA(section.c_str(), key.c_str(), defaultvalue.c_str(), m_buffer, m_numcharinbuf, m_filename.c_str());
//	std::string str;
//	str.assign(m_buffer);
//	return str;
//}
//
//void SettingsParser::writeInt(const std::string section, const std::string key, const int value)
//{
//	std::stringstream dv;
//	dv << value;
//	WritePrivateProfileStringA(section.c_str(), key.c_str(), dv.str().c_str(), m_filename.c_str());
//}
//
//void SettingsParser::writeFloat(const std::string section, const std::string key, const float value)
//{
//	std::stringstream dv;
//	dv << value;
//	WritePrivateProfileStringA(section.c_str(), key.c_str(), dv.str().c_str(), m_filename.c_str());
//}
//
//void SettingsParser::writeBool(const std::string section, const std::string key, const bool value)
//{
//	std::stringstream dv;
//	int iv = value ? 1 : 0;
//	dv << iv;
//	WritePrivateProfileStringA(section.c_str(), key.c_str(), dv.str().c_str(), m_filename.c_str());
//}
//
//void SettingsParser::writeString(const std::string section, const std::string key, const std::string value)
//{
//	WritePrivateProfileStringA(section.c_str(), key.c_str(), value.c_str(), m_filename.c_str());
//}
