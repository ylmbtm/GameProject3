// ----------------------------------------------------------------------------------
//
// FXMaker
// Created by ismoon - 2012 - ismoonto@gmail.com
//
// ----------------------------------------------------------------------------------

using UnityEngine;
using System.Collections;

// [AddComponentMenu("FXMaker/NcAutoDestruct	%#F")]

public class NcAutoDestruct : NcEffectBehaviour
{
	// Attribute ------------------------------------------------------------------------
	public		float				m_fLifeTime				= 2;
	public		float				m_fSmoothDestroyTime	= 0;
	public		bool				m_bDisableEmit			= true;
	public		bool				m_bSmoothHide			= true;
 	protected	bool				m_bEndNcCurveAnimation	= false;

	public		enum CollisionType	{NONE, COLLISION, WORLD_Y};
	public		CollisionType		m_CollisionType			= CollisionType.NONE;
	public		LayerMask			m_CollisionLayer		= -1;
	public		float				m_fCollisionRadius		= 0.3f;
	public		float				m_fDestructPosY			= 0.2f;

	// read only
	protected	float				m_fStartTime			= 0;
	protected	float				m_fStartDestroyTime;
	protected	NcCurveAnimation	m_NcCurveAnimation;

	// Create ---------------------------------------------------------------------------
	public static NcAutoDestruct CreateAutoDestruct(GameObject baseGameObject, float fLifeTime, float fDestroyTime, bool bSmoothHide)
	{
		NcAutoDestruct ncAutoDestruct = baseGameObject.AddComponent<NcAutoDestruct>();
		ncAutoDestruct.m_fLifeTime			= fLifeTime;
		ncAutoDestruct.m_fSmoothDestroyTime	= fDestroyTime;
		ncAutoDestruct.m_bSmoothHide		= bSmoothHide;
		if (IsActive(baseGameObject))
		{
			ncAutoDestruct.Start();
			ncAutoDestruct.Update();
		}
		return ncAutoDestruct;
	}

	// Property -------------------------------------------------------------------------
#if UNITY_EDITOR
	public override string CheckProperty()
	{
		if (1 < gameObject.GetComponents(GetType()).Length)
			return "SCRIPT_WARRING_DUPLICATE";

		return "";	// no error
	}
#endif

	// Loop Function --------------------------------------------------------------------
	void Awake()
	{
 		m_bEndNcCurveAnimation	= false;	// disable

		m_fStartTime			= 0;
		m_NcCurveAnimation		= null;
	}

	void Start()
	{
		m_fStartTime = GetEngineTime();
		if (m_bEndNcCurveAnimation)
			m_NcCurveAnimation = GetComponent<NcCurveAnimation>();
	}

	void Update()
	{
		if (0 < m_fStartDestroyTime)
		{
			if (0 < m_fSmoothDestroyTime)
			{
				if (m_bSmoothHide)
				{
					float fRate = 1 - ((GetEngineTime() - m_fStartDestroyTime) / m_fSmoothDestroyTime);
					if (fRate < 0)
						fRate = 0;

					// Recursively
					Renderer[] rens = transform.GetComponentsInChildren<Renderer>(true);
					for (int n = 0; n < rens.Length; n++)
					{
						Renderer	ren		= rens[n];
						string		colName	= GetMaterialColorName(ren.material);

						if (colName != null)
						{
							Color col = ren.material.GetColor(colName);
							col.a = Mathf.Min(fRate, col.a);
							ren.material.SetColor(colName, col);
						}
					}
				}
				if (m_fStartDestroyTime + m_fSmoothDestroyTime < GetEngineTime())
					AutoDestruct();
			}
		} else {
			// Time
// 			if (0 < m_fStartTime && m_fLifeTime != 0)
			if (0 < m_fStartTime)
			{
				if (m_fStartTime + m_fLifeTime <= GetEngineTime())
					StartDestroy();
			}

			// event
			if (m_bEndNcCurveAnimation && m_NcCurveAnimation != null)
				if (1 <= m_NcCurveAnimation.GetElapsedRate())
					StartDestroy();
		}
	}

	void FixedUpdate()
	{
		if (0 < m_fStartDestroyTime)
			return;
		bool bDestect = false;

		if (m_CollisionType == CollisionType.NONE)
			return;

		if (m_CollisionType == CollisionType.COLLISION)
		{
#if UNITY_EDITOR
			Collider[]	colls = Physics.OverlapSphere(transform.position, m_fCollisionRadius, m_CollisionLayer);
			foreach (Collider coll in colls)
			{
				if (coll.gameObject.GetComponent("FxmInfoIndexing") != null)
					continue;
				bDestect = true;
				break;
			}
#else
			if (Physics.CheckSphere(transform.position, m_fCollisionRadius, m_CollisionLayer))
				bDestect = true;
#endif
		} else
		if (m_CollisionType == CollisionType.WORLD_Y && transform.position.y <= m_fDestructPosY)
			bDestect = true;

		if (bDestect)
			StartDestroy();
	}

	// Control Function -----------------------------------------------------------------
	void StartDestroy()
	{
		if (m_fSmoothDestroyTime <= 0)
			AutoDestruct();
		else {
			m_fStartDestroyTime	= GetEngineTime();
			if (m_bDisableEmit)
				DisableEmit();
		}
	}

	// Event Function -------------------------------------------------------------------
	public override void OnUpdateEffectSpeed(float fSpeedRate, bool bRuntime)
	{
		m_fLifeTime				/= fSpeedRate;
		m_fSmoothDestroyTime	/= fSpeedRate;
	}

	void AutoDestruct()
	{
		DestroyObject(gameObject);
	}
}
