#ifndef __RANK_LIB__H_
#define __RANK_LIB__H_

struct TRankItem
{
	TRankItem()
	{
		RankID = 0;
		RankValue = 0;
	}
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
		m_vtRankList.assign(m_dwRankNum, TRankItem());
		return TRUE;
	}

	BOOL SearchInsert2(UINT64 RankID, UINT64 RankValue, INT32& nOrgIndex, INT32& nTargetIndex)
	{
		nOrgIndex = -1;
		nTargetIndex = -1;
		INT32 nCount = (INT32)m_vtRankList.size();
		if (nCount == 0)
		{
			return FALSE;
		}

		for (int i = 0; i < m_vtRankList.size(); i++)
		{
			if (m_vtRankList[i].RankID == RankID)
			{
				nOrgIndex = i;
			}

			if ((RankValue > m_vtRankList[i].RankValue || m_vtRankList[i].RankID == 0) && nTargetIndex == -1)
			{
				nTargetIndex = i;
			}

			if ((m_vtRankList[i].RankID == 0) || (nOrgIndex != -1 && nTargetIndex != -1))
			{
				break;
			}
		}

		return TRUE;
	}


	BOOL SearchInsert(UINT64 RankID, UINT64 RankValue, INT32& nOrgIndex, INT32& nTargetIndex)
	{
		nOrgIndex = -1;
		nTargetIndex = -1;
		std::vector<TRankItem>::size_type nCount = m_vtRankList.size();
		if(nCount == 0)
		{
			return FALSE;
		}

		if (RankValue > m_vtRankList[0].RankValue)
		{
			nTargetIndex = 0;
		}
		else
		{
			std::vector<TRankItem>::size_type left = 0, right = nCount - 1;
			while (left <= right)
			{
				int mid = left + (right - left) / 2;
				if ((RankValue > m_vtRankList[mid].RankValue) && (RankValue <= m_vtRankList[mid - 1].RankValue))
				{
					nTargetIndex = mid;
					break;
				}
				else if (RankValue > m_vtRankList[mid].RankValue)
				{
					right = mid - 1;
				}
				else
				{
					left = mid + 1;
				}
			}
		}

		std::vector<TRankItem>::size_type myIndex = nCount - 1;
		for (std::vector<TRankItem>::size_type i = nTargetIndex; i < nCount; i++)
		{
			if (m_vtRankList[i].RankID == RankID)
			{
				nOrgIndex = i;
				break;
			}

			if (m_vtRankList[i].RankID == 0)
			{
				break;
			}
		}

		return TRUE;
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
		std::vector<TRankItem>::size_type nCount = m_vtRankList.size();
		UINT64 MinValue = m_vtRankList[nCount - 1].RankValue;
		if (RankValue <= MinValue)
		{
			return -1;
		}

		INT32 nOrgIndex = -1, nTargetIndex = -1;

		SearchInsert(RankID, RankValue, nOrgIndex, nTargetIndex);

		if (nOrgIndex != -1)
		{
			if (nOrgIndex == nTargetIndex)
			{
				m_vtRankList[nOrgIndex].RankValue = RankValue;
				m_vtRankList[nOrgIndex].RankID = RankID;
				return nTargetIndex;
			}
		}
		else
		{
			nOrgIndex = nCount - 1;
		}

		for (std::vector<TRankItem>::size_type i = nOrgIndex; i > nTargetIndex; i--)
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
		INT32 nOldIndex = -1;

		//if (OldRankValue >= uMinValue)
		{
			for ( INT32 i = 0; i < nCount; i++)
			{
				if (RankID == m_vtRankList[i].RankID)
				{
					nOldIndex = i;
					break;
				}
			}
		}

		if (nOldIndex >= 0)
		{
			m_vtRankList[nOldIndex].RankValue = NewRankValue;
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

		INT32 nOrgIndex = -1, nTargetIndex = -1;

		SearchInsert(RankID, RankValue, nOrgIndex, nTargetIndex);

		return nOrgIndex;
	}
public:
	INT32 m_dwShowNum;
	INT32 m_dwRankNum;
	std::vector<TRankItem> m_vtRankList;
};

#endif // 




