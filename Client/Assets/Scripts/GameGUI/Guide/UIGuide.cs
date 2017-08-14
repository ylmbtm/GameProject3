using UnityEngine;
using System.Collections;
using System;
using BIE;

public class UIGuide : GTWindow
{
    private GameObject mask;
    private Transform  pivot;
    private Transform  hand;
    private UISprite   areaCircle;
    private UISprite   areaRect;
    private GameObject row;
    private Transform  tipTransform;
    private UILabel    tipText;
    private UISprite   tipIcon;
    private GameObject btnSkip;
    private GameObject btnFalse;
     
    public UIGuide()
    {
        Path = "Guide/UIGuide";
        Resident = false;
        Type = EWindowType.NewBie;
        MaskType = EWindowMaskType.None;
        ShowMode = EWindowShowMode.DoNothing;
    }

    protected override void OnAwake()
    {
        mask          = transform.Find("Mask").gameObject;
        pivot         = transform.Find("Pivot");
        hand          = pivot.Find("Hand");
        areaCircle    = pivot.Find("Area_Circle").GetComponent<UISprite>();
        areaRect      = pivot.Find("Area_Rect").GetComponent<UISprite>();
        row           = pivot.Find("Row").gameObject;
        tipTransform = pivot.Find("Tip");
        tipText       = tipTransform.Find("Content").GetComponent<UILabel>();
        tipIcon       = tipTransform.Find("Icon").GetComponent<UISprite>();
        btnSkip       = pivot.Find("Top/BtnSkip").gameObject;
        btnFalse      = pivot.Find("BtnFalse").gameObject;

        hand.gameObject.SetActive(false);
        areaCircle.gameObject.SetActive(false);
        areaRect.gameObject.SetActive(false);
        row.gameObject.SetActive(false);
        tipTransform.gameObject.SetActive(false);
        tipText.text = string.Empty;
        btnSkip.SetActive(false);
        btnFalse.SetActive(false);
    }

    protected override void OnEnable()
    {
        
    }

    protected override void OnAddButtonListener()
    {
  
    }

    protected override void OnAddHandler()
    {
        
    }

    protected override void OnClose()
    {
        
    }

    protected override void OnDelHandler()
    {
        
    }

    public void ShowGuideBase(GuideBase data)
    {
        mask.SetActive(data.IsLocked);
        btnSkip.SetActive(data.IsShowSkip);
        UIEventListener.Get(btnSkip).onClick = (go) =>
        {
            data.Finish();
        };
    }

    public void ShowGuideTip(string tipText, Vector2 tipPos, EGuideGirlPos  tipGirlPos)
    {
        if(string.IsNullOrEmpty(tipText))
        {
            this.tipTransform.gameObject.SetActive(false);
        }
        else
        {
            this.tipTransform.gameObject.SetActive(true);
            this.tipTransform.localPosition = tipPos;
            this.tipText.text = tipText;
        }
        switch(tipGirlPos)
        {
            case EGuideGirlPos.TYPE_NONE:
                tipIcon.gameObject.SetActive(false);
                break;
            case EGuideGirlPos.TYPE_LEFT:
                tipIcon.gameObject.SetActive(true);
                tipIcon.transform.localScale = new Vector3(-1, 1, 1);
                tipIcon.transform.localPosition = new Vector3(-100, 0, 0);
                break;
            case EGuideGirlPos.TYPE_RIGHT:
                tipIcon.gameObject.SetActive(true);
                tipIcon.transform.localScale = new Vector3(1, 1, 1);
                tipIcon.transform.localPosition = new Vector3(100, 0, 0);
                break;
        }
    }

    public void ShowGuideOperation(Transform target, GuideOperation data)
    {
        GameObject btn = null;
        DoGuideInstantie(target, data, ref btn);
        if (btn == null)
        {
            btnFalse.SetActive(true);
            UIEventListener.Get(btnFalse).onClick = (go) =>
            {
                data.Finish();
            };
            return;    
        }
        UIEventListener ul = target.GetComponent<UIEventListener>();

        switch (data.OperationType)
        {
            case EGuideUIOperationType.TYPE_CLICK:
                UIEventListener.Get(btn).onClick = (go) =>
                {
                    if (ul != null && ul.onClick != null)
                    {
                        ul.onClick(go);
                    }
                    data.Finish();
                    UnityEngine.GameObject.DestroyImmediate(btn);
                };
                break;
            case EGuideUIOperationType.TYPE_PRESS:
                UIEventListener.Get(btn).onPress = (go, state) =>
                {
                    if (ul != null && ul.onPress != null)
                    {
                        ul.onPress(go, state);
                    }
                    data.Finish();
                    UnityEngine.GameObject.DestroyImmediate(btn);
                };
                break;
            case EGuideUIOperationType.TYPE_SWAP:
                UIEventListener.Get(btn).onDrag = (go, delta) =>
                {
                    if (ul != null && ul.onDrag != null)
                    {
                        ul.onDrag(go, delta);
                    }
                    data.Finish();
                    UnityEngine.GameObject.DestroyImmediate(btn);
                };
                break;
        }
    }

    public void DoGuideInstantie(Transform target, GuideOperation data, ref GameObject btn)
    {
        if (target == null)
        {
            return;
        }
        BoxCollider collider = target.GetComponent<BoxCollider>();
        if (collider == null)
        {
            return;
        }
        btn = NGUITools.AddChild(pivot.gameObject, target.gameObject);
        btn.transform.position = target.transform.position;

        switch (data.BoardType)
        {
            case EGuideBoardType.TYPE_NONE:
                areaCircle.gameObject.SetActive(false);
                areaRect.gameObject.SetActive(false);
                break;
            case EGuideBoardType.TYPE_RECTANGLE:
                areaCircle.gameObject.SetActive(false);
                areaRect.gameObject.SetActive(true);
                areaRect.width = (int)data.BoardSize.x;
                areaRect.height = (int)data.BoardSize.y;
                areaRect.transform.localPosition = btn.transform.localPosition;
                break;
            case EGuideBoardType.TYPE_CIRCLE:
                areaCircle.gameObject.SetActive(true);
                areaRect.gameObject.SetActive(false);
                areaCircle.width = (int)data.BoardSize.x;
                areaCircle.height = (int)data.BoardSize.y;
                areaCircle.transform.localPosition = btn.transform.localPosition;
                break;
        }

        float r1 = 100;
        float r2 = 70;
        float x = data.BoardSize.x * 0.5f;
        float y = data.BoardSize.y * 0.5f;

        switch (data.RowType)
        {
            case EGuideRowType.TYPE_NONE:
                row.gameObject.SetActive(false);
                break;
            case EGuideRowType.TYPE_UP:
                row.gameObject.SetActive(true);
                row.transform.localEulerAngles = new Vector3(0, 0, 270);
                row.transform.localPosition = btn.transform.localPosition + new Vector3(0, y + r1, 0);

                break;
            case EGuideRowType.TYPE_UP_RIGHT:
                row.gameObject.SetActive(true);
                row.transform.localEulerAngles = new Vector3(0, 0, 225);
                row.transform.localPosition = btn.transform.localPosition + new Vector3(x + r2, y + r2, 0);
                break;
            case EGuideRowType.TYPE_RIGHT:
                row.gameObject.SetActive(true);
                row.transform.localEulerAngles = new Vector3(0, 0, 180);
                row.transform.localPosition = btn.transform.localPosition + new Vector3(x + r1, 0, 0);
                break;
            case EGuideRowType.TYPE_DOWN_RIGHT:
                row.gameObject.SetActive(true);
                row.transform.localEulerAngles = new Vector3(0, 0, 135);
                row.transform.localPosition = btn.transform.localPosition + new Vector3(x + r2, -y - r2, 0);
                break;
            case EGuideRowType.TYPE_DOWN:
                row.gameObject.SetActive(true);
                row.transform.localEulerAngles = new Vector3(0, 0,  90);
                row.transform.localPosition = btn.transform.localPosition + new Vector3(0, -y - r1, 0);
                break;
            case EGuideRowType.TYPE_DOWN_LEFT:
                row.gameObject.SetActive(true);
                row.transform.localEulerAngles = new Vector3(0, 0, 45);
                row.transform.localPosition = btn.transform.localPosition + new Vector3(-x - r2, -y - r2, 0);
                break;
            case EGuideRowType.TYPE_LEFT:
                row.gameObject.SetActive(true);
                row.transform.localEulerAngles = new Vector3(0, 0, 0);
                row.transform.localPosition = btn.transform.localPosition + new Vector3(-x - r1, 0, 0);
                break;
            case EGuideRowType.TYPE_UP_LEFT:
                row.gameObject.SetActive(true);
                row.transform.localEulerAngles = new Vector3(0, 0, -45);
                row.transform.localPosition = btn.transform.localPosition + new Vector3(-x - r2, y + r2, 0);
                break;         
        }
    }
}
