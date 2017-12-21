#ifndef __COMMAND_LINE_H__
#define __COMMAND_LINE_H__

class CCommandLine
{
public:
	CCommandLine(INT32 argc, CHAR* argv[]);

	~CCommandLine();

	INT32		GetIntValue(const std::string& strKey) const;

	std::string GetStrValue(const std::string& strKey) const;

	INT64		GetLongValue(const std::string& strKey) const;

	FLOAT		GetFloatValue(const std::string& strKey) const;
protected:
	std::map<std::string, std::string> m_ParameterMap;
};


#endif /* __COMMAND_LINE_H__*/