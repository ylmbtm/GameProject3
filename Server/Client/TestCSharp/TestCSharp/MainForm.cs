using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace TestCSharp
{

    public partial class MainForm : Form
    {
        ClientConnector m_ClientConnector = new ClientConnector();

        NetPacketHandler m_PacketHandler = new NetPacketHandler();

        Timer procTimer = new Timer();

        public MainForm()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Debug.Print("NetPacketHandler::OnCommandHandle");

            m_ClientConnector.RegisterMsgHandler(m_PacketHandler);

            procTimer .Tick += new EventHandler(procTimer_Tick);

            procTimer.Interval = 100;//默认100毫秒

            procTimer.Start();

            m_ClientConnector.SetLoginServerAddr("127.0.0.1", 7994);
        }

       public void procTimer_Tick(object sender, EventArgs e)
        {
            m_ClientConnector.Render();
        }

       private void Login_Click(object sender, EventArgs e)
       {
           //m_ClientEngine.m_WriteHelper.BeginWrite((UInt16)Command_ID.CMD_CHAR_NEW_ACCOUNT_REQ, 0);

          // m_ClientEngine.m_WriteHelper.WriteFixString("mmmmm",32);
           //m_ClientEngine.m_WriteHelper.WriteFixString("kkkkkk", 32);

           //m_ClientEngine.m_WriteHelper.EndWrite();

           //m_ClientEngine.SendData(m_ClientEngine.m_WriteHelper.GetData(), m_ClientEngine.m_WriteHelper.GetDataLen());
           m_ClientConnector.Login("test0", "123456", true);
       }
    }

    public class NetPacketHandler : MessageHandler
    {
        public override Boolean OnCommandHandle(Command_ID wCommandID, UInt64 u64ConnID, ReadBufferHelper ReadHelper)
        {
            switch (wCommandID)
            {
                case Command_ID.CMD_CHAR_LOGIN_ACK:
                    {
                        StCharLoginAck Ack = new StCharLoginAck();
                        Ack.Read(ReadHelper);
                    }
                    break;
                case Command_ID.CMD_CHAR_NEW_CHAR_ACK:
                    {

                    }
                    break;
                case Command_ID.CMD_CHAR_DEL_CHAR_ACK:
                    {

                    }
                    break;
                case Command_ID.CMD_CHAR_NEW_ACCOUNT_ACK:
                    {
                        UInt16 nRetCode = ReadHelper.ReadUint16();
                    }
                    break;
                case Command_ID.CMD_CHAR_ENTER_GAME_ACK:
                    {

                    }
                    break;
                case Command_ID.CMD_CHAR_NEARBY_ADD:
                    {

                    }
                    break;
                case Command_ID.CMD_CHAR_NEARBY_UPDATE:
                    {

                    }
                    break;
                case Command_ID.CMD_CHAR_NEARBY_REMOVE:
                    {

                    }
                    break;

                default:
                    {

                    }
                    break;
            }

            return true;
        }
    }
}
