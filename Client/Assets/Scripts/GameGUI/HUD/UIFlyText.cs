using UnityEngine;
using System.Collections;
using TMPro;

public class UIFlyText : MonoBehaviour
{
    public AnimationCurve     moveCurve;
    public AnimationCurve     scaleCurve;
    public AnimationCurve     alphaCurve;
    public TextMeshPro        textMesh;
    public Vector3            offset          = new Vector3(0, 150, 0);
    public float              lifeTime        = 1;

    private float             mStartTime      = 0;
    private Vector3           mStartPos       = Vector3.zero;
    private float             mStartFontSize  = 1f;
    private bool              mIsPlaying      = false;
    private float             mEnlarge        = 1;
    private Keyframe[]        offsets;
    private Keyframe[]        alphas;
    private Keyframe[]        scales;

    public string Text
    {
        get { return textMesh.text; }
        set { textMesh.text = value; }
    }

    public string Path
    {
        get; set;
    }

    public Color  TextColor
    {
        get { return textMesh.color; }
        set { textMesh.color = value; }
    }

    public float  TextAlpha
    {
        get { return textMesh.alpha; }
        set { textMesh.alpha = value; }
    }

    public float  TextScale
    {
        get { return textMesh.fontSize; }
        set { textMesh.fontSize = value; }
    }

    public float  TextEnlarge
    {
        get { return mEnlarge; }
        set { mEnlarge = value; }
    }

    public void Init(Vector3 pos)
    {
        mStartPos = pos;
        transform.localPosition = pos;
        mStartTime = Time.realtimeSinceStartup;
        Invoke("Release", lifeTime);
        mIsPlaying = true;
        GetComponent<MeshRenderer>().material.renderQueue = 2970;
        offsets = moveCurve.keys;
        alphas = alphaCurve.keys;
        scales = scaleCurve.keys;
    }

    void Awake()
    {
        mStartFontSize = TextScale;
    }

    void Update()
    {
        if (mIsPlaying == false)
        {
            return;
        }
        float time = Time.realtimeSinceStartup;
        float offsetEnd = offsets[offsets.Length - 1].time;
        float alphaEnd = alphas[alphas.Length - 1].time;
        float scalesEnd = scales[scales.Length - 1].time;
        float totalEnd = Mathf.Max(scalesEnd, Mathf.Max(offsetEnd, alphaEnd));
        float currentTime = time - mStartTime;
        float o = moveCurve.Evaluate(currentTime);
        float a = alphaCurve.Evaluate(currentTime);
        float s = scaleCurve.Evaluate(currentTime);
        TextAlpha = a;
        TextScale = s * mStartFontSize * mEnlarge;
        transform.localPosition = mStartPos + o * offset;
    }

    void Release()
    {
        Text = string.Empty;
        GTPoolManager.Instance.ReleaseGo(Path, gameObject);
        mIsPlaying = false;
    }

    void OnDestroy()
    {
        CancelInvoke();
    }
}
