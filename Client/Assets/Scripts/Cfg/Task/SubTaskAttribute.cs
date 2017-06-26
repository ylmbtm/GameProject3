using UnityEngine;
using System.Collections;
using System;

namespace CFG
{
    public class SubTaskAttribute : Attribute
    {
        public string            Label = string.Empty;
        public ETaskSubFuncType  Func = ETaskSubFuncType.TYPE_TALK;
    }
}

