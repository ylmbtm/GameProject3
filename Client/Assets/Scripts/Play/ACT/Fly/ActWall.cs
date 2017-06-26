using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActWall : ActObj
    {
        public enum EActWallType
        {
            STOP_MISSILE,
            STOP_ENEMY,
        }

        [SerializeField]
        public EBind        CasterBind = EBind.Head;
        [SerializeField]
        public EBind        TargetBind = EBind.Head;
        [SerializeField]
        public EActWallType WallType   = EActWallType.STOP_MISSILE;
        [SerializeField] 
        public float        L          = 5;
        [SerializeField]
        public float        W          = 4;
        [SerializeField]
        public float        H          = 2;
    }
}
