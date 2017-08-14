using UnityEngine;
using System.Collections;

namespace MAP
{
    public interface IEctype
    {
        float StTime   { get;}
        float EdTime   { get;}
        float CurrTime { get;}
        float PastTime { get;}
        void  Enter();
        void  EnterEctypeStep(int id);
        void  Execute();
        void  Exit();
    }
}