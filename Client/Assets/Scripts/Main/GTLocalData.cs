using UnityEngine;
using System.Collections;

public class GTLocalData
{
    private string m_Username      = string.Empty;
    private string m_Password      = string.Empty;
    private bool   m_MusicActive   = true;
    private bool   m_SoundActive   = true;
    private int    m_MaxInstance   = 1001;
    private ulong  m_GUID          = 10000001;

    public string Username
    {
        get { return m_Username; }
        set { m_Username = value; Save(); }
    }

    public string Password
    {
        get { return m_Password; }
        set { m_Password = value; Save(); }
    }

    public bool   MusicActive
    {
        get { return m_MusicActive; }
        set { m_MusicActive = value; Save(); }
    }

    public bool   SoundActive
    {
        get { return m_SoundActive; }
        set { m_SoundActive = value; Save(); }
    }

    public int    MaxInstance
    {
        get { return m_MaxInstance; }
        set { m_MaxInstance = value; Save(); }
    }

    public ulong  GUID
    {
        get { return m_GUID; }
        set { m_GUID = value; Save(); }
    }

    public void   Save()
    {
        string json = LitJson.JsonMapper.ToJson(this);
        string key = System.IO.Directory.GetCurrentDirectory();
        PlayerPrefs.SetString(key, json);
    }
}
