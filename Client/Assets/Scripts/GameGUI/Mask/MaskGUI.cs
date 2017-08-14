using UnityEngine;
using System.Collections;
using System;

public class MaskGUI : MonoBehaviour
{
    private GTWindow   m_TargetWindow = null;
    private UIPanel    m_Panel;
    private GameObject m_Mask;

    private void Awake()
    {
        m_Panel = GetComponent<UIPanel>();
        m_Mask = transform.Find("Mask").gameObject;
    }

    private void Start()
    {
        UIEventListener.Get(m_Mask).onClick = OnBtnMaskClick;
    }

    private void OnBtnMaskClick(GameObject go)
    {
        if (m_TargetWindow != null && m_TargetWindow.OnMaskClick != null)
        {
            m_TargetWindow.OnMaskClick();
            m_TargetWindow = null;
        }
    }

    public  void SetTargetWindow(GTWindow w)
    {
        if (w == null)
        {
            return;
        }
        this.m_TargetWindow = w;
        this.m_Panel.depth = m_TargetWindow.Panel.depth - 1;
    }
}
