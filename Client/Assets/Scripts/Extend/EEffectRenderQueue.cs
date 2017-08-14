using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class EEffectRenderQueue : MonoBehaviour
{
    public int     queueOffset=1;
    public bool    forward = true;
    public UIPanel panel;
    private List<ParticleSystem> mParticles = new List<ParticleSystem>();

    void Awake()
    {
        panel = NGUITools.FindInParents<UIPanel>(gameObject);
        GetComponents<ParticleSystem>(mParticles);
        GetComponentsInChildren<ParticleSystem>(mParticles);
        if (panel == null)
        {
            return;
        }
        int thisQueue = 0;
        if (forward)
        {
            thisQueue = panel.startingRenderQueue + queueOffset;
        }
        else
        {
            thisQueue = panel.startingRenderQueue - queueOffset;
        }
        for (int i = 0; i < mParticles.Count; i++)
        {
            Renderer render = mParticles[i].GetComponent<Renderer>();
            if (render != null && render.material != null)
            {
                render.material.renderQueue = thisQueue;
            }
        }
    }
}
