#include "stdafx.h"
#include "DFA.h"

INT32 DFANode::s_length = 0;
INT32 DFANode::s_maxlen = 0;
std::string DFANode::s_str = "";

//--------------------------------------------------------------------------------------------------------
DFANode::DFANode(KeyType key) : m_key(key), m_start(0)
{

}
//--------------------------------------------------------------------------------------------------------
DFANode::~DFANode()
{
	ClearChildNode();
}
//--------------------------------------------------------------------------------------------------------
BOOL DFANode::AddNode(const KeyType* key, INT32 len)
{
	if (key == NULL || len < 0)
	{
		return FALSE;
	}

	int i = 0;
	DFANode* ptemNode = this;
	while (i < len)
	{
		ptemNode = ptemNode->AddNode(key[i]);
		++i;
	}

	return TRUE;
}
//--------------------------------------------------------------------------------------------------------
DFANode* DFANode::AddNode(KeyType key)
{
	DFANodeMap::iterator it = m_childNodeMap.find(key);

	if (it == m_childNodeMap.end())
	{
		DFANode* temnode = new DFANode(key);
		m_childNodeMap.insert(std::make_pair(key, temnode));
		return temnode;
	}
	else
	{
		return it->second;
	}
}
//--------------------------------------------------------------------------------------------------------
DFANode* DFANode::GetNode(KeyType key) const
{
	DFANodeMap::const_iterator  it = m_childNodeMap.find(key);
	if (it == m_childNodeMap.end())
	{
		return NULL;
	}
	else
	{
		return it->second;
	}

}
//--------------------------------------------------------------------------------------------------------
BOOL DFANode::HasKeyWord(const KeyType* pdata, int len, BOOL bReturn/* = true*/)
{
	if (bReturn)
	{
		s_length = 0;
		s_maxlen = 0;
	}

	INT32 currentPos = 0;
	while (currentPos < len)
	{
		KeyType p = pdata[currentPos];
		DFANode* pnode = GetNode(p);
		++s_length;
		if (pnode == NULL)///如果没有找到到。从下一个开始找
		{
			if (bReturn == false)
			{
				return FALSE;
			}

			++currentPos;
			s_length = 0;
			continue;
		}
		else if (pnode->HasEnding())
		{
			//记录最佳匹配长度
			s_maxlen = s_length;

			if (bReturn)
			{
				m_start = currentPos;
			}

			//如果只有一个子节点或者没有节点则直接返回
			if (pnode->GetChildCount() <= 1)
			{
				return TRUE;
			}
		}

		int startpos = currentPos + 1;
		if (startpos >= len)
		{
			return s_maxlen != 0;
		}

		bool b = pnode->HasKeyWord(pdata + startpos, len - startpos, false);

		if (b == true)
		{
			if (bReturn)
			{
				m_start = currentPos;
			}

			return TRUE;
		}
		else
		{
			if (bReturn == false)
			{
				return FALSE;
			}
			else if (s_maxlen != 0)
			{
				s_length = s_maxlen;
				return TRUE;
			}

			++currentPos;
			s_length = 0;
			continue;
		}
	}

	return FALSE;
}
//--------------------------------------------------------------------------------------------------------
BOOL DFANode::LoadFile(const std::string& filename)
{
	ClearChildNode();

	FILE* pFile = fopen(filename.c_str(), "r+");

	if (pFile == NULL)
	{
		return FALSE;
	}

	char line[1024] = { 0 };
	do
	{
		fgets(line, 1024, pFile);

		int nLen = strlen(line);
		if (line[nLen - 1] == '\n')
		{
			line[nLen - 1] = 0;
		}

		if (line[nLen - 1] == ' ')
		{
			line[nLen - 1] = 0;
		}

		if (strcmp(line, "*") == 0)
		{
			continue;
		}

		strcat(line, "#");//每个字符添加#作为结尾符

		std::string tmpline = line;

		std::transform(tmpline.begin(), tmpline.end(), tmpline.begin(), toupper);

		AddNode(tmpline.c_str(), (int)tmpline.size());

	}
	while (!feof(pFile));

	fclose(pFile);

	return TRUE;
}
//--------------------------------------------------------------------------------------------------------
BOOL DFANode::IsFileterWord(const std::string& word)
{
	std::string wpData(word);

	std::transform(wpData.begin(), wpData.end(), wpData.begin(), toupper);

	return HasKeyWord(wpData.c_str(), (int)wpData.size());
}

//--------------------------------------------------------------------------------------------------------
BOOL DFANode::FilterKeyWords(std::string& word, const std::string& dest /*= "***"*/)
{
	if (word.empty())
	{
		return FALSE;
	}

	std::string wpData = word;

	std::transform(wpData.begin(), wpData.end(), wpData.begin(), toupper);

	//用来记录字符串的查询位置
	int start = 0, strlen = 0;
	int lastPos = -1;	//上次替换的位置
	BOOL keyflag = FALSE;

	//替换成的宽字符和长度
	while (HasKeyWord(wpData.c_str(), (int)wpData.size()))
	{
		//获取敏感词在字符串中的位置和长度，判断是否是连续敏感词，如果是则替换为空
		GetKeyPos(start, strlen);
		if (lastPos == start)
		{
			wpData.replace(wpData.begin() + start, wpData.begin() + start + strlen, "");
			lastPos = start;
		}
		else
		{
			wpData.replace(wpData.begin() + start, wpData.begin() + start + strlen, dest.c_str());
			lastPos = start + (int)dest.size();
		}
		//表示含有敏感词
		keyflag = TRUE;
	}

	//如果有敏感词，则需要使用替换后的字符串
	if (keyflag)
	{
		word = wpData;
	}

	return TRUE;
}
//--------------------------------------------------------------------------------------------------------
void DFANode::GetKeyPos(INT32& nStart, INT32& nLen)
{
	nStart	= m_start;
	nLen	= s_length;
	m_start = 0;
	s_length = 0;
}
//--------------------------------------------------------------------------------------------------------
void DFANode::ClearChildNode()
{
	DFANodeMap::iterator  itend = m_childNodeMap.end();
	for (DFANodeMap::iterator  it = m_childNodeMap.begin(); it != itend; ++it)
	{
		delete (it->second);
	}

	m_childNodeMap.clear();
}
//--------------------------------------------------------------------------------------------------------
BOOL DFANode::HasEnding()
{
	return m_childNodeMap.empty() ? TRUE : m_childNodeMap.find('#') != m_childNodeMap.end();
}

INT32 DFANode::GetChildCount() const
{
	return (INT32)m_childNodeMap.size();
}

