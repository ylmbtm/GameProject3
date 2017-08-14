using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using TMPro;

public class FlywordSystem : IGameLoop
{
    public const string FLYWORD1 = "Guis/HUD/UIFlyword1";
    public const string FLYWORD2 = "Guis/HUD/UIFlyword2";
    public const string FLYWORD3 = "Guis/HUD/UIFlyword3";
    public const string FLYWORD4 = "Guis/HUD/UIFlyword4";

    public void Execute()
    {

    }

    public void Release()
    {

    }

    public void Show(string value, Vector3 pos, EFlyWordType type)
    {
        Play(value, pos, type);
    }

    string GetPath(EFlyWordType type)
    {
        switch (type)
        {
            case EFlyWordType.TYPE_ENEMY_HURT:
                return FLYWORD2;
            case EFlyWordType.TYPE_ENEMY_CRIT:
                return FLYWORD2;
            case EFlyWordType.TYPE_AVATAR_CRIT:
                return FLYWORD1;
            case EFlyWordType.TYPE_AVATAR_HURT:
                return FLYWORD1;
            case EFlyWordType.TYPE_AVATAR_HEAL:
                return FLYWORD3;
            case EFlyWordType.TYPE_AVATAR_BACKMAGIC:
                return FLYWORD3;
            case EFlyWordType.TYPE_PARTNER_HURT:
            case EFlyWordType.TYPE_PARTNER_CRIT:
                return FLYWORD4;
            default:
                return string.Empty;
        }
    }

    void Play(string value, Vector3 pos, EFlyWordType type)
    {
        string path = GetPath(type);
        GameObject go = GTPoolManager.Instance.GetObject(path);
        if (go == null)
        {
            return;
        }
        UIFlyText flyword = go.GET<UIFlyText>();
        flyword.gameObject.layer = GTLayer.LAYER_UI;
        flyword.Path = path;
        flyword.transform.parent = GTCameraManager.Instance.NGUICamera.transform;
        Vector3 pos_3d = pos;
        Vector2 pos_screen = GTCameraManager.Instance.MainCamera.WorldToScreenPoint(pos_3d);
        Vector3 pos_ui = GTCameraManager.Instance.NGUICamera.ScreenToWorldPoint(pos_screen);
        pos_ui.z = 0;
        switch (type)
        {
            case EFlyWordType.TYPE_ENEMY_HURT:
                flyword.TextColor = Color.red;
                flyword.Text = "－" + value;
                break;
            case EFlyWordType.TYPE_ENEMY_CRIT:
                flyword.TextColor = Color.red;
                flyword.TextEnlarge = 1.2f;
                flyword.Text = "－" + value;
                break;
            case EFlyWordType.TYPE_AVATAR_CRIT:
                flyword.TextEnlarge = 1.5f;
                flyword.Text = "爆击 " + value;
                break;
            case EFlyWordType.TYPE_AVATAR_HURT:
                flyword.Text = value;
                break;
            case EFlyWordType.TYPE_AVATAR_HEAL:
                flyword.TextColor = Color.green;
                flyword.Text = "＋" + value;
                break;
            case EFlyWordType.TYPE_AVATAR_BACKMAGIC:
                flyword.TextColor = new Color(0, 0.7f, 1);
                flyword.Text = "＋" + value;
                break;
            case EFlyWordType.TYPE_PARTNER_HURT:
            case EFlyWordType.TYPE_PARTNER_CRIT:
                flyword.Text = value;
                break;
        }
        flyword.Init(pos_ui);
    }
}
