#ifndef __RANK_LIB__H_
#define __RANK_LIB__H_

template<class TID, class TValue>
struct TRankItem
{
    TRankItem()
    {
        RankID = 0;
        RankValue = 0;
    }
    TID RankID;
    TValue RankValue;
};


template<class TID, class TValue>
class TRanker
{
public:
    TRanker()
    {

    }

    TRanker(INT32 nShow, INT32 nTotal)
    {
        InitRanker(nShow, nTotal);
    }

    ~TRanker()
    {
        Clear();

        m_vtRankList.clear();
    }

    TRanker<TID, TValue>& operator = (const TRanker<TID, TValue>& tRanker)
    {
        m_vtRankList.clear();

        m_vtRankList = tRanker.m_vtRankList;

        return *this;
    }

    BOOL InitRanker(INT32 nShow, INT32 nTotal)
    {
        m_nShowNum = nShow;
        m_nRankNum = nTotal;
        m_vtRankList.assign(m_nRankNum, TRankItem<TID, TValue>());
        return TRUE;
    }

    void Clear()
    {
        for(INT32 i = 0; i < m_vtRankList.size(); i++)
        {
            m_vtRankList[i].RankID = 0;
            m_vtRankList[i].RankValue = -1;
        }
    }

    INT32 GetShowNum()
    {
        return m_nShowNum;
    }

    INT32 GetTotalNum()
    {
        return m_nShowNum;
    }

    INT32 SetRankItem(TID RankID, TValue RankValue)
    {
        INT32 nCount = (INT32)m_vtRankList.size();
        TValue MinValue = m_vtRankList[nCount - 1].RankValue;
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

        for (INT32 i = nOrgIndex; i > nTargetIndex; i--)
        {
            m_vtRankList[i].RankID = m_vtRankList[i - 1].RankID;
            m_vtRankList[i].RankValue = m_vtRankList[i - 1].RankValue;
        }

        m_vtRankList[nTargetIndex].RankID = RankID;
        m_vtRankList[nTargetIndex].RankValue = RankValue;

        return nTargetIndex;
    }

    void SetRankItemEx(TID RankID, TValue NewRankValue)
    {
        INT32 nCount = (INT32)m_vtRankList.size();
        TValue uMinValue = m_vtRankList[nCount - 1].RankValue;
        INT32 nOldIndex = -1;

        for ( INT32 i = 0; i < nCount; i++)
        {
            if (RankID == m_vtRankList[i].RankID)
            {
                nOldIndex = i;
                break;
            }
        }

        if (nOldIndex >= 0)
        {
            m_vtRankList[nOldIndex].RankValue = NewRankValue;
            std::sort(m_vtRankList.begin(), m_vtRankList.end(), TRanker::RankItemCompare);
            return;
        }

        if (NewRankValue > uMinValue)
        {
            m_vtRankList[nCount - 1].RankID = RankID;
            m_vtRankList[nCount - 1].RankValue = NewRankValue;
            std::sort(m_vtRankList.begin(), m_vtRankList.end(), TRanker::RankItemCompare);
        }

        return ;
    }

    INT32 GetRankIndex(TID RankID, TValue RankValue)
    {
        INT32 nCount = (INT32)m_vtRankList.size();
        TValue MinValue = m_vtRankList[nCount - 1].RankValue;
        if (RankValue <= MinValue)
        {
            return -1;
        }

        INT32 nOrgIndex = -1, nTargetIndex = -1;

        SearchInsert(RankID, RankValue, nOrgIndex, nTargetIndex);

        return nOrgIndex;
    }

    BOOL RemoveRankItem(TID RankID)
    {
        INT32 nPos = -1;
        for (INT32 i = 0; i < m_vtRankList.size(); i++)
        {
            if (m_vtRankList[i].RankID == RankID)
            {
                nPos = i;
                break;
            }
        }

        if (nPos < 0)
        {
            return TRUE;
        }

        for (INT32 i = nPos; i < m_vtRankList.size() - 1; i++)
        {
            m_vtRankList[i] = m_vtRankList[i + 1];
        }

        m_vtRankList[m_vtRankList.size() - 1].RankID = 0;
        m_vtRankList[m_vtRankList.size() - 1].RankValue = 0;

        return TRUE;
    }

    TRankItem<TID, TValue>* GetRankItem(INT32 nIndex)
    {
        if (nIndex >= m_nRankNum)
        {
            return NULL;
        }

        return &m_vtRankList.at(nIndex);
    }

private:
    static BOOL RankItemCompare(TRankItem<TID, TValue> a, TRankItem<TID, TValue> b)
    {
        return a.RankValue > b.RankValue;
    }

    BOOL SearchInsert2(TID RankID, TValue RankValue, INT32& nOrgIndex, INT32& nTargetIndex)
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


    BOOL SearchInsert(TID RankID, TValue RankValue, INT32& nOrgIndex, INT32& nTargetIndex)
    {
        nOrgIndex = -1;
        nTargetIndex = -1;
        INT32 nCount = (INT32)m_vtRankList.size();
        if (nCount == 0)
        {
            return FALSE;
        }

        if (RankValue > m_vtRankList[0].RankValue)
        {
            nTargetIndex = 0;
        }
        else
        {
            INT32 left = 0, right = nCount - 1;
            while (left <= right)
            {
                INT32 mid = left + (right - left) / 2;
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

        INT32 myIndex = nCount - 1;
        for (INT32 i = nTargetIndex; i < nCount; i++)
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

    INT32 m_nShowNum;
    INT32 m_nRankNum;
    std::vector<TRankItem<TID, TValue>> m_vtRankList;
};

#endif // 




