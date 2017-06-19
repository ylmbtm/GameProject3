// ----------------------------------------------------------------------------------
//
// FXMaker
// Created by ismoon - 2012 - ismoonto@gmail.com
//
// ----------------------------------------------------------------------------------


using UnityEngine;
using System.Collections;

public class NcBillboard : NcEffectBehaviour
{
	// Attribute ------------------------------------------------------------------------
	public		enum ROTATION	{NONE, RND}
	public		ROTATION		m_RatationMode;
	public		enum AXIS		{X=0, Y, Z};
	public		AXIS			m_RatationAxis		= AXIS.Z;
	protected	float			m_fRndValue;
	protected	float			m_fTotalRotationValue;
	protected	Quaternion		m_qOiginal;

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
	{	m_qOiginal	= transform.rotation;
	}

	void OnEnable()
	{

#if UNITY_EDITOR
		if (IsCreatingEditObject() == false)
			UpdateBillboard();
#else
 		UpdateBillboard();
#endif
	}

	public void UpdateBillboard()
	{
		m_fRndValue	= Random.Range(0,360.0f);
		if (enabled)
			Look();
	}

	void Start()
	{

	}

	void Look()
	{
		if (Camera.main == null)
			return;
		Vector3		vecUp;
       vecUp = Camera.main.transform.rotation * Vector3.up;
      transform.LookAt(transform.position + Camera.main.transform.rotation * Vector3.back, vecUp);
		if (m_RatationMode == ROTATION.RND)
	  transform.localRotation	*= Quaternion.Euler((m_RatationAxis == AXIS.X ? m_fRndValue : 0), (m_RatationAxis == AXIS.Y ? m_fRndValue : 0), (m_RatationAxis == AXIS.Z ? m_fRndValue : 0));


//		transform.LookAt(transform.position + Camera.main.transform.rotation * Vector3.back,
//			Camera.main.transform.rotation * Vector3.up);
	}
	// Control Function -----------------------------------------------------------------
	// Event Function -------------------------------------------------------------------
}


