using UnityEngine;
using System.Collections;
using NLE;
using System;

namespace NLE
{
    public class NFactory
    {
        public static NGroup      CreateGroup(NSequencer sequencer, Type type, string label)
        {
            GameObject go = new GameObject(label, type);
            go.transform.parent = sequencer.transform;
            return go.GetComponent<NGroup>();
        }

        public static NItem       CreateTrack(NGroup group, Type type, string label)
        {
            GameObject go = new GameObject(label, type);
            go.transform.parent = group.transform;
            return go.GetComponent<NItem>();
        }
    }
}

