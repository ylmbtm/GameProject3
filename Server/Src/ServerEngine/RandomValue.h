#ifndef __RANDOM_VALUE__
#define __RANDOM_VALUE__

template <UINT16 Size>
class CRandonValue
{
private:
	CRandonValue()
	{
		for(INT32 i = 0; i < Size; i++)
		{
			m_Values[i] = i;
		}

		Disorder(m_Values, Size);

		m_nCurIndex		= 1;

		m_nStartIndex	= 0;
	}

	~CRandonValue()
	{


	}

	void Disorder(UINT16 Arr[], UINT16 nCount)
	{
		UINT16 nIndex, nTemp;
		rand(time(NULL));
		for (UINT16 i = 0; i <n; i++)
		{
			nIndex = rand() % (n - i) + i;
			if (nIndex != i)
			{
				nTemp		= Arr[i];
				Arr[i]		= Arr[nIndex];
				Arr[nIndex]	= nTemp;
			}
		}
	}


public:
	static CRandonValue& GetInstance()
	{
		static CRandonValue _Value;

		return _Value;
	}


	UINT16 Random()
	{
		m_nCurIndex = (m_nCurIndex+1)%Size;
		if(m_nCurIndex != m_nStartIndex)
		{
			return m_Values[m_nCurIndex];
		}
		else
		{
			m_nStartIndex = m_nCurIndex = rand()%Size;

			return m_Values[m_nCurIndex];
		}
	}

	UINT16  m_Values[Size];

	UINT16  m_nCurIndex;

	UINT16  m_nStartIndex;
}


#endif /* __RANDOM_VALUE__*/