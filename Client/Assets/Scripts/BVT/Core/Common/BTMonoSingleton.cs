using System.Threading;
using UnityEngine;

namespace BVT
{
    public abstract class BTMonoSingleton<T> : MonoBehaviour where T : BTMonoSingleton<T>
    {
        private static T      instance = null;
        private static object lockObject = new object();

        public  static T Instance
        {
            get
            {
                if (instance == null)
                {
                    object obj = lockObject;
                    Monitor.Enter(obj);
                    instance = GameObject.FindObjectOfType(typeof(T)) as T;
                    if (instance == null)
                    {
                        instance = new GameObject(typeof(T).Name, typeof(T)).GetComponent<T>();
                    }
                    Monitor.Exit(obj);
                }
                return instance;
            }
        }
    }
}
