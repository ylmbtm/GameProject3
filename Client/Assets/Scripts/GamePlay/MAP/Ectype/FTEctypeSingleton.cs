using UnityEngine;
using System.Collections;
using System.Threading;
using System;

namespace MAP
{
    public class FTEctypeSingleton<T> : IEctype where T : class, new()
    {
        static T _instance;
        static object _lock = new object();

        public static T Instance
        {
            get
            {
                if (_instance == null)
                {
                    object lockObject = _lock;
                    Monitor.Enter(lockObject);
                    try
                    {
                        if (_instance == null)
                            _instance = Activator.CreateInstance<T>();
                    }
                    finally
                    {
                        Monitor.Exit(lockObject);
                    }
                }
                return _instance;
            }
        }

        public float StTime
        {
            get; set;
        }

        public float EdTime
        {
            get; set;
        }

        public float CurrTime
        {
            get { return Time.realtimeSinceStartup - StTime; }
        }

        public float PastTime
        {
            get { return EdTime - StTime; }
        }

        public virtual void Enter()
        {
            this.StTime = Time.realtimeSinceStartup;
            this.EdTime = Time.realtimeSinceStartup;
        }

        public virtual void EnterEctypeStep(int id)
        {

        }

        public virtual void Execute()
        {

        }

        public virtual void Exit()
        {
            this.EdTime = Time.realtimeSinceStartup;
        }
    }
}