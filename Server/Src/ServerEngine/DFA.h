#ifndef __D_F_A_H__
#define __D_F_A_H__

class DFANode;
typedef char KeyType;
typedef std::map<KeyType, DFANode*> DFANodeMap;
typedef std::set<KeyType> DFANodeSet;

class DFANode
{
public:
	DFANode(KeyType key = 0);

	~DFANode();

	DFANode* AddNode(KeyType key);

	BOOL AddNode(const KeyType* key, INT32 len);

	DFANode* GetNode(KeyType key) const;

	BOOL HasKeyWord(const KeyType* pdata, INT32 len, BOOL bReturn = TRUE);

	BOOL LoadFile(const std::string& filename);

	BOOL IsFileterWord(const std::string& word);

	BOOL FilterKeyWords(std::string& word, const std::string& dest = "*");

private:
	void GetKeyPos(INT32& nStart, INT32& nLen);

	void ClearChildNode();

	BOOL HasEnding();

	INT32 GetChildCount() const;
protected:
	KeyType		m_key;
	INT32		m_start;
	DFANodeMap	m_childNodeMap;

	static INT32 s_length;
	static INT32 s_maxlen;
};

#endif