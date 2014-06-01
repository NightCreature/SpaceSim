//#ifndef SETTINGSPARSER_H
//#define SETTINGSPARSER_H
//
//#include <string>
//
/////This class provides the functionality to read a INI file. The settings you get with this are then used to
/////Initialize other parts of the engine.
//class SettingsParser
//{
//public:
//	SettingsParser() {};
//	SettingsParser(std::string filename);
//	~SettingsParser() {};
//
//	///These functions specify a section and a key and return the associated value or return the default
//	int readInt(const std::string section, const std::string key, const int defaultvalue);
//	float readFloat(const std::string section, const std::string key, const float defaultvalue);
//	bool readBool(const std::string section, const std::string key, const bool defaultvalue);
//	std::string readString(const std::string section, const std::string key, const std::string defaultvalue);
//
//	///These functions specify a section, a key and a value it then writes this value in the INI file
//	void writeInt(const std::string section, const std::string key, const int value);
//	void writeFloat(const std::string section, const std::string key, const float value);
//	void writeBool(const std::string section, const std::string key, const bool value);
//	void writeString(const std::string section, const std::string key, const std::string value);
//protected:
//private:
//	std::string m_filename;
//	char		m_buffer[255];
//	int			m_numcharinbuf;
//};
//#endif