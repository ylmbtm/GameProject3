#ifndef __RANK_LIB__H_
#define __RANK_LIB__H_

struct TRankItem
{
	UINT64 RankID;
	UINT64 RankValue;
};

bool RankItemCompare(TRankItem a, TRankItem b)
{
	return a.RankValue < b.RankValue;
}

class TRanker
{
public:
	BOOL InitRanker(INT32 dwShow, INT32 dwTotal)
	{
		m_dwShowNum = dwShow;
		m_dwRankNum = dwTotal;
	}

	int SearchInsert(UINT64 RankValue)
	{
		INT32 nLen = (INT32)m_vtRankList.size();
		if(nLen == 0)
		{
			return 0;
		}

		INT32 left = 0, right = nLen - 1;
		while(left <= right)
		{
			INT32 mid = left + (right - left) / 2;
			if(RankValue == m_vtRankList[mid].RankValue)
			{
				return mid;
			}
			else if(RankValue < m_vtRankList[mid].RankValue)
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}
		return left;
	}

	void Clear()
	{
		for(std::vector<TRankItem>::size_type i = 0; i < m_vtRankList.size(); i++)
		{
			m_vtRankList[i].RankID = 0;
			m_vtRankList[i].RankValue = -1;
		}
	}


	INT32 SetRankItem(UINT64 RankID, UINT64 RankValue)
	{
		INT32 nCount = (INT32)m_vtRankList.size();
		UINT64 MinValue = m_vtRankList[nCount - 1].RankValue;
		if (RankValue <= MinValue)
		{
			return -1;
		}

		INT32 nTargetIndex = SearchInsert(RankValue);

		INT32 myIndex = nCount - 1;
		for (INT32 i = nTargetIndex; i < nCount; i++)
		{
			if (m_vtRankList[i].RankID == RankID || m_vtRankList[i].RankID == 0)
			{
				m_vtRankList[i].RankValue = RankValue;
				m_vtRankList[i].RankID = RankID;
				myIndex = i;
				break;
			}
		}

		if (myIndex == nTargetIndex)
		{
			return nTargetIndex;
		}

		for (INT32 i = myIndex; i > nTargetIndex; i--)
		{
			m_vtRankList[i].RankID = m_vtRankList[i - 1].RankID;
			m_vtRankList[i].RankValue = m_vtRankList[i - 1].RankValue;
		}

		m_vtRankList[nTargetIndex].RankID = RankID;
		m_vtRankList[nTargetIndex].RankValue = RankValue;
		return nTargetIndex;
	}

	void SetRankItemEx(UINT64 RankID, UINT64 OldRankValue, UINT64 NewRankValue)
	{
		INT32 nCount = (INT32)m_vtRankList.size();
		UINT64 uMinValue = m_vtRankList[nCount - 1].RankValue;
		INT32 nMyIndex = -1;

		if (OldRankValue >= uMinValue)
		{
			for ( INT32 i = 0; i < nCount; i++)
			{
				if (RankID == m_vtRankList[i].RankID)
				{
					nMyIndex = i;
					break;
				}
			}
		}

		if (nMyIndex >= 0)
		{
			m_vtRankList[nMyIndex].RankValue = NewRankValue;
			std::sort(m_vtRankList.begin(), m_vtRankList.end(), RankItemCompare);
			return;
		}

		if (NewRankValue > uMinValue)
		{
			m_vtRankList[nCount - 1].RankID = RankID;
			m_vtRankList[nCount - 1].RankValue = NewRankValue;
			std::sort(m_vtRankList.begin(), m_vtRankList.end(), RankItemCompare);
		}

		return ;
	}

	INT32 GetRankIndex(UINT64 RankID, UINT64 RankValue)
	{
		INT32 nCount = (INT32)m_vtRankList.size();
		UINT64 MinValue = m_vtRankList[nCount - 1].RankValue;
		if (RankValue <= MinValue)
		{
			return -1;
		}

		INT32 nTargetIndex = SearchInsert(RankValue);
		if (nTargetIndex == nCount)
		{
			return -1;
		}

		for (INT32 i = nTargetIndex; i >= 0; i--)
		{
			if (m_vtRankList[i].RankID == RankID)
			{
				return i + 1;
			}
		}

		return -1;
	}
public:
	INT32 m_dwShowNum;
	INT32 m_dwRankNum;
	std::vector<TRankItem> m_vtRankList;
};

#endif // 




