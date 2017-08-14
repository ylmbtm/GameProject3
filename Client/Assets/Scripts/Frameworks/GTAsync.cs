using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GTAsync : GTMonoSingleton<GTAsync>
{
    static List<Callback> m_Asyncs = new List<Callback>();

    public void Run(Callback callback)
    {
        m_Asyncs.Add(callback);
    }

    public void Execute()
    {
        for (int i = 0; i < m_Asyncs.Count; i++)
        {
            m_Asyncs[i].Invoke();
        }
        m_Asyncs.Clear();
    }
}
