using UnityEngine;
using System.Collections;
using CFG;

namespace MAP
{
    public class FTPoint : FTElement
    {
        public override DCFG Export()
        {
            MapPoint data       = new MapPoint();
            data.Pos            = Pos;
            data.EulerAngles    = EulerAngles;
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapPoint data       = cfg as MapPoint;
            this.Pos            = data.Pos;
            this.EulerAngles    = data.EulerAngles;
        }
    }
}