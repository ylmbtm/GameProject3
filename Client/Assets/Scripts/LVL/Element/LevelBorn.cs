using UnityEngine;
using System.Collections;
using CFG;

namespace LVL
{
    [LevelElementVariable(CfgType = typeof(MapBorn))]
    public class LevelBorn : LevelElement
    {
        [LevelVariable]
        public EBattleCamp Camp = EBattleCamp.A;

        private GameObject   m_Body;
        private MeshRenderer m_MeshRenderer;
        private Material     m_Material;

        public override void Build()
        {
            if (m_Body == null)
            {
                m_Body = GameObject.CreatePrimitive(PrimitiveType.Cube);
                m_Body.transform.ResetLocalTransform(transform);
            }
            m_MeshRenderer = m_Body.GetComponent<MeshRenderer>();
            if (m_MeshRenderer == null)
            {
                return;
            }
            m_Material = m_MeshRenderer.material;
            if (m_Material != null)
            {
                Shader shader = Shader.Find("Custom/TranspUnlit");
                m_Material = new Material(shader) { hideFlags = HideFlags.HideAndDontSave };
            }
            switch (Camp)
            {
                case EBattleCamp.A:
                    m_Material.color = Color.green;
                    break;
                case EBattleCamp.B:
                    m_Material.color = Color.blue;
                    break;
                case EBattleCamp.C:
                    m_Material.color = Color.yellow;
                    break;
            }
        }
    }
}