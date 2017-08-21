#pragma once
#define TILESIZE 1  // change this also to reflect tile size. 64x64.
#define MAPDATANOCOPY		//use the pointer for mapdata

//说明
/*
 对TileMap来说， 每一位表示一块方各，当位为1表示不可通行， 0:表示可以通行
*/
class AstarFinder
{
private:
	struct NODE       // node structure
	{
		long f, h;
		int g, tmpg;
		int x, y;
		int NodeNum;
		NODE* Parent;
		NODE* Child[8];  // a node may have upto 8+(NULL) children.
		NODE* NextNode;  // for filing purposes
	};

	NODE* m_pOpenList;    // the node list pointers
	NODE* m_pClosedList;  
	NODE* m_pCurPath;    // pointer to the best path

	struct STACK        // the stack structure
	{
		NODE* pNode;
		STACK* pNextStack;
	};

	STACK* m_pStack;

	int m_nRowCnt,			// tilemap data members, need to be initialisize
	    m_nColCnt,			// with current map's width and height
	    m_nTotalTiles;	// to allocate memory for the
	BYTE* m_pTileMap;		// pointer to the A* own tilemap data array
public:
	AstarFinder(void);
	~AstarFinder();
	void InitAstarMap(BYTE* pMap, int w, int h);

	BOOL NewPath(int sx, int sy, int dx, int dy); 
	BOOL IsReached(void); 
	BOOL PathNextNode(void)
	{
		if(m_pCurPath->Parent != NULL)
		{
			m_pCurPath = m_pCurPath->Parent;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	int NodeGetX(void)      { return m_pCurPath->x; }
	int NodeGetY(void)      { return m_pCurPath->y; }

	int GetTileNum(int x, int y); 

	//为真表示不能通过
	int IsTileAviable(int x, int y);


private:
	void FreeNodes(void);
	void FindPath(int sx, int sy, int dx, int dy);
	NODE* GetBestNode(void);
	void GenerateSuccessors(NODE* BestNode, int dx, int dy);
	void GenerateSucc(NODE* BestNode, int x, int y, int dx, int dy);
	NODE* CheckOPEN(int tilenum);
	NODE* CheckCLOSED(int tilenum);
	void Insert(NODE* Successor);
	void PropagateDown(NODE* Old);
	void Push(NODE* Node); // stack functions
	NODE* Pop(void);
};



