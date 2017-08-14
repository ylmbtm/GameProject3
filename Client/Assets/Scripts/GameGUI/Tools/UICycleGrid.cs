using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public class UICycleGrid : MonoBehaviour
{
    public delegate void OnInitializeItemCallback(GameObject go, int realIndex);

    public int           itemSize     = 100;
    public int           itemInterval = 300;
    public int           itemCount    = 0;
    public bool          cullContent  = true;
    public int           maxPerLine   = 1;
    public bool          negative     = false;
    public int           useItemCount = 0;
    public GameObject    useItemPrefab;

    public OnInitializeItemCallback onInitializeItemCallback;
    public OnInitializeItemCallback onInitializeItemCallbackForLua;

    private Transform             m_Trans;
    private UIPanel               m_Panel;
    private UIScrollView          m_Scroll;
    private bool                  m_Horizontal = false;
    private bool                  m_FirstTime  = true;
    private List<Transform>       m_Children   = new List<Transform>();
    private Vector3               m_PanelTrans;
    private Vector2               m_PanelOffset;
    private Vector2               m_FirstPoint; //初始化位置
    private float                 m_Extents;
    private int                   m_CurUseCount = 0;
    private int                   minIndex = 0;
    private int                   maxIndex = 0;


    void Start()
    {
        CacheScrollView();
        if (useItemPrefab != null)
        {
            useItemPrefab.SetActive(false);
            for (int i = 0; i < useItemCount; i++)
            {
                NGUITools.AddChild(gameObject, useItemPrefab);
            }
        }
        m_FirstTime = true;
        if(itemCount > 0)
        {
            if (negative)
            {
                minIndex = 1 - itemCount;
                maxIndex = 0;
            }
            else
            {
                minIndex = 0;
                maxIndex = itemCount - 1;
            }
        }
        SortBasedOnScrollMovement();
        if (m_Scroll != null) m_Scroll.GetComponent<UIPanel>().onClipMove = OnMove;
        m_FirstTime = false;
        m_PanelTrans = m_Panel.transform.localPosition;
        m_PanelOffset = m_Panel.clipOffset;
    }


    protected void OnMove(UIPanel panel) { WrapContent(); }

    [ContextMenu("Sort Based on Scroll Movement")]
    protected void SortBasedOnScrollMovement()
    {
        if (!CacheScrollView())
        {
            return;
        }
        m_Children.Clear();
        for (int i = 0; i < m_Trans.childCount; ++i)
        {
            Transform t = m_Trans.GetChild(i);
            if (itemCount == 0)
            {
                t.gameObject.SetActive(false);
            }
            else
            {
                if (m_Children.Count >= itemCount)
                {
                    t.gameObject.SetActive(false);
                }
                else
                {
                    t.gameObject.SetActive(true);
                    m_Children.Add(t);
                }
            }
        }
        if (m_Children.Count == 0)
        {
            return;
        }
        if (m_CurUseCount <= 0 || m_CurUseCount > m_Children.Count)
        {
            m_CurUseCount = m_Children.Count;
        }
        m_Extents = itemSize * (m_Children.Count / maxPerLine + (m_Children.Count % maxPerLine == 0 ? 0 : 1)) * 0.5f;
        m_FirstPoint = m_Children[0].transform.localPosition;
        ResetChildPositions();
    }

    [ContextMenu("Sort Alphabetically")]
    protected void SortAlphabetically()
    {
        if (!CacheScrollView()) return;
        m_Children.Clear();
        for (int i = 0; i < m_Trans.childCount; ++i)
        {
            Transform t = m_Trans.GetChild(i);
            if (itemCount == 0)
            {
                t.gameObject.SetActive(false);
            }
            else
            {
                if (m_Children.Count >= itemCount)
                {
                    t.gameObject.SetActive(false);
                }
                else
                {
                    t.gameObject.SetActive(true);
                    m_Children.Add(t);
                }
            }
        }
        if (m_CurUseCount <= 0 || m_CurUseCount > m_Children.Count) m_CurUseCount = m_Children.Count;
        if (m_Children.Count == 0)
        {
            return;
        }
        m_FirstPoint = m_Children[0].transform.localPosition;
        m_Extents = itemSize * (m_Children.Count / maxPerLine + (m_Children.Count % maxPerLine == 0 ? 0 : 1)) * 0.5f;

        // Sort the list of children so that they are in order
        m_Children.Sort(UIGrid.SortByName);
        ResetChildPositions();
    }

    protected bool CacheScrollView()
    {
        m_Trans = transform;
        m_Panel = NGUITools.FindInParents<UIPanel>(gameObject);
        m_Scroll = m_Panel.GetComponent<UIScrollView>();
        if (m_Scroll == null) return false;
        if (m_Scroll.movement == UIScrollView.Movement.Horizontal) m_Horizontal = true;
        else if (m_Scroll.movement == UIScrollView.Movement.Vertical) m_Horizontal = false;
        else return false;
        return true;
    }

    protected void ResetChildPositions()
    {
        for (int i = 0, imax = m_Children.Count; i < imax; ++i)
        {
            Transform t = m_Children[i];

            t.localPosition = m_Horizontal ? new Vector3(i / maxPerLine * itemSize, i % maxPerLine * itemInterval, 0f) : new Vector3(i % maxPerLine * itemInterval + 2, -i / maxPerLine * itemSize, 0f);
            //t.localPosition = mHorizontal ? new Vector3(i * itemSize, 0, 0f) : new Vector3(0, -i * itemSize, 0f);

            t.name = i.ToString();
            UpdateItem(t, i);
        }
    }

    protected void UpdateItem(Transform item, int index)
    {
        int realIndex = (m_Scroll.movement == UIScrollView.Movement.Vertical) ?
            Mathf.RoundToInt(item.localPosition.y / itemSize) :
            Mathf.RoundToInt(item.localPosition.x / itemSize);
        int sortIndex = (m_Scroll.movement == UIScrollView.Movement.Vertical) ?
            Mathf.RoundToInt((item.localPosition.x - m_FirstPoint.x) / itemInterval) :
            Mathf.RoundToInt((item.localPosition.y - m_FirstPoint.y) / itemInterval);
        realIndex = Mathf.Abs(realIndex) * maxPerLine + sortIndex;
        if (onInitializeItemCallback != null)
        {
            onInitializeItemCallback(item.gameObject, realIndex);
        }
        if (onInitializeItemCallbackForLua != null)
        {
            onInitializeItemCallbackForLua(item.gameObject, realIndex +1);
        }
    }

    public Transform FindItem(int index)
    {
        for (int i = 0; i < m_Children.Count; i++)
        {
            Transform item = m_Children[i];
            int realIndex = (m_Scroll.movement == UIScrollView.Movement.Vertical) ?
    Mathf.RoundToInt(item.localPosition.y / itemSize) :
    Mathf.RoundToInt(item.localPosition.x / itemSize);
            int sortIndex = (m_Scroll.movement == UIScrollView.Movement.Vertical) ?
                Mathf.RoundToInt((item.localPosition.x - m_FirstPoint.x) / itemInterval) :
                Mathf.RoundToInt((item.localPosition.y - m_FirstPoint.y) / itemInterval);
            realIndex = Mathf.Abs(realIndex) * maxPerLine + sortIndex;

            if (realIndex == index)
            {
                return item;
            }

        }
        return null;
    }

    public void WrapContent()
    {
        Vector3[] corners = m_Panel.worldCorners;
        for (int i = 0; i < 4; ++i)
        {
            Vector3 v = corners[i];
            v = m_Trans.InverseTransformPoint(v);
            corners[i] = v;
        }

        Vector3 center = Vector3.Lerp(corners[0], corners[2], 0.5f);
        bool allWithinRange = true;
        float ext2 = m_Extents * 2f;

        if (m_Horizontal)
        {
            float min = corners[0].x - itemSize;
            float max = corners[2].x + itemSize;

            for (int i = 0, imax = m_Children.Count; i < imax && i < m_CurUseCount; ++i)
            {
                Transform t = m_Children[i];
                float distance = t.localPosition.x - center.x;

                if (distance < -m_Extents)
                {
                    Vector3 pos = t.localPosition;
                    pos.x += ext2;
                    distance = pos.x - center.x;
                    int realIndex = Mathf.RoundToInt(pos.x / itemSize);
                    int sortIndex = Mathf.RoundToInt((pos.y - m_FirstPoint.y) / itemInterval);
                    realIndex = realIndex * maxPerLine + sortIndex;

                    if (minIndex == maxIndex || (minIndex <= realIndex && realIndex <= maxIndex))
                    {
                        t.localPosition = pos;
                        UpdateItem(t, i);
                    }
                    else allWithinRange = false;
                }
                else if (distance > m_Extents)
                {
                    Vector3 pos = t.localPosition;
                    pos.x -= ext2;
                    distance = pos.x - center.x;
                    int realIndex = Mathf.RoundToInt(pos.x / itemSize);
                    //--mxh
                    int sortIndex = Mathf.RoundToInt((pos.y - m_FirstPoint.y) / itemInterval);
                    realIndex = realIndex * maxPerLine + sortIndex;

                    if (minIndex == maxIndex || (minIndex <= realIndex && realIndex <= maxIndex))
                    {
                        t.localPosition = pos;
                        UpdateItem(t, i);
                    }
                    else allWithinRange = false;
                }
                else if (m_FirstTime) UpdateItem(t, i);

                if (cullContent)
                {
                    distance += m_Panel.clipOffset.x - m_Trans.localPosition.x;
                    if (!UICamera.IsPressed(t.gameObject))
                        NGUITools.SetActive(t.gameObject, (distance > min && distance < max), false);
                }
            }
        }
        else
        {
            float min = corners[0].y - itemSize;
            float max = corners[2].y + itemSize;

            for (int i = 0, imax = m_Children.Count; i < imax && i < m_CurUseCount; ++i)
            {
                Transform t = m_Children[i];
                float distance = t.localPosition.y - center.y;
                if (distance > m_Extents)
                {
                    Vector3 pos = t.localPosition;
                    pos.y -= ext2; //zheng
                    //pos.y += ext2;
                    distance = pos.y - center.y;
                    int realIndex = Mathf.RoundToInt(pos.y / itemSize);
                    int sortIndex = Mathf.RoundToInt((pos.x - m_FirstPoint.x) / itemInterval);

                    realIndex = -realIndex * maxPerLine + sortIndex;
                    if (minIndex == maxIndex || (minIndex <= realIndex && realIndex <= maxIndex))
                    {
                        t.localPosition = pos;
                        UpdateItem(t, i);
                    }
                    else
                    {
                        allWithinRange = false;
                    }
                }
                else if (distance < -m_Extents)
                {
                    Vector3 pos = t.localPosition;
                    pos.y += ext2;
                    if (pos.y > 0)
                    { }
                    distance = pos.y - center.y;
                    int realIndex = Mathf.RoundToInt(pos.y / itemSize);
                    int sortIndex = Mathf.RoundToInt((pos.x - m_FirstPoint.x) / itemInterval);

                    realIndex = -realIndex * maxPerLine + sortIndex;
    
                    if (minIndex == maxIndex || (minIndex <= realIndex && realIndex <= maxIndex))
                    {
                        t.localPosition = pos;
                        UpdateItem(t, i);
                    }
                    else
                    {
                        allWithinRange = false;
                    }
                }
                else if (m_FirstTime) UpdateItem(t, i);

                if (cullContent)
                {
                    distance += m_Panel.clipOffset.y - m_Trans.localPosition.y;
                    if (!UICamera.IsPressed(t.gameObject))
                        NGUITools.SetActive(t.gameObject, (distance > min && distance < max), false);
                }
            }
        }
        m_Scroll.restrictWithinPanel = !allWithinRange;
    }

    public void ResetPosition()
    {
        ResetChildPositions();
        WrapContent();
        m_Panel.transform.localPosition = m_PanelTrans;
        m_Panel.clipOffset = m_PanelOffset;
    }

    public void Init(int count, int rowNum, bool isNegative = false)
    {
        this.itemCount = count;
        this.maxPerLine = rowNum;
        this.negative = isNegative;
        if (itemCount > 0)
        {
            if (negative)
            {
                minIndex = 1 - itemCount;
                maxIndex = 0;
            }
            else
            {
                minIndex = 0;
                maxIndex = itemCount - 1;
            }
        }
        this.SortBasedOnScrollMovement();
        this.WrapContent();
    }

    void OnValidate()
    {
        if (maxIndex < minIndex)
            maxIndex = minIndex;
        if (minIndex > maxIndex)
            maxIndex = minIndex;
    }
}