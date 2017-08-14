using UnityEngine;
using System.Collections;
using System;

public class CharacterHUD : ICharacterComponent
{
    private Character  mOwner;
    private HUDBoard   mBoard = null;

    public CharacterHUD(Character owner)
    {
        this.mOwner = owner;
        if (GTLauncher.Instance.CurrCopyType != ECopyType.TYPE_CITY &&
            GTLauncher.Instance.CurrCopyType != ECopyType.TYPE_WORLD &&
            GTLauncher.Instance.CurrCopyType != ECopyType.TYPE_AREA)
        {
            return;
        }
        this.mBoard = GTWorld.Instance.Hud.Create(mOwner);
        this.mBoard.name = GTTools.Format("Character_HUD_{0}_{1}", owner.GUID, owner.ID);
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
                    string text1 = GTTools.Format("<#00F4FF>Lv.{0} {1}</color>", mOwner.Level, mOwner.Name);
                    string text2 = GTTools.Format("<#00F4FF><{0}></color>", "单身狗");
                    mBoard.Show(text1, text2, 1);
                }
                break;
            case EActorType.NPC:
                {
                    mBoard.SetVisable(true);
                    string text1 = GTTools.Format("<#00FF17>{0}</color>",   mOwner.Name);
                    string text2 = GTTools.Format("<#00FF17><{0}></color>", "魔教");
                    mBoard.Show(text1, text2);
                }
                break;
            case EActorType.MONSTER:
                {
                    mBoard.SetVisable(true);
                    string text1 = GTTools.Format("<#FF0000>{0}</color>", mOwner.Name);
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