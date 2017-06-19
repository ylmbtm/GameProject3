// ----------------------------------------------------------------------------------
//
// FXMaker
// Created by ismoon - 2012 - ismoonto@gmail.com
//
// ----------------------------------------------------------------------------------

using UnityEngine;
using System.Collections;

// [AddComponentMenu("FXMaker/NcAttachSound")]

public class NcAttachSound : NcEffectBehaviour
{
	// Attribute ------------------------------------------------------------------------
	public		float		m_fDelayTime;
	public		float		m_fRepeatTime;
	public		int			m_nRepeatCount;
	public		AudioClip	m_AudioClip;
	public		int			m_nPriority				= 128;
	public		bool		m_bLoop					= false;
	public		float		m_fVolume				= 1;
	public		float		m_fPitch				= 1;

	protected	AudioSource	m_AudioSource;
	protected	float		m_fStartTime;
	protected	int			m_nCreateCount			= 0;
	protected	bool		m_bStartAttach			= false;

	// Property -------------------------------------------------------------------------
#if UNITY_EDITOR
	public override string CheckProperty()
	{
		if (m_AudioClip == null)
			return "SCRIPT_EMPTY_AUDIOCLIP";
		return "";	// no error
	}
#endif

 	public override int GetAnimationState()
	{
		if ((enabled && IsActive(gameObject)) || (m_AudioSource != null && m_AudioSource.isPlaying))
			return 1;
// 		if ((enabled && IsActive(gameObject)) && m_AudioClip != null &&
// 			((m_fRepeatTime == 0 && m_nCreateCount < 1) ||
// 			 (m_fRepeatTime != 0 && m_nRepeatCount == 0) ||
// 			 (m_fRepeatTime != 0 && m_nRepeatCount != 0 && m_nCreateCount < m_nRepeatCount) ||
// 			 m_bLoop ||
// 			 (m_AudioSource != null && m_AudioSource.isPlaying)))
// 			return 1;
		return 0;
	}

	public void Replay()
	{
		m_bStartAttach	= false;
	}

	// Loop Function --------------------------------------------------------------------
	void Update()
	{
		if (m_AudioClip == null)
		{
			enabled = false;
			return;
		}

		if (m_bStartAttach == false)
		{
			m_fStartTime	= GetEngineTime();
			m_bStartAttach	= true;
		}

		if (m_fStartTime + m_fDelayTime <= GetEngineTime())
		{
			CreateAttachSound();
			if (0 < m_fRepeatTime && (m_nRepeatCount == 0 || m_nCreateCount < m_nRepeatCount))
			{
				m_fStartTime	= GetEngineTime();
				m_fDelayTime	= m_fRepeatTime;
			} else {
				enabled = false;
			}
		}
	}

	// Control Function -----------------------------------------------------------------
	void CreateAttachSound()
	{
		m_AudioSource = gameObject.AddComponent<AudioSource>();
		m_AudioSource.clip		= m_AudioClip;
		m_AudioSource.priority	= m_nPriority;
		m_AudioSource.loop		= m_bLoop;
		m_AudioSource.volume	= m_fVolume;
		m_AudioSource.pitch		= m_fPitch;
		m_AudioSource.Play();
		m_nCreateCount++;
	}

	// Event Function -------------------------------------------------------------------
	public override void OnUpdateEffectSpeed(float fSpeedRate, bool bRuntime)
	{
		m_fDelayTime		/= fSpeedRate;
		m_fRepeatTime		/= fSpeedRate;
	}
}
