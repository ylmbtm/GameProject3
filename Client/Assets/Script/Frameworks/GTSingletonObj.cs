using UnityEngine;
using System.Collections;

public class GTSingletonObj<T> where T : GTSingletonObj<T>, new()
{
    public static T Instance;

    static GTSingletonObj()
    {
        Instance = new T();
    }
}
