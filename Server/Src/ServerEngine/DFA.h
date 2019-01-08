#ifndef __D_F_A_H__
#define __D_F_A_H__

class DFANode;
typedef char KeyType;
typedef std::map<KeyType, DFANode*> DFANodeMap;
typedef std::set<KeyType> DFANodeSet;

class DFANode
{
public:
	DFANode(KeyType key);

	~DFANode();

	//加入了个子节点
	DFANode* AddNode(KeyType key);

	BOOL AddNode(const KeyType* key, INT32 len);

	//获取包含 key的子节点
	DFANode* GetNode(KeyType key) const;

	/**判断是否包含指定的key*/
	BOOL HasKeyWord(const KeyType* pdata, INT32 len, BOOL bReturn = TRUE);

	//加载关键字文件
	BOOL LoadFile(const std::string& filename);

	//判断是否含有关键字 是的话true
	BOOL IsFileterWord(const std::string& word);

	//把字符串中的所有敏感词都替换成"***"
	BOOL FilterKeyWords(std::string& word, const std::string& dest = "*");

private:
	void GetKeyPos(INT32& nStart, INT32& nLen);

	//清空节点数据
	void ClearChildNode();

	//判断子节点是否含有收尾服
	BOOL HasEnding();

	//获取子节点个数
	INT32 GetChildCount() const;
protected:
	KeyType		m_key;
	INT32		m_start;
	DFANodeMap	m_childNodeMap;		//所有子节点

	static INT32 s_length;		//记录匹配到的长度
	static INT32 s_maxlen;		//记录最佳匹配长度
};

#endif