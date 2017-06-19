using System; 
using System.Net; 
using System.Collections; 
using System.Collections.Generic;
using System.Text;
using System.Threading; 
using System.Runtime.InteropServices;

public class ReadBufferHelper 
{
	public const int BUFFER_MAX = 8096;
    private byte[] m_DataBuffer = new byte[BUFFER_MAX];

    public int m_TotalLen = 0;
	public int m_ReadPos  = 0;

    public TransferHeader m_TransferHeader = new TransferHeader();

    public CommandHeader m_CommandHeader = new CommandHeader();
	
	private static System.Text.ASCIIEncoding _converter = new System.Text.ASCIIEncoding();
	
	public byte[] GetData()
	{
        return m_DataBuffer;
	}
	
	public void SetData(byte[] data)
	{
        m_DataBuffer = data;
	}
	
	//---------------------------------------------------------------------------------
    public Boolean BeginRead()
    {
        m_ReadPos = 0;

        m_TransferHeader.Read(this);

        m_CommandHeader.Read(this);

        return true;
    }

    public Boolean EndRead()
    {
        return true;
    }

	public SByte ReadInt8()
	{
        return (SByte)m_DataBuffer[m_ReadPos++];
	}

	public Int16 ReadInt16()
	{
        m_ReadPos += 2;
        return BitConverter.ToInt16(m_DataBuffer, m_ReadPos - 2);
	}
		
	public Int32 ReadInt32()
	{
        m_ReadPos += 4;
        return BitConverter.ToInt32(m_DataBuffer, m_ReadPos - 4);
	}

	public Int64 ReadInt64()
	{
        m_ReadPos += 8;
        return BitConverter.ToInt64(m_DataBuffer, m_ReadPos - 8);
	}
	
	public Byte ReadUint8()
	{
        return m_DataBuffer[m_ReadPos++];
	}

	public UInt16 ReadUint16()
	{
        m_ReadPos += 2;
        return BitConverter.ToUInt16(m_DataBuffer, m_ReadPos - 2);
	}

	public UInt32 ReadUint32()
	{
        m_ReadPos += 4;
        return BitConverter.ToUInt32(m_DataBuffer, m_ReadPos - 4);
	}
	
	public UInt64 ReadUint64()
	{
        m_ReadPos += 8;
        return BitConverter.ToUInt64(m_DataBuffer, m_ReadPos - 8);
	}
	
	public float ReadFloat()
	{
        m_ReadPos += 4;
        return BitConverter.ToSingle(m_DataBuffer, m_ReadPos - 4);
	}

	public double ReadDouble()
	{
        m_ReadPos += 8;
        return BitConverter.ToDouble(m_DataBuffer, m_ReadPos - 8);
	}
	
	public string ReadString()
	{
        int offset = m_ReadPos;
        while (m_DataBuffer[m_ReadPos++] != 0)
		{
		}

        return _converter.GetString(m_DataBuffer, offset, m_ReadPos - offset - 1);
	}

    public string ReadFixString(int FixSize)
    {
        int offset = m_ReadPos;

        m_ReadPos += FixSize;

        return _converter.GetString(m_DataBuffer, offset, m_ReadPos - offset);
    }

	public byte[] ReadBlob()
	{
		UInt32 size = ReadUint32();
		byte[] buf = new byte[size];

        Array.Copy(m_DataBuffer, m_ReadPos, buf, 0, size);

        m_ReadPos += (int)size;

		return buf;
	}

    public void ReadSkip(UInt32 v)
    {
        m_ReadPos += (int)v;
    }

    //---------------------------------------------------------------------------------
    public bool ReadEOF()
    {
        return (m_TotalLen - m_ReadPos) <= 0;
    }

    public int GetLeftSize()
    {
        return m_TotalLen - m_ReadPos;
    }

    public int GetDataLen()
    {
        return m_TotalLen;
    }

	//---------------------------------------------------------------------------------
	public byte[] GetBuffer()
	{
        byte[] buf = new byte[GetLeftSize()];
        Array.Copy(m_DataBuffer, m_ReadPos, buf, 0, GetLeftSize());
		return buf;
	}
	
	//---------------------------------------------------------------------------------
	public string toString()
	{
		string s = "";

		byte[] buf = GetBuffer();

		for(int i=0; i<buf.Length; i++)
		{
			s += buf[i];
			s += " ";
		}
		return s;
	}
}

public class WriteBufferHelper
{
    public const int BUFFER_MAX = 8096;
    private byte[] m_DataBuffer = new byte[BUFFER_MAX];

    public int m_WritePos = 0;

    public UInt64 m_u64ClientID;    //¿Í»§¶ËID

    public TransferHeader m_TransferHeader = new TransferHeader();

    public CommandHeader m_CommandHeader = new CommandHeader();

    private static System.Text.ASCIIEncoding _converter = new System.Text.ASCIIEncoding();

    public byte[] GetData()
    {
        return m_DataBuffer;
    }

    public void SetData(byte[] data)
    {
        m_DataBuffer = data;
    }

    //---------------------------------------------------------------------------------
    public void BeginWrite(UInt16 wCommandID, Byte CmdHandleID)
    {
        m_WritePos = 0;

        m_TransferHeader.CheckCode = 255;
        m_TransferHeader.dwSize = 0;

        m_CommandHeader.CmdHandleID = CmdHandleID;
        m_CommandHeader.dwSceneID = 0;
        m_CommandHeader.u64CharID = m_u64ClientID;
        m_CommandHeader.wCommandID = wCommandID;

        m_TransferHeader.Write(this);

        m_CommandHeader.Write(this);
    }

    public void EndWrite()
    {
        int nLen = m_WritePos;

        m_WritePos = 1;

        WriteInt16((Int16)nLen);

        m_WritePos = nLen;
    }

    public void WriteInt8(SByte v)
    {
        m_DataBuffer[m_WritePos++] = (Byte)v;
    }

    public void WriteInt16(Int16 v)
    {
        WriteInt8((SByte)(v & 0xff));
        WriteInt8((SByte)(v >> 8 & 0xff));
    }

    public void WriteInt32(Int32 v)
    {
        for (int i = 0; i < 4; i++)
            WriteInt8((SByte)(v >> i * 8 & 0xff));
    }

    public void WriteInt64(Int64 v)
    {
        byte[] getdata = BitConverter.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_DataBuffer[m_WritePos++] = getdata[i];
        }
    }

    public void WriteUint8(Byte v)
    {
        m_DataBuffer[m_WritePos++] = v;
    }

    public void WriteUint16(UInt16 v)
    {
        WriteUint8((Byte)(v & 0xff));
        WriteUint8((Byte)(v >> 8 & 0xff));
    }

    public void WriteUint32(UInt32 v)
    {
        for (int i = 0; i < 4; i++)
            WriteUint8((Byte)(v >> i * 8 & 0xff));
    }

    public void WriteUint64(UInt64 v)
    {
        byte[] getdata = BitConverter.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_DataBuffer[m_WritePos++] = getdata[i];
        }
    }

    public void WriteFloat(float v)
    {
        byte[] getdata = BitConverter.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_DataBuffer[m_WritePos++] = getdata[i];
        }
    }

    public void WriteDouble(double v)
    {
        byte[] getdata = BitConverter.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_DataBuffer[m_WritePos++] = getdata[i];
        }
    }

    public void WriteBlob(byte[] v)
    {
        UInt32 size = (UInt32)v.Length;
        if (size + 4 > FreeSize())
        {
            return;
        }

        WriteUint32(size);

        for (UInt32 i = 0; i < size; i++)
        {
            m_DataBuffer[m_WritePos++] = v[i];
        }
    }

    public void WriteString(string v)
    {
        if (v.Length > FreeSize())
        {
            return;
        }

        byte[] getdata = System.Text.Encoding.ASCII.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_DataBuffer[m_WritePos++] = getdata[i];
        }

        m_DataBuffer[m_WritePos++] = 0;
    }

    public void WriteFixString(string v, int nFixSize)
    {
        if (v.Length > FreeSize())
        {
            return;
        }

        if (nFixSize > FreeSize())
        {
            return;
        }

        byte[] getdata = System.Text.Encoding.ASCII.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_DataBuffer[m_WritePos++] = getdata[i];
        }

        for (int i = getdata.Length; i < nFixSize; i++)
        {
            m_DataBuffer[m_WritePos++] = 0;
        }
    }
    //---------------------------------------------------------------------------------
    public UInt32 FreeSize()
    {
        return (UInt32)(BUFFER_MAX - m_WritePos);
    }
    //---------------------------------------------------------------------------------
    public int GetDataLen()
    {
        return m_WritePos;
    }

}


