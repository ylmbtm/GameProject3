using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTChunk
    {
        public EChunkType Type      = EChunkType.TYPE_BUILD;
        public float      Threshold = 100;
        public float      CenterX   = 100;
        public float      CenterY   = 100;
    }
}