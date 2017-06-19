// ----------------------------------------------------------------------------------
//
// FXMaker
// Created by ismoon - 2012 - ismoonto@gmail.com
//
// ----------------------------------------------------------------------------------

using UnityEngine;
using System.Collections;

public class NcSpriteTexture : NcEffectBehaviour
{
	// Attribute ------------------------------------------------------------------------
	public		GameObject						m_NcSpriteFactoryPrefab	= null;
	protected	NcSpriteFactory					m_NcSpriteFactoryCom	= null;
	public		NcSpriteFactory.NcFrameInfo[]	m_NcSpriteFrameInfos	= null;
	public		float							m_fUvScale				= 1;
	public		int								m_nSpriteFactoryIndex	= 0;
	public		int								m_nFrameIndex			= 0;

	public		NcSpriteFactory.MESH_TYPE		m_MeshType				= NcSpriteFactory.MESH_TYPE.BuiltIn_Plane;
	public		NcSpriteFactory.ALIGN_TYPE		m_AlignType				= NcSpriteFactory.ALIGN_TYPE.CENTER;

	protected	GameObject						m_EffectObject			= null;

	// Property -------------------------------------------------------------------------
#if UNITY_EDITOR
	public override string CheckProperty()
	{
		if (1 < gameObject.GetComponents(GetType()).Length)
			return "SCRIPT_WARRING_DUPLICATE";
		if (m_NcSpriteFactoryPrefab == null || m_NcSpriteFactoryPrefab.GetComponent<NcSpriteFactory>() == null)
			return "SCRIPT_EMPTY_SPRITEFACTORY";
		if (1 < GetEditingUvComponentCount())
			return "SCRIPT_DUPERR_EDITINGUV";
		if (GetComponent<Renderer>() == null || GetComponent<Renderer>().sharedMaterial == null)
			return "SCRIPT_EMPTY_MATERIAL";

		return "";	// no error
	}
#endif

	// Loop Function --------------------------------------------------------------------
	void Awake()
	{
// #if UNITY_EDITOR
// 		if (IsCreatingEditObject() == false)
// #endif
		{
			if (m_NcSpriteFactoryPrefab == null && gameObject.GetComponent<NcSpriteFactory>() != null)
				m_NcSpriteFactoryPrefab = gameObject;
			if (m_NcSpriteFactoryPrefab && m_NcSpriteFactoryPrefab.GetComponent<NcSpriteFactory>() != null)
				m_NcSpriteFactoryCom = m_NcSpriteFactoryPrefab.GetComponent<NcSpriteFactory>();
		}
	}

	void Start()
	{
		UpdateSpriteTexture(true);
	}

// 	void Update()
// 	{
// 	}

	// Control Function -----------------------------------------------------------------
	public void SetSpriteFactoryIndex(int nSpriteFactoryIndex, int nFrameIndex, bool bRunImmediate)
	{
		if (m_NcSpriteFactoryCom == null)
		{
			if (m_NcSpriteFactoryPrefab && m_NcSpriteFactoryPrefab.GetComponent<NcSpriteFactory>() != null)
				m_NcSpriteFactoryCom = m_NcSpriteFactoryPrefab.GetComponent<NcSpriteFactory>();
			else return;
		}
		m_nSpriteFactoryIndex	= nSpriteFactoryIndex;
		if (m_NcSpriteFactoryCom.IsValidFactory() == false)
			return;
		m_NcSpriteFrameInfos	= m_NcSpriteFactoryCom.GetSpriteNode(nSpriteFactoryIndex).m_FrameInfos;
		m_nFrameIndex			= (0 <= nFrameIndex ? nFrameIndex : m_nFrameIndex);
		m_nFrameIndex			= (m_NcSpriteFrameInfos.Length == 0 || m_NcSpriteFrameInfos.Length <= m_nFrameIndex ? 0 : m_nFrameIndex);
		m_fUvScale				= m_NcSpriteFactoryCom.m_fUvScale;
		if (bRunImmediate)
			UpdateSpriteTexture(bRunImmediate);
	}

	void UpdateSpriteTexture(bool bShowEffect)
	{
		if (UpdateSpriteMaterial() == false)
			return;
		if (m_NcSpriteFactoryCom.IsValidFactory() == false)
			return;
		if (m_NcSpriteFrameInfos.Length == 0)
			SetSpriteFactoryIndex(m_nSpriteFactoryIndex, m_nFrameIndex, false);

		if (m_MeshFilter == null)
		{
			if (gameObject.GetComponent<MeshFilter>() != null)
				 m_MeshFilter = gameObject.GetComponent<MeshFilter>();
			else m_MeshFilter = gameObject.AddComponent<MeshFilter>();
		}
		NcSpriteFactory.CreatePlane(m_MeshFilter, m_fUvScale, m_NcSpriteFrameInfos[m_nFrameIndex], m_AlignType, m_MeshType);
  		NcSpriteFactory.UpdateMeshUVs(m_MeshFilter, m_NcSpriteFrameInfos[m_nFrameIndex].m_TextureUvOffset);
// 		{
// // 			Debug.Log("m_Renderer.material");
// 			renderer.material.mainTextureScale	= new Vector2(m_fTilingX, m_fTilingY);
// 			renderer.material.mainTextureOffset	= new Vector2(m_fOffsetX, m_fOffsetY);
// 		}

		if (bShowEffect)
			m_EffectObject = m_NcSpriteFactoryCom.CreateSpriteEffect(m_nSpriteFactoryIndex, transform);
	}

	public bool UpdateSpriteMaterial()
	{
		if (m_NcSpriteFactoryPrefab == null)
			return false;
		if (m_NcSpriteFactoryPrefab.GetComponent<Renderer>() == null || m_NcSpriteFactoryPrefab.GetComponent<Renderer>().sharedMaterial == null || m_NcSpriteFactoryPrefab.GetComponent<Renderer>().sharedMaterial.mainTexture == null)
			return false;
		if (GetComponent<Renderer>() == null)
			return false;
		
		if (m_NcSpriteFactoryCom == null)
			return false;
		if (m_nSpriteFactoryIndex < 0 || m_NcSpriteFactoryCom.GetSpriteNodeCount() <= m_nSpriteFactoryIndex)
			return false;
		if (m_NcSpriteFactoryCom.m_SpriteType != NcSpriteFactory.SPRITE_TYPE.NcSpriteTexture)
			return false;
		GetComponent<Renderer>().sharedMaterial = m_NcSpriteFactoryPrefab.GetComponent<Renderer>().sharedMaterial;
		return true;
	}

	// Event Function -------------------------------------------------------------------
	public override void OnUpdateEffectSpeed(float fSpeedRate, bool bRuntime)
	{
	}

	public override void OnUpdateToolData()
	{
	}
}

