using UnityEngine;
using System.Collections;
using System;

public class CharacterBoard : ICharacterComponent
{
    private Character mOwner;
    private UIBoard   mBoard = null;

    public CharacterBoard(Character owner)
    {
        this.mOwner = owner;
        if (GTLauncher.Instance.CurrSceneType != ESceneType.TYPE_CITY &&
            GTLauncher.Instance.CurrSceneType != ESceneType.TYPE_WORLD &&
            GTLauncher.Instance.CurrSceneType != ESceneType.TYPE_AREA)
        {
            return;
        }
        if (mOwner.Type == EActorType.NPC ||
            mOwner.Type == EActorType.PLAYER ||
            mOwner.Type == EActorType.MONSTER)
        {
            this.mBoard = GTWorld.Instance.Hud.Create(mOwner);
        }
    }

    public void Show()
    {
        if (mBoard == null)
        {
            return;
        }
        switch (mOwner.Type)
        {
            case EActorType.PLAYER:
                {
                    mBoard.SetVisable(true);
                    string text1 = GTTools.Format("<#00ff35>Lv.{0} {1}</color>", mOwner.Level, mOwner.Name);
                    string text2 = GTTools.Format("<#00ff35><{0}></color>", "单身狗");
                    mBoard.Show(text1, text2, 1);
                }
                break;
            case EActorType.NPC:
                {
                    mBoard.SetVisable(false);
                    string text1 = GTTools.Format("<#0000ff>{0}</color>",   mOwner.Name);
                    string text2 = GTTools.Format("<#0000ff><{0}></color>", "魔教");
                    mBoard.Show(text1, text2);
                }
                break;
            case EActorType.MONSTER:
                {
                    mBoard.SetVisable(false);
                    string text1 = GTTools.Format("<#ff0000>{0}</color>", mOwner.Name);
                    string text2 = null;
                    mBoard.Show(text1, text2);
                }
                break;
        }
    }

    public void SetHeight(float height)
    {
        if (mBoard == null)
        {
            return;
        }
        mBoard.SetHeight(height);
    }

    public void Execute()
    {

    }

    public void Release()
    {
        GTWorld.Instance.Hud.Release(mOwner);
    }

    public void SetActive(bool active)
    {
        if (mBoard != null)
        {
            mBoard.enabled = active;
        }
    }
}