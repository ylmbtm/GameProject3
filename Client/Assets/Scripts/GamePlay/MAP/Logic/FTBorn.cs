using UnityEngine;
using System.Collections;
using System;
using CFG;

namespace MAP
{
    public class FTBorn : FTComponent
    {
        public override void Startup()
        {

        }

        public override void Trigger()
        {

        }

        public override void Release()
        {

        }

        public override DCFG Export()
        {
            MapBorn data = new MapBorn();
            data.ID      = this.ID;
            data.Pos     = this.Pos;
            data.Face    = this.Face;
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapBorn data = cfg as MapBorn;
            this.ID      = data.ID;
            this.Pos     = data.Pos;
            this.Face    = data.Face;
        }

        public override void OnDrawGizmos()
        {
            transform.localScale = Vector3.one;
            switch ((EBattleCamp)ID)
            {
                case EBattleCamp.CT_PVE_PLAYER:
                    FTDraw.DrawGizmosCapsule(transform.position + new Vector3(0, 2, 0), transform.rotation, transform.localScale * 2, Color.green);
                    break;
                case EBattleCamp.CT_PVP_PLAYER1:
                case EBattleCamp.CT_PVP_PLAYER2:
                case EBattleCamp.CT_PVP_PLAYER3:
                case EBattleCamp.CT_PVP_PLAYER4:
                case EBattleCamp.CT_PVP_PLAYER5:
                    FTDraw.DrawGizmosCapsule(transform.position + new Vector3(0, 2, 0), transform.rotation, transform.localScale * 2, Color.cyan);
                    break;
                case EBattleCamp.CT_PVE_ENEMY:
                    FTDraw.DrawGizmosCapsule(transform.position + new Vector3(0, 2, 0), transform.rotation, transform.localScale * 2, Color.red);
                    break;
                case EBattleCamp.CT_PVE_NEUTRAL:
                    FTDraw.DrawGizmosCapsule(transform.position + new Vector3(0, 2, 0), transform.rotation, transform.localScale * 2, Color.yellow);
                    break;
            }
        }

        public override void OnDrawInspector()
        {
#if UNITY_EDITOR
            GUILayout.Space(10);
            this.ID = (int)((EBattleCamp)UnityEditor.EditorGUILayout.EnumPopup("Camp", (EBattleCamp)ID));
            GUILayout.Space(5);
#endif
        }
    }
}