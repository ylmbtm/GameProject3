using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class EHUDToast : MonoBehaviour
{

    protected class Entry
    {
        public float time;			// Timestamp of when this entry was added
        public float stay = 0f;		// How long the text will appear to stay stationary on the screen
        public float offset = 100f;	// How far the object has moved based on time
        public float val = 0f;		// Optional value (used for damage)
        public UILabel label;		// Label on the game object
        public float movementStart { get { return time + stay; } }
    }

    static int Comparison(Entry a, Entry b)
    {
        if (a.movementStart < b.movementStart) return -1;
        if (a.movementStart > b.movementStart) return 1;
        return 0;
    }

    public Font font;
    public UILabel.Effect effect = UILabel.Effect.None;
    public int  fontSize = 16;
    public float offset = 100;

    /// <summary>
    /// Curve used to move entries with time.
    /// </summary>
    public AnimationCurve offsetCurve = new AnimationCurve(new Keyframe[] { new Keyframe(0f, 200f), new Keyframe(0.5f, 200), new Keyframe(1f, 200), new Keyframe(2f, 300) });

    /// <summary>
    /// Curve used to fade out entries with time.
    /// </summary>
    public AnimationCurve alphaCurve = new AnimationCurve(new Keyframe[] { new Keyframe(0f, 0f), new Keyframe(0.1f, 1), new Keyframe(1f, 1), new Keyframe(2f, 0f) });

    /// <summary>
    /// Curve used to scale the entries.
    /// </summary>
    public AnimationCurve scaleCurve = new AnimationCurve(new Keyframe[] { new Keyframe(0f, 0f), new Keyframe(0.25f, 1.3f), new Keyframe(0.5f, 1f), new Keyframe(2f, 1f) });

    List<Entry> mList = new List<Entry>();
    List<Entry> mUnused = new List<Entry>();

    int counter = 0;
    public bool isVisible { get { return mList.Count != 0; } }


    Entry Create()
    {
        if (mUnused.Count > 0)
        {
            Entry ent = mUnused[mUnused.Count - 1];
            mUnused.RemoveAt(mUnused.Count - 1);
            ent.time = Time.realtimeSinceStartup;
            ent.label.depth = NGUITools.CalculateNextDepth(gameObject);
            NGUITools.SetActive(ent.label.gameObject, true);
            ent.offset = offset;
            mList.Add(ent);
            return ent;
        }

        Entry ne = new Entry();
        ne.time = Time.realtimeSinceStartup;
        ne.label = NGUITools.AddWidget<UILabel>(gameObject);
        ne.label.name = "Label"+counter;
        ne.label.effectStyle = effect;
        ne.label.trueTypeFont = font;
        ne.label.fontSize = fontSize;
        ne.label.alignment = NGUIText.Alignment.Justified;
        ne.label.overflowMethod = UILabel.Overflow.ResizeFreely;
        mList.Add(ne);
        ++counter;
        return ne;
    }

    /// <summary>
    /// Delete the specified entry, adding it to the unused list.
    /// </summary>

    void Delete(Entry ent)
    {
        mList.Remove(ent);
        mUnused.Add(ent);
        NGUITools.SetActive(ent.label.gameObject, false);
      //  NGUITools.SetActive(ent.sprite.gameObject, false);
    }

    /// <summary>
    /// Add a new scrolling text entry.
    /// </summary>

    public void Add(object obj, Color c, float stayDuration)
    {
        if (!enabled) return;

        float time = Time.realtimeSinceStartup;

        // Create a new entry
        Entry ne = Create();
        ne.stay = stayDuration;
        ne.label.color = c;
        ne.label.alpha =0;
        ne.label.text = obj.ToString();
        mList.Sort(Comparison);
    }

    void OnDisable()
    {
        for (int i = mList.Count; i > 0; )
        {
            Entry ent = mList[--i];
            if (ent.label != null) ent.label.enabled = false;
            else mList.RemoveAt(i);
        }
    }

    void Update()
    {
        float time = Time.realtimeSinceStartup;

        Keyframe[] offsets = offsetCurve.keys;
        Keyframe[] alphas = alphaCurve.keys;
        Keyframe[] scales = scaleCurve.keys;

        float offsetEnd = offsets[offsets.Length - 1].time;
        float alphaEnd = alphas[alphas.Length - 1].time;
        float scalesEnd = scales[scales.Length - 1].time;
        float totalEnd = Mathf.Max(scalesEnd, Mathf.Max(offsetEnd, alphaEnd));
        for (int i = mList.Count; i > 0; )
        {
            Entry ent = mList[--i];
            float currentTime = time - ent.movementStart;
            ent.offset = offsetCurve.Evaluate(currentTime);
            ent.label.alpha = alphaCurve.Evaluate(currentTime);
            float s = scaleCurve.Evaluate(time - ent.time);
            if (s < 0.001f) s = 0.001f;
            ent.label.cachedTransform.localScale = new Vector3(s, s, s);
            if (currentTime > totalEnd)
            { 
                Delete(ent);
            }
            else
            { 
                ent.label.enabled = true;
            }
        }

        float offset =0f;

        for (int i = mList.Count; i > 0; )
        {
            Entry ent = mList[--i];
            ent.label.cachedTransform.localPosition = new Vector3(0f, offset, 0f);
            offset += fontSize*1.2f;
        }
    }
}
