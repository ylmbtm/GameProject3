#ifndef __HTTP_PARAMETER_H__
#define __HTTP_PARAMETER_H__

class HttpParameter
{
public:
	HttpParameter(const std::string& strParam);

	~HttpParameter(void);

	void ParseStringToMap(const std::string& strParam);

	bool HasKey(const std::string& strKey) const;

	bool GetIntValue(const std::string& strKey, INT32& intValue) const;

	bool GetStrValue(const std::string& strKey, std::string& strValue) const;

	bool GetLongValue(const std::string& strKey, INT64& longValue) const;

	bool GetFloatValue(const std::string& strKey, FLOAT& floatValue) const;

protected:
	std::map<std::string, std::string> m_ParameterMap;
};


#endif /* __HTTP_PARAMETER_H__*/