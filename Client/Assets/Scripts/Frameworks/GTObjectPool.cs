using System;
using System.Collections.Generic;
using UnityEngine.Events;

public interface IObjectPool
{
    int CountAll
    {
        get;
    }

    int CountActive
    {
        get;
    }

    int CountInactive
    {
        get;
    }
}

public class GTObjectPool<T> : IObjectPool
{
    public delegate T CreateObj();
    private readonly Stack<T>                m_Stack = new Stack<T>();
    private readonly UnityAction<T>          m_ActionOnGet;
    private readonly UnityAction<T>          m_ActionOnRelease;
    private GTObjectPool<T>.CreateObj        m_objCreator;
    public static readonly List<IObjectPool> s_AllPool = new List<IObjectPool>();

    public int CountAll
    {
        get;
        private set;
    }

    public int CountActive
    {
        get
        {
            return this.CountAll - this.CountInactive;
        }
    }

    public int CountInactive
    {
        get
        {
            return this.m_Stack.Count;
        }
    }

    public GTObjectPool(CreateObj creator, UnityAction<T> actionOnGet, UnityAction<T> actionOnRelease)
    {
        this.m_objCreator = creator;
        this.m_ActionOnGet = actionOnGet;
        this.m_ActionOnRelease = actionOnRelease;
        GTObjectPool<T>.s_AllPool.Add(this);
    }

    public T Get()
    {
        T t;
        if (this.m_Stack.Count == 0)
        {
            t = this.m_objCreator();
            this.CountAll++;
        }
        else
        {
            t = this.m_Stack.Pop();
        }
        if (this.m_ActionOnGet != null)
        {
            this.m_ActionOnGet(t);
        }
        return t;
    }

    public void Release(T element)
    {
        if (this.m_ActionOnRelease != null)
        {
            this.m_ActionOnRelease(element);
        }
        this.m_Stack.Push(element);
    }

    public void Clear()
    {
        GTObjectPool<T>.s_AllPool.Clear();
    }
}

