using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GTAudioManager : GTMonoSingleton<GTAudioManager>
{
    public enum AudioChannel
    {
        Motion       = 1,
        Action       = 2,
        Skill        = 4,
        Behit        = 8,
        SkillCombine = 16
    }

    public bool               MusicActive                          { get; private set; }
    public bool               SoundActive                          { get; private set; }
    public AudioSource        MusicAudioSource                     { get; private set; }
    public AudioSource        SoundAudioSource                     { get; private set; }
    public Queue<AudioSource> EffectAudioSourceQueue               { get; private set; }
    private GameObject        musicSource;
    private GameObject        soundSource;

    public override void SetRoot(Transform parent)
    {
        base.SetRoot(parent);
        musicSource = new GameObject("MusicSource");
        soundSource = new GameObject("SoundSource");
        musicSource.transform.parent = transform;
        soundSource.transform.parent = transform;
        MusicAudioSource = musicSource.AddComponent<AudioSource>();
        MusicAudioSource.loop = true;
        MusicAudioSource.playOnAwake = false;
        SoundAudioSource = soundSource.AddComponent<AudioSource>();
        SoundAudioSource.loop = false;
        SoundAudioSource.playOnAwake = false;
        MusicActive = GTGlobal.LocalData.MusicActive;
        SoundActive = GTGlobal.LocalData.SoundActive;
        EffectAudioSourceQueue = new Queue<AudioSource>();
        for (int i = 0; i < 20; i++)
        {
            AudioSource ad = soundSource.AddComponent<AudioSource>();
            EffectAudioSourceQueue.Enqueue(ad);
        }
    }

    public void SetMusicActive(bool active)
    {
        if (MusicActive == active)
        {
            return;
        }
        MusicActive = active;
        GTGlobal.LocalData.MusicActive = active;
        if (MusicAudioSource == null)
        {
            return;
        }
        MusicAudioSource.gameObject.SetActive(active);
    }

    public void SetSoundActive(bool active)
    {
        if (SoundActive == active)
        {
            return;
        }
        if (SoundAudioSource == null)
        {
            return;
        }
        SoundActive = active;
        GTGlobal.LocalData.SoundActive = active;
    }

    public void PlayClipAtPoint(string soundPath, Vector3 pos)
    {
        if (SoundActive == false)
        {
            return;
        }
        AudioClip clip = GTResourceManager.Instance.Load<AudioClip>(soundPath);
        if (clip == null)
        {
            return;
        }
        AudioSource.PlayClipAtPoint(clip, pos);
    }

    public AudioSource PlaySound(string soundPath)
    {
        if (SoundActive == false)
        {
            return null;
        }
        AudioClip clip = GTResourceManager.Instance.Load<AudioClip>(soundPath);
        if (clip == null)
        {
            return null;
        }
        SoundAudioSource.Stop();
        SoundAudioSource.clip = clip;
        SoundAudioSource.PlayOneShot(clip);
        return SoundAudioSource;
    }

    public AudioSource PlayEffectAudio(string soundPath)
    {
        if (SoundActive == false)
        {
            return null;
        }
        AudioClip clip = GTResourceManager.Instance.Load<AudioClip>(soundPath);
        if (clip == null)
        {
            return null;
        }
        AudioSource audio = DequeueEffectAudio();
        audio.clip = clip;
        audio.volume = 1;
        audio.Play();
        audio.loop = false;
        EffectAudioSourceQueue.Enqueue(audio);
        return audio;
    }

    public AudioSource PlayEffectAudio(string soundPath, float volume = 1f, float pitch = 1f, bool loop = false)
    {
        if (SoundActive == false)
        {
            return null;
        }
        AudioClip clip = GTResourceManager.Instance.Load<AudioClip>(soundPath);
        if (clip == null)
        {
            return null;
        }
        AudioSource audio = DequeueEffectAudio();
        audio.clip = clip;
        audio.volume = volume;
        audio.pitch = pitch;
        audio.loop = loop;
        audio.Play();
        if (!loop)
        {
            EffectAudioSourceQueue.Enqueue(audio);
        }
        return audio;
    }

    public AudioSource EnqueueEffectAudio(AudioSource audio)
    {
        if (audio == null)
        {
            return null;
        }
        audio.volume = 0;
        audio.clip = null;
        audio.Stop();
        EffectAudioSourceQueue.Enqueue(audio);
        return audio;
    }

    public AudioSource DequeueEffectAudio()
    {
        if (EffectAudioSourceQueue.Count == 0)
        {
            AudioSource ad = soundSource.AddComponent<AudioSource>();
            return ad;
        }
        else
        {
            return EffectAudioSourceQueue.Dequeue();
        }
    }

    public AudioSource PlayMusic(string soundPath)
    {
        if (MusicActive == false)
        {
            return null;
        }
        AudioClip clip = GTResourceManager.Instance.Load<AudioClip>(soundPath);
        if (clip == null)
        {
            return null;
        }
        if (MusicAudioSource.isPlaying && MusicAudioSource.clip.name == clip.name)
        {
            return null;
        }
        MusicAudioSource.gameObject.SetActive(true);
        MusicAudioSource.clip = clip;
        MusicAudioSource.loop = true;
        MusicAudioSource.Play();
        return MusicAudioSource;
    }
}
