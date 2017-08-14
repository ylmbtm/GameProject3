using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using DG.Tweening;

public class CharacterAvatar : ICharacterComponent
{
    private Transform                   mRoot;
    private Transform                   mShadow;
    private GameObject                  mWeapon1;
    private GameObject                  mWeapon2;
    private TrailRenderer               mWeaponTrail1;
    private TrailRenderer               mWeaponTrail2;
    private Material                    mWeaponMat1;
    private Material                    mWeaponMat2;
    private Shader                      mWeaponShader1;
    private Shader                      mWeaponShader2;
    private SkinnedMeshRenderer         mSkinRenderer;
    private Material                    mSkinMaterial;
    private Shader                      mSkinShader;
    private Animator                    mAnimator;

    private GameObject                  mRootObj;
    private Transform                   mHand1;
    private Transform                   mHand2;
    private Transform                   mBodyTrans;
    private Transform                   mHeadTrans;
    private Transform                   mBuffTrans;
    private Transform                   mRidePoint;
    private int[]                       mEquipArray;


    public CharacterAvatar(Transform root)
    {
        this.mRoot          = root;
        this.mRootObj        = mRoot.gameObject;
        this.mRidePoint      = GTTools.GetBone(mRoot, "Bone026");
        this.mHand1          = GTTools.GetBone(mRoot, "Bip01 Prop1");
        this.mHand2          = GTTools.GetBone(mRoot, "Bip01 Prop2");
        this.mBodyTrans      = GTTools.GetBone(mRoot, "BP_Spine");
        this.mHeadTrans      = GTTools.GetBone(mRoot, "BP_Head");
        this.mBuffTrans      = GTTools.GetBone(mRoot, "BP_Buff");
        this.mAnimator       = mRootObj.GetComponent<Animator>();
        this.mShadow         = mRoot.Find("shadow");
        this.mSkinRenderer   = mRoot.GetComponentInChildren<SkinnedMeshRenderer>();
        this.mSkinMaterial   = this.mSkinRenderer == null ? null : this.mSkinRenderer.material;
        this.mSkinShader     = this.mSkinMaterial == null ? null : this.mSkinMaterial.shader;
        this.mEquipArray     = new int[8];
    }

    public void Execute()
    {

    }

    public void Release()
    {
        GTResourceManager.Instance.DestroyObj(mWeaponMat1);
        GTResourceManager.Instance.DestroyObj(mWeaponMat2);
        GTResourceManager.Instance.DestroyObj(mWeapon1);
        GTResourceManager.Instance.DestroyObj(mWeapon2);
    }

    public void SetWeaponActive(bool active)
    {
        if (mWeapon1 != null)
        {
            mWeapon1.SetActive(active);
        }
        if (mWeapon2 != null)
        {
            mWeapon2.SetActive(active);
        }
    }

    public void SetShadowActive(bool active)
    {
        if (mShadow != null)
        {
            mShadow.gameObject.SetActive(active);
        }
    }

    public void SetTransparentVertLitOn()
    {
        Shader newSkinShader = GTShader.ChangeShader(mSkinMaterial, "MyMobile/Monster/Transparent_VertLit");
        Shader newWeaponShader1 = GTShader.ChangeShader(mWeaponMat1, "MyMobile/Monster/Transparent_VertLit");
        Shader newWeaponShader2 = GTShader.ChangeShader(mWeaponMat2, "MyMobile/Monster/Transparent_VertLit");
        if (mSkinMaterial != null && newSkinShader != null)
        {
            mSkinMaterial.SetFloat("_AlphaCon", 0.2f);
        }
        if (mWeaponMat1 != null && newWeaponShader1 != null)
        {
            mWeaponMat1.SetFloat("_AlphaCon", 0.2f);
        }
        if (mWeaponMat2 != null && newWeaponShader2 != null)
        {
            mWeaponMat2.SetFloat("_AlphaCon", 0.2f);
        }
    }

    public void SetTransparentVertLitOff()
    {
        GTShader.ChangeShader(mSkinMaterial, mSkinShader);
        GTShader.ChangeShader(mWeaponMat1,   mWeaponShader1);
        GTShader.ChangeShader(mWeaponMat2,   mWeaponShader2);
    }

    public void SetHitShaderOn()
    {
        Shader newSkinShader = GTShader.ChangeShader(mSkinMaterial, "MyMobile/Monster/VertLit");
        if (mSkinMaterial != null && newSkinShader != null)
        {
            mSkinMaterial.SetColor("_HitColor", new Color(92f / 255, 0, 0));
        }
    }

    public void SetHitShaderOff()
    {
        GTShader.ChangeShader(mSkinMaterial, mSkinShader);
        if(mSkinShader.name == "MyMobile/Monster/VertLit")
        {
            mSkinMaterial.SetColor("_HitColor", Color.black);
        }
    }

    public void ChangeAvatar(int pos, int id)
    {
        if (mEquipArray[pos - 1] == id)
        {
            return;
        }
        switch (pos)
        {
            case 1:
                ChangeHelmet(id);
                break;
            case 2:
                ChangeNecklace(id);
                break;
            case 3:
                ChangeArmor(id);
                break;
            case 4:
                ChangeShoes(id);
                break;
            case 5:
                ChangeWrist(id);
                break;
            case 6:
                ChangeRing(id);
                break;
            case 7:
                ChangeTalisman(id);
                break;
            case 8:
                ChangeWeapon(id);
                break;
        }
        mEquipArray[pos - 1] = id;
    }

    public void PlayAnim(string animName, Callback onFinish)
    {
        GTAction.Get(mAnimator).Play(animName, onFinish);
    }

    public Transform  GetBindTransform(EBind bind)
    {
        switch(bind)
        {
            case EBind.Head:
                return this.mHeadTrans;
            case EBind.Body:
                return this.mBodyTrans;
            case EBind.Foot:
                return this.mRoot;
            case EBind.LHand:
                return this.mHand1;
            case EBind.RHand:
                return this.mHand2;
            case EBind.Buff:
                return this.mBuffTrans;
            default:
                return null;
        }
    }

    public Vector3    GetBindPosition(EBind bind)
    {
        switch (bind)
        {
            case EBind.Head:
                return this.mHeadTrans == null ? this.mRoot.position + new Vector3(0, 2, 0) : this.mHeadTrans.position;
            case EBind.Body:
                return this.mBodyTrans == null ? this.mRoot.position + new Vector3(0, 1, 0) : this.mBodyTrans.position;
            case EBind.Foot:
                return this.mRoot.position;
            case EBind.LHand:
                return this.mHand1 == null ? this.mRoot.position + new Vector3(0, 1, 0) : this.mHand1.position;
            case EBind.RHand:
                return this.mHand2 == null ? this.mRoot.position + new Vector3(0, 1, 0) : this.mHand2.position;
            case EBind.Buff:
                return this.mBuffTrans == null ? this.mRoot.position + new Vector3(0, 1, 0) : this.mBuffTrans.position;
            default:
                return Vector3.zero;
        }
    }

    public Transform  GetRidePoint()
    {
        return mRidePoint;
    }

    public GameObject GetRootObj()
    {
        return mRootObj;
    }

    void ChangeHelmet(int id)
    {

    }

    void ChangeNecklace(int id)
    {

    }

    void ChangeArmor(int id)
    {

    }

    void ChangeShoes(int id)
    {

    }

    void ChangeWrist(int id)
    {

    }

    void ChangeRing(int id)
    {

    }

    void ChangeTalisman(int id)
    {

    }

    void ChangeWeapon(int id)
    {
        DItem itemDB = ReadCfgItem.GetDataById(id);
        if (itemDB == null)
        {
            return;
        }
        GTResourceManager.Instance.DestroyObj(mWeaponMat1);
        GTResourceManager.Instance.DestroyObj(mWeaponMat2);
        GTResourceManager.Instance.DestroyObj(mWeapon1);
        GTResourceManager.Instance.DestroyObj(mWeapon2);   
        if (mHand1 != null && !string.IsNullOrEmpty(itemDB.Model_R))
        {
            mWeapon1 = GTResourceManager.Instance.Load<GameObject>(itemDB.Model_R, true);
            if (mWeapon1 != null)
            {
                NGUITools.SetLayer(mWeapon1, mRoot.gameObject.layer);
                GTTools.ResetLocalTransform(mWeapon1.transform, mHand1);
                MeshRenderer renderer = mWeapon1.GetComponent<MeshRenderer>();
                if(renderer==null)
                {
                    renderer = mWeapon1.GetComponentInChildren<MeshRenderer>();
                }
                mWeaponMat1 = renderer == null ? null : renderer.material;
                mWeaponShader1 = mWeaponMat1 == null ? null : mWeaponMat1.shader;
            }
        }
        if (mHand2 != null && !string.IsNullOrEmpty(itemDB.Model_L))
        {
            mWeapon2 = GTResourceManager.Instance.Load<GameObject>(itemDB.Model_L, true);
            if (mWeapon2 != null)
            {
                NGUITools.SetLayer(mWeapon2, mRoot.gameObject.layer);
                GTTools.ResetLocalTransform(mWeapon2.transform, mHand2);
                MeshRenderer renderer = mWeapon2.GetComponent<MeshRenderer>();
                if (renderer == null)
                {
                    renderer = mWeapon2.GetComponentInChildren<MeshRenderer>();
                }
                mWeaponMat2 = renderer == null ? null : renderer.material;
                mWeaponShader2 = mWeaponMat2== null ? null : mWeaponMat2.shader;
            }
        }
    }
}