// ----------------------------------------------------------------------------------
//
// FXMaker
// Created by ismoon - 2012 - ismoonto@gmail.com
//
// ----------------------------------------------------------------------------------

using UnityEngine;

using System.Collections;
using System.Collections.Generic;

public class NcEffectBehaviour : MonoBehaviour
{
	public class _RuntimeIntance
	{
		public GameObject	m_ParentGameObject;
		public GameObject	m_ChildGameObject;
		public _RuntimeIntance(GameObject	parentGameObject, GameObject childGameObject)
		{
			m_ParentGameObject	= parentGameObject;
			m_ChildGameObject	= childGameObject;
		}
	}

	// Attribute ------------------------------------------------------------------------
	private	static	bool			m_bShuttingDown		= false;
	public			float			m_fUserTag;
	protected		MeshFilter		m_MeshFilter;

	public NcEffectBehaviour()
	{
		m_MeshFilter	= null;
	}

	// Property -------------------------------------------------------------------------
	public static float GetEngineTime()
	{
		if (Time.time == 0)
			return 0.000001f;
		return Time.time;
	}

	public static float GetEngineDeltaTime()
	{
		return Time.deltaTime;
	}

#if UNITY_EDITOR
	public virtual string CheckProperty()
	{
		return "";	// no error
	}

	protected bool IsCreatingEditObject()
	{
		GameObject	main	= GameObject.Find("_FXMaker");
		if (main == null)
			return false;
		GameObject	effroot	= FindRootEditorEffect();
		if (effroot == null)
			return false;
		return (effroot.transform.childCount == 0);
	}

	protected GameObject GetFXMakerGameObject()
	{
		return GameObject.Find("_FXMaker");
	}

	public static GameObject FindRootEditorEffect()
	{
		GameObject	parentObj = GameObject.Find("_CurrentObject");
		return parentObj;
	}

// 	protected FXMakerMain GetFXMakerMain()
// 	{
// 		// find toolmain
// 		GameObject fxMaker = GameObject.Find("_FXMaker");
// 		if (Application.isPlaying && fxMaker != null)
// 			return fxMaker.GetComponent<FXMakerMain>();
// 		return null;
// 	}

	protected int GetEditingUvComponentCount()
	{
		int nCount = 0;
		if (GetComponent<NcSpriteAnimation>() != null)
			nCount++;
		if (GetComponent<NcUvAnimation>() != null)
			nCount++;
		if (GetComponent<NcTilingTexture>() != null)
			nCount++;
		if (GetComponent<NcSpriteTexture>() != null)
			nCount++;
		return nCount;
	}
#endif
	public virtual int GetAnimationState()	// 1 = ing, 0 = stop, -1 = none
	{
		return -1;
	}


	public static GameObject GetRootInstanceEffect()
	{
		if (IsSafe() == false)
			return null;
		GameObject	parentObj = GameObject.Find("_InstanceObject");
		if (parentObj == null)
			parentObj = new GameObject("_InstanceObject");
		return parentObj;
	}

	protected static void SetActive(GameObject target, bool bActive)
	{
#if (UNITY_4_0 || UNITY_4_1 || UNITY_4_2 || UNITY_4_3 || UNITY_4_4 || UNITY_4_5 || UNITY_4_6 || UNITY_4_7 || UNITY_4_8 || UNITY_4_9)
		target.SetActive(bActive);
#else
        target.SetActive(bActive);
#endif
    }

	protected static void SetActiveRecursively(GameObject target, bool bActive)
	{
#if (UNITY_4_0 || UNITY_4_1 || UNITY_4_2 || UNITY_4_3 || UNITY_4_4 || UNITY_4_5 || UNITY_4_6 || UNITY_4_7 || UNITY_4_8 || UNITY_4_9)
		for (int n = target.transform.childCount-1; 0 <= n; n--)
			if (n < target.transform.childCount)
				SetActiveRecursively(target.transform.GetChild(n).gameObject, bActive);
		target.SetActive(bActive);
#else
		target.SetActive(bActive);
#endif
	}

	protected static bool IsActive(GameObject target)
	{
#if (UNITY_4_0 || UNITY_4_1 || UNITY_4_2 || UNITY_4_3 || UNITY_4_4 || UNITY_4_5 || UNITY_4_6 || UNITY_4_7 || UNITY_4_8 || UNITY_4_9)
		return (target.activeInHierarchy && target.activeSelf);
#else
		return target.activeSelf;
#endif
	}

	protected static void RemoveAllChildObject(GameObject parent, bool bImmediate)
	{
		for (int n = parent.transform.childCount-1; 0 <= n; n--)
		{
			if (n < parent.transform.childCount)
			{
				Transform	obj = parent.transform.GetChild(n);
				if (bImmediate)
					Object.DestroyImmediate(obj.gameObject);
				else Object.Destroy(obj.gameObject);
			}
// 			obj.parent = null;
// 			Object.Destroy(obj.gameObject);
		}
	}

	public static void HideNcDelayActive(GameObject tarObj)
	{
		SetActiveRecursively(tarObj, false);
/*
		NcDelayActive[]	coms = tarObj.GetComponentsInChildren<NcDelayActive>(true);
		foreach (NcDelayActive com in coms)
			com.CancelDelayActive();
*/
	}

	public static Texture[] PreloadTexture(GameObject tarObj)
	{
		if (tarObj == null)
			return new Texture[0];
		List<GameObject>	parentPrefabList = new List<GameObject>();
		parentPrefabList.Add(tarObj);
		return PreloadTexture(tarObj, parentPrefabList);
	}

	static Texture[] PreloadTexture(GameObject tarObj, List<GameObject> parentPrefabList)
	{
		if (IsSafe() == false)
			return null;
		// texture
		Renderer[]		rens = tarObj.GetComponentsInChildren<Renderer>(true);
		List<Texture>	texList = new List<Texture>();
		foreach (Renderer ren in rens)
		{
			if (ren.sharedMaterials == null || ren.sharedMaterials.Length <= 0)
				continue;
			foreach (Material mat in ren.sharedMaterials)
				if (mat != null && mat.mainTexture != null)
					texList.Add(mat.mainTexture);
		}

		// prefab
		NcAttachPrefab[]	prefabs = tarObj.GetComponentsInChildren<NcAttachPrefab>(true);
		foreach (NcAttachPrefab obj in prefabs)
			if (obj.m_AttachPrefab != null)
			{
				Texture[] ret = PreloadPrefab(obj.m_AttachPrefab, parentPrefabList, true);
				if (ret == null)
					obj.m_AttachPrefab = null;	// clear
				else texList.AddRange(ret);
			}
		NcParticleSystem[]	pss = tarObj.GetComponentsInChildren<NcParticleSystem>(true);
		foreach (NcParticleSystem ps in pss)
			if (ps.m_AttachPrefab != null)
			{
				Texture[] ret = PreloadPrefab(ps.m_AttachPrefab, parentPrefabList, true);
				if (ret == null)
					ps.m_AttachPrefab = null;	// clear
				else texList.AddRange(ret);
			}
		NcSpriteTexture[]	sts = tarObj.GetComponentsInChildren<NcSpriteTexture>(true);
		foreach (NcSpriteTexture st in sts)
			if (st.m_NcSpriteFactoryPrefab != null)
			{
				Texture[] ret = PreloadPrefab(st.m_NcSpriteFactoryPrefab, parentPrefabList, false);
				if (ret != null)
					texList.AddRange(ret);
			}

		// sound
		NcAttachSound[]	ass = tarObj.GetComponentsInChildren<NcAttachSound>(true);
		foreach (NcAttachSound ncas in ass)
			if (ncas.m_AudioClip != null)
				continue;

		// prefab & sound
		NcSpriteFactory[]	sprites = tarObj.GetComponentsInChildren<NcSpriteFactory>(true);
		foreach (NcSpriteFactory sp in sprites)
			if (sp.m_SpriteList != null)
				for (int n = 0; n < sp.m_SpriteList.Count; n++)
					if (sp.m_SpriteList[n].m_EffectPrefab != null)
					{
						Texture[] ret = PreloadPrefab(sp.m_SpriteList[n].m_EffectPrefab, parentPrefabList, true);
						if (ret == null)
							sp.m_SpriteList[n].m_EffectPrefab = null;	// clear
						else texList.AddRange(ret);
						if (sp.m_SpriteList[n].m_AudioClip != null)
							continue;
					}

		return texList.ToArray();
	}

	static Texture[] PreloadPrefab(GameObject tarObj, List<GameObject> parentPrefabList, bool bCheckDup)
	{
		if (parentPrefabList.Contains(tarObj))
		{
			if (bCheckDup)
			{
				string str = "";
				for (int n = 0; n < parentPrefabList.Count; n++)
					str += parentPrefabList[n].name + "/";
				Debug.LogWarning("LoadError : Recursive Prefab - " + str + tarObj.name);
				return null;	// error
			} else return null;
		}
		parentPrefabList.Add(tarObj);
		Texture[] ret = PreloadTexture(tarObj, parentPrefabList);
		parentPrefabList.Remove(tarObj);
		return ret;
	}

	// ----------------------------------------------------------------------------------------------
#if UNITY_EDITOR
	public static void AdjustSpeedEditor(GameObject target, float fSpeedRate)	// support shuriken, legancy, mesh
	{
		NcEffectBehaviour[] coms = target.GetComponentsInChildren<NcEffectBehaviour>(true);
		foreach (NcEffectBehaviour com in coms)
			com.OnUpdateEffectSpeed(fSpeedRate, false);
	}
#endif
	public static void AdjustSpeedRuntime(GameObject target, float fSpeedRate)	// support legancy/mesh , not support shuriken
	{
		NcEffectBehaviour[] coms = target.GetComponentsInChildren<NcEffectBehaviour>(true);
		foreach (NcEffectBehaviour com in coms)
			com.OnUpdateEffectSpeed(fSpeedRate, true);
	}

	public static string GetMaterialColorName(Material mat)
	{
		string[] propertyNames = { "_Color", "_TintColor", "_EmisColor" };

		if (mat != null)
			foreach (string name in propertyNames)
				if (mat.HasProperty(name))
					return name;
		return null;
	}

	protected void DisableEmit()
	{
		NcParticleSystem[] ncpss = gameObject.GetComponentsInChildren<NcParticleSystem>(true);
		foreach (NcParticleSystem ncps in ncpss)
			if (ncps != null)
				ncps.SetDisableEmit();
		NcAttachPrefab[] ncaps = gameObject.GetComponentsInChildren<NcAttachPrefab>(true);
		foreach (NcAttachPrefab ncap in ncaps)
			if (ncap != null)
				ncap.enabled = false;
		ParticleSystem[] pss = gameObject.GetComponentsInChildren<ParticleSystem>(true);
        foreach (ParticleSystem em in pss)
            if (em != null)
            {
                //em.enableEmission = false;
            }

        ParticleEmitter[] pes = gameObject.GetComponentsInChildren<ParticleEmitter>(true);
        foreach (ParticleEmitter em in pes)
            if (em != null)
                em.emit = false;
    }

	// SafeCreate -----------------------------------------------------------------------------------
	public static bool IsSafe()
	{
		return (!m_bShuttingDown);
	}

	protected GameObject CreateEditorGameObject(GameObject srcGameObj)
	{
#if UNITY_EDITOR
		GameObject	fxmMain = GetFXMakerGameObject();
		if (fxmMain != null && (this is NcDuplicator) == false)
		{
 			_RuntimeIntance	arg = new _RuntimeIntance(gameObject, srcGameObj);
			if (srcGameObj.transform.parent == null)
				ChangeParent(GetRootInstanceEffect().transform, srcGameObj.transform, true, null);
 			fxmMain.SendMessage("OnRuntimeIntance", arg, SendMessageOptions.DontRequireReceiver);
		}
#endif
		return srcGameObj;
	}

	public GameObject CreateGameObject(string name)
	{
		if (IsSafe() == false)
			return null;
		return CreateEditorGameObject(new GameObject(name));
	}

	public GameObject CreateGameObject(GameObject original)
	{
		if (IsSafe() == false)
			return null;
		return CreateEditorGameObject((GameObject)Object.Instantiate(original));
	}

	public GameObject CreateGameObject(GameObject original, Vector3 position, Quaternion rotation)
	{
		if (IsSafe() == false)
			return null;
		return CreateEditorGameObject((GameObject)Object.Instantiate(original, position, rotation));
	}

	public GameObject CreateGameObject(GameObject parentObj, GameObject prefabObj)
	{
		if (IsSafe() == false)
			return null;
		GameObject newChild = CreateGameObject(prefabObj);
		if (parentObj != null)
			ChangeParent(parentObj.transform, newChild.transform, true, null);
		return newChild;
	}

	public GameObject CreateGameObject(GameObject parentObj, Transform parentTrans, GameObject prefabObj)
	{
		if (IsSafe() == false)
			return null;
		GameObject newChild = CreateGameObject(prefabObj);
		if (parentObj != null)
			ChangeParent(parentObj.transform, newChild.transform, true, parentTrans);
		return newChild;
	}

	protected void ChangeParent(Transform newParent, Transform child, bool bKeepingLocalTransform, Transform addTransform)
	{
		// Keeping transform
		NcTransformTool	trans = null;
		if (bKeepingLocalTransform)
		{
			trans	= new NcTransformTool(child.transform);
			if (addTransform != null)
				trans.AddTransform(addTransform);
		}
		child.parent = newParent;
		if (bKeepingLocalTransform)
			trans.CopyToLocalTransform(child.transform);

		// Update component
		if (bKeepingLocalTransform)
		{
// 			NcEffectBehaviour[]	coms = gameObject.GetComponentsInChildren<NcEffectBehaviour>();
// 			foreach (NcEffectBehaviour com in coms)
// 				com.SendMessage("Update", SendMessageOptions.DontRequireReceiver);
			NcBillboard[]	ncBills = gameObject.GetComponentsInChildren<NcBillboard>();
	 		foreach (NcBillboard ncb in ncBills)
				ncb.UpdateBillboard();
		}
	}

	// ----------------------------------------------------------------------------
	protected void UpdateMeshColors(Color color)
	{
		if (m_MeshFilter == null)
			m_MeshFilter = (MeshFilter)gameObject.GetComponent(typeof(MeshFilter));
		if (m_MeshFilter == null || m_MeshFilter.sharedMesh == null || m_MeshFilter.mesh == null)
			return;

		Color[]	colors = new Color[m_MeshFilter.mesh.vertexCount];
		for (int n = 0; n < colors.Length; n++)
			colors[n] = color;
		m_MeshFilter.mesh.colors = colors;
	}

	// Loop Function --------------------------------------------------------------------
	// Control Function -----------------------------------------------------------------
	// Event Function -------------------------------------------------------------------
	public void OnApplicationQuit()
	{
		m_bShuttingDown = true;
	}

	public virtual void OnUpdateEffectSpeed(float fSpeedRate, bool bRuntime)
	{
	}

	public virtual void OnUpdateToolData()
	{
	}
}
