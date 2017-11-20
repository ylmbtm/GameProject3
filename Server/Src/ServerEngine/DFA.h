#ifndef __D_F_A_H__
#define __D_F_A_H__

class DFANode;
typedef wchar_t keytype;
typedef std::map<keytype, DFANode*> DFANodeMap;
typedef std::set<keytype> DFANodeSet;

class DFANode
{
public:
	DFANode(keytype key);

	~DFANode();

	//加入了个子节点
	DFANode* addNode(keytype key);
	bool addNode(const keytype* key, int len);

	//获取包含 key的子节点
	DFANode* getNode(keytype key) const;

	/**判断是否包含指定的key*/
	bool hasKeyWord(const keytype* pdata, int len, bool isreturn = true);

	//加载关键字文件
	bool loadFile(const std::string& filename);

	//判断是否含有关键字 是的话true
	bool isFileterWord(const std::string& word);
	bool isFileterWord(const std::wstring& word);

	//把字符串中的所有敏感词都替换成"***"
	bool filterKeyWords(std::string& word, const std::wstring& dest = L"***");

private:
	void getKeyPos(INT32& nStart, INT32& nLen);

	//清空节点数据
	void clearChildNode();

	//判断子节点是否含有收尾服
	bool hasEnding();

	//获取子节点个数
	inline INT32 getChildCount() const { return (INT32)_childNodeSet.size(); }
protected:
	keytype	_key;
	INT32 _start;
	DFANodeMap _childNodeMap;		//所有子节点
	DFANodeSet _childNodeSet;		//所有子节点字符保存

	static INT32 s_length;		//记录匹配到的长度
	static INT32 s_maxlen;		//记录最佳匹配长度
	static std::wstring s_str;	//静态的用来替换的字符串
};

#endif