using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GTCallback
{
    List<Callback> m_callbacks = new List<Callback>();

    public void AddListener(Callback callback)
    {
        if(m_callbacks.Contains(callback))
        {
            m_callbacks.Add(callback);
        }
    }

    public void Invoke(bool clear = true)
    {
        for (int i = 0; i < m_callbacks.Count; i++)
        {
            m_callbacks[i].Invoke();
        }
        if(clear)
        {
            Release();
        }
    }

    public void Release()
    {
        m_callbacks.Clear();
    }

    public bool CanInvoke()
    {
        return m_callbacks.Count > 0;
    }
}
