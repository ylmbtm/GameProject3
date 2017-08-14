using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using Protocol;

public class UIDialogue : GTWindow
{
    private GameObject      btnNext;
    private GameObject      btnSkip;
    private UILabel         dlgTitle;
    private UILabel         dlgContent;
    private UITexture       dlgTexture;
    private int             dlgStID;
    private int             dlgEdID;
    private int             dlgCurID;
    private ERender         dlgRenderer;
    private CharacterAvatar dlgAvatar;
    private Callback        onFinish;

    public UIDialogue()
    {
        Resident = false;
        Path = "Common/UIDialogue";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.WhiteTransparent;
        ShowMode = EWindowShowMode.DoNothing;
    }

    protected override void OnAwake()
    {
        Transform bottom = transform.Find("Bottom");
        Transform top    = transform.Find("Top");
        this.btnNext     = bottom.Find("BtnNext").gameObject;
        this.btnSkip     = top.Find("BtnSkip").gameObject;
        this.dlgTitle    = bottom.Find("DlgTitle").GetComponent<UILabel>();
        this.dlgContent  = bottom.Find("DlgContent").GetComponent<UILabel>();
        this.dlgTexture  = bottom.Find("DlgTexture").GetComponent<UITexture>();
    }

    protected override void OnEnable()
    {

    }

    protected override void OnClose()
    {
        if (dlgRenderer != null)
        {
            dlgRenderer.Release();
            dlgRenderer = null;
        }
        if (dlgAvatar != null)
        {
            dlgAvatar = null;
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnNext).onClick = OnBtnNextClick;
        UIEventListener.Get(btnSkip).onClick = OnBtnSkipClick;
    }

    protected override void OnAddHandler()
    {
        
    }

    protected override void OnDelHandler()
    {
        
    }

    private void OnBtnNextClick(GameObject go)
    {
        this.GoNext();
        this.ShowDialogue();
    }

    private void OnBtnSkipClick(GameObject go)
    {
        if (onFinish != null)
        {
            onFinish();
            onFinish = null;
        }
        Hide();
    }

    private void GoNext()
    {
        if (this.dlgCurID == this.dlgEdID)
        {
            if (onFinish != null)
            {
                onFinish();
                onFinish = null;
            }
            Hide();
            dlgCurID = -1;
            return;
        }

        if (this.dlgCurID < dlgStID)
        {
            this.dlgCurID = dlgStID;
        }
        else
        {
            this.dlgCurID++;
        }

    }

    private void ShowDialogue()
    {
        DDialogue db = ReadCfgDialogue.GetDataById(dlgCurID);
        if (db == null)
        {
            return;
        }
        ShowDialogueTitle(db);
        ShowDialogueContent(db);
        ShowDialogueModel(db);
    }

    private void ShowDialogueModel(DDialogue db)
    {
        DActor actorDB = null;
        if (db.Actor == 0)
        {
            XCharacter role = GTGlobal.Main;
            actorDB = ReadCfgActor.GetDataById(role.Id);
        }
        else
        {
            actorDB = ReadCfgActor.GetDataById(db.Actor);
        }

        if (actorDB == null)
        {
            return;
        }
        if (dlgRenderer != null)
        {
            dlgRenderer.DelAllModels();
        }
        else
        {
            dlgRenderer = ERender.AddRender(dlgTexture);
        }
        dlgAvatar = GTWorld.Instance.AddAvatar(actorDB.Model);
        dlgAvatar.PlayAnim(db.Anim, null);
        dlgRenderer.AddModel(dlgAvatar.GetRootObj());
        dlgAvatar.GetRootObj().transform.localPosition = new Vector3(-0.22f, -1.61f, 1.0f);
        dlgAvatar.GetRootObj().transform.localEulerAngles = new Vector3(0, 180, 0);
    }

    private void ShowDialogueTitle(DDialogue db)
    {
        if (db.Actor == 0)
        {
            Character c = GTWorld.Main;
            dlgTitle.text = c == null ? string.Empty : c.Name;
        }
        else
        {
            DActor actorDB = ReadCfgActor.GetDataById(db.Actor);
            dlgTitle.text = actorDB == null ? string.Empty : actorDB.Name;
        }
    }

    private void ShowDialogueContent(DDialogue db)
    {

        string replaceName = string.Empty;
        switch (db.ContentType)
        {
            case EDialogueContentType.TYPE_NONE:
                {
                    replaceName = string.Empty;
                }
                break;
            case EDialogueContentType.TYPE_PLAYER:
                {
                    Character c = GTWorld.Main;
                    replaceName = c == null ? string.Empty : c.Name;
                }
                break;
            case EDialogueContentType.TYPE_ACTOR:
                {
                    DActor actorDB = ReadCfgActor.GetDataById(db.ContentID);
                    replaceName = actorDB == null ? string.Empty : actorDB.Name;
                }
                break;
            case EDialogueContentType.TYPE_ITEM:
                {
                    DItem itemDB = ReadCfgItem.GetDataById(db.ContentID);
                    replaceName = itemDB == null ? string.Empty : itemDB.Name;
                }
                break;
            case EDialogueContentType.TYPE_MAP:
                {
                    DCopyMainChapter worldDB = ReadCfgCopyMainChapter.GetDataById(db.ContentID);
                    replaceName = worldDB == null ? string.Empty : worldDB.Name;
                }
                break;
        }
        dlgContent.text = db.Content.Replace("%p%", replaceName);

        switch (db.ContentShowType)
        {
            case EDialogueContentShowType.Normal:
                dlgContent.GetComponent<TypewriterEffect>().enabled = false;
                break;
            case EDialogueContentShowType.Effect:
                dlgContent.GetComponent<TypewriterEffect>().enabled = true;
                dlgContent.GetComponent<TypewriterEffect>().ResetToBeginning();
                break;
        }
    }

    public void  ShowDialogue(int st, int ed, bool isCanSkip, Callback onFinish)
    {
        this.dlgStID = st;
        this.dlgEdID = ed;
        this.onFinish = onFinish;
        this.GoNext();
        this.ShowDialogue();
        this.btnSkip.SetActive(isCanSkip);
    }
}