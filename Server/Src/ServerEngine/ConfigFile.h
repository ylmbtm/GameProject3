#ifndef _CONFIG_FILE_H_
#define _CONFIG_FILE_H_

class CConfigFile
{
private:
	CConfigFile(void);
	~CConfigFile(void);

public:
	static CConfigFile* GetInstancePtr();

public:
	BOOL Load(std::string strFileName);

	std::string GetStringValue(std::string strName);

	INT32 GetIntValue( std::string VarName);

	FLOAT GetFloatValue( std::string VarName);

	DOUBLE GetDoubleValue( std::string VarName);

	INT32  GetRealNetPort(std::string VarName);

private:
	std::map<std::string, std::string> m_Values;
};

#endif