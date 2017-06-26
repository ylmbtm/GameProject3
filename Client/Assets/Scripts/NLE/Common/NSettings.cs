using UnityEngine;
using System.Collections;

namespace NLE
{
#if UNITY_EDITOR
    public class NSettings
    {

        public static int NSequencerID
        {
            get { return UnityEditor.EditorPrefs.GetInt("NSequencerID", -1); }
            set { UnityEditor.EditorPrefs.SetInt("NSequencerID", value); }
        }       
    }
#endif
}

