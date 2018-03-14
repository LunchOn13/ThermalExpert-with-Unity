using UnityEngine;
using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections.Generic;
using UnityEngine.UI;
using System.Runtime.InteropServices;

public class Server : MonoBehaviour
{

    static bool isconnected = false;
    enum Commands : int
    {
        String = 0,
        Image
    }

    #region ReceiveBuffer define
    struct ReceiveBuffer
    {
        public const int BUFFER_SIZE = 4096;
        public byte[] Buffer;
        public int ToReceive;
        public MemoryStream BufStream;

        public ReceiveBuffer(int toRec)
        {
            Buffer = new byte[BUFFER_SIZE];
            ToReceive = toRec;
            BufStream = new MemoryStream(toRec);
        }

        public void Dispose()
        {
            Buffer = null;
            ToReceive = 0;
            Close();
            if (BufStream != null)
                BufStream.Dispose();
        }

        public void Close()
        {
            if (BufStream != null && BufStream.CanWrite)
                BufStream.Close();
        }
    }
    #endregion

    #region Client define
    class Client
    {
        byte[] lenBuffer;
        ReceiveBuffer buffer;
        Socket socket;

        public IPEndPoint EndPoint
        {
            get
            {
                if (socket != null && socket.Connected)
                    return (IPEndPoint)socket.RemoteEndPoint;

                return new IPEndPoint(IPAddress.None, 0);
            }
        }
        public delegate void OnSendEventHandler(Client sender, int sent);
        public event OnSendEventHandler OnSend;
        public delegate void DisconnectedEventHandler(Client sender);
        public event DisconnectedEventHandler Disconnected;
        public delegate void DataReceivedEventHandler(Client sender, ReceiveBuffer e);
        public event DataReceivedEventHandler DataReceived;

        public Client(Socket s)
        {
            socket = s;
            lenBuffer = new byte[4];
        }

        public void Close()
        {
            if (socket != null)
            {
                socket.Disconnect(false);
                socket.Close();
            }

            buffer.Dispose();
            socket = null;
            lenBuffer = null;
            Disconnected = null;
            DataReceived = null;
        }

        public void ReceiveAsync()
        {
            // Debug.Log("receiveAsync");

            if (isconnected == false)
            {
                // 소켓 연결할때
                socket.BeginReceive(lenBuffer, 0, lenBuffer.Length, SocketFlags.None, ReceiveCallback, null);
            }
            else
            {
                // 소켓 연결한 이후 이미지 데이터 받아올때
                socket.BeginReceive(buffer.Buffer, 0, buffer.Buffer.Length, SocketFlags.None, receivePacketCallBack, null);
            }
        }

        
        int sendBufferSize = 9;
        public void SendAsync_position()    // 마우스를 클릭한곳의 좌표를 socket 으로 보냄
        {
            // _gain 전송
            // _offset 전송
            if (isClicked)
            {
                isClicked = false;
                if (pos_x < 384 && pos_x > 0 && pos_y < 288 && pos_y > 0)
                {

                    //Debug.Log((int)x + " " + (int)y);
                    byte[] y_position = BitConverter.GetBytes((uint)pos_y);
                    byte[] x_position = BitConverter.GetBytes((uint)pos_x);

                    byte[] position = new byte[sendBufferSize];

                    position[0] = 0;
                    System.Buffer.BlockCopy(x_position, 0, position, 1, x_position.Length);
                    System.Buffer.BlockCopy(y_position, 0, position, 1 + x_position.Length, y_position.Length);

                    // Debug.Log("X ::::: " + pos_x + " Y  :::: " + pos_y);
                    //string s = "";
                    //for(int i = 0; i < position.Length; i++)
                    //{
                    //   s+= string.Format("{0:X} ", position[i]);
                    //}
                    //Debug.Log(s);

                    socket.BeginSend(position, 0, position.Length, SocketFlags.None, SendCallback, null);
                    //SendAsync_position();
                }
            }
        }

        public void SendAsync_operand()
        {
            //if (socket != null)
            //{
            //    // Debug.Log("Send operand");
            //    //Debug.Log("gain " + gain + " offset " + offset);
            //    byte[] _gain = BitConverter.GetBytes(gain);
            //    byte[] _offset = BitConverter.GetBytes(offset);

            //    byte[] operand = new byte[sendBufferSize];

            //    operand[0] = 1;
            //    System.Buffer.BlockCopy(_gain, 0, operand, 1, _gain.Length);
            //    System.Buffer.BlockCopy(_offset, 0, operand, _gain.Length + 1, _offset.Length);

            //    socket.BeginSend(operand, 0, operand.Length, SocketFlags.None, SendCallback, null);
            //}
        }


        public void SendAsync_Calibrate()
        {
            if (socket != null)
            {
                byte[] CalibrateOn = BitConverter.GetBytes(OnCalibrate);

                byte[] send = new byte[sendBufferSize];
                send[0] = 2;
                System.Buffer.BlockCopy(CalibrateOn, 0, send, 1, CalibrateOn.Length);

                socket.BeginSend(send, 0, send.Length, SocketFlags.None, SendCallback, null);
            }
        }

        //public void SendAsync_AIE()
        //{
        //    if(socket != null)
        //    {
        //        byte[] _isAIE = BitConverter.GetBytes(isAIE);
        //        byte[] _AIEval = BitConverter.GetBytes(AIEval);

        //        byte[] operand = new byte[1 + _isAIE.Length + _AIEval.Length];

        //        operand[0] = 2;
        //        System.Buffer.BlockCopy(_isAIE, 0, operand, 1, _isAIE.Length
        //        System.Buffer.BlockCopy(_AIEval, 0, operand, 1+_isAIE.Length, _AIEval.Length);

        //        socket.BeginSend(operand, 0, operand.Length, SocketFlags.None, SendCallback, null);
        //    }
        //}


        public void SendCallback(IAsyncResult ar)
        {
            try
            {
                int sent = socket.EndSend(ar);

                if (OnSend != null)
                {
                    OnSend(this, sent);
                }
            }
            catch (Exception e)
            {
                Debug.Log("Send error " + e.Message);
            }
        }

        public void ReceiveCallback(IAsyncResult ar)
        {
            // Debug.Log("ReceiveCallback");

            BitConverter.ToInt32(lenBuffer, 0);

            try
            {
                // 넘겨 받은 바이트의 갯수
                int rec = socket.EndReceive(ar);

                if (rec == 0)
                {
                    if (Disconnected != null)
                    {
                        Disconnected(this);
                        return;
                    }
                }

                if (rec != 4)
                {
                    throw new Exception();
                }
            }
            catch (SocketException se)
            {
                switch (se.SocketErrorCode)
                {
                    case SocketError.ConnectionAborted:
                    case SocketError.ConnectionReset:
                        if (Disconnected != null)
                        {
                            Disconnected(this);
                            return;
                        }
                        break;
                }
            }
            catch (ObjectDisposedException)
            {
                return;
            }
            catch (NullReferenceException)
            {
                return;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }

            buffer = new ReceiveBuffer(BitConverter.ToInt32(lenBuffer, 0));
            socket.BeginReceive(buffer.Buffer, 0, buffer.Buffer.Length, SocketFlags.None, receivePacketCallBack, null);
        }

        public void receivePacketCallBack(IAsyncResult ar)
        {
            //Debug.Log("RecievePacketCallback");
            int rec = socket.EndReceive(ar);

            if (rec <= 0)
            {
                return;
            }

            buffer.BufStream.Write(buffer.Buffer, 0, rec);

            buffer.ToReceive -= rec;

            if (buffer.ToReceive > 0)
            {
                Array.Clear(buffer.Buffer, 0, buffer.Buffer.Length);

                socket.BeginReceive(buffer.Buffer, 0, buffer.Buffer.Length, SocketFlags.None, receivePacketCallBack, null);
                return;
            }

            if (DataReceived != null)
            {
                buffer.BufStream.Position = 0;
                DataReceived(this, buffer);
            }

            buffer.Dispose();

            isconnected = false;
            ReceiveAsync();
        }
    }
    #endregion

    class Listener
    {
        public delegate void SocketAcceptedHandler(Socket e);
        public event SocketAcceptedHandler Accepted;

        Socket listener;
        public int Port;

        public bool Running
        {
            get;
            private set;
        }

        public Listener() { Port = 0; }
        public void Start(int port)
        {
            if (Running)
                return;

            listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            listener.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            listener.Bind(new IPEndPoint(IPAddress.Any, port));
            listener.Listen(0);

            listener.BeginAccept(AcceptedCallback, null);
            Running = true;
        }

        public void Stop()
        {
            if (!Running)
                return;

            listener.Close();
            Running = false;
        }

        void AcceptedCallback(IAsyncResult ar)
        {
            try
            {
                Socket s = listener.EndAccept(ar);

                if (Accepted != null)
                {
                    Accepted(s);
                }
            }
            catch { }

            if (Running)
            {
                try
                {
                    listener.BeginAccept(AcceptedCallback, null);
                }
                catch { }
            }
        }
    }
    // Use this for initialization


    static List<Client> ClientList;
    Listener listener;
    float[] idata = new float[384 * 288];

    Client c = null;
    private void Awake()
    {
        ClientList = new List<Client>();

        listener = new Listener();
        listener.Accepted += new Listener.SocketAcceptedHandler(Listener_Accepted);

        listener.Start(8192);
        Debug.Log("Server Started");

    }

    void Listener_Accepted(Socket e)
    {
        Client client = new Client(e);
        ClientList.Add(client);

        client.OnSend += new Client.OnSendEventHandler(client_OnSend);
        client.DataReceived += new Client.DataReceivedEventHandler(Client_DataReceived);
        client.Disconnected += new Client.DisconnectedEventHandler(Client_Disconnected);
        client.ReceiveAsync();
        c = client;
        Debug.Log("Conneted: " + client.EndPoint.ToString());
        isconnected = true;

    }

    void client_OnSend(Client sender, int sent)
    {
        // Debug.Log("Sent " + sent);

    }

    void Client_Disconnected(Client sender)
    {
        sender.Close();
        sender = null;

        //Invoke((MethodInvoker)delegate
        //{
        //    toolStripStatusLabel1.Text = "Connected: ...";
        //    DialogResult res = MessageBox.Show("Client Disconnected\nClear Data?", "서버 메시지", MessageBoxButtons.YesNo);
        //    if (res == System.Windows.Forms.DialogResult.Yes)
        //    {
        //        lstText.Items.Clear();
        //        pbImage.Image = null;
        //    }
        //});
        Debug.Log("Disconnected.");
        isconnected = false;
    }

    void Client_DataReceived(Client sender, ReceiveBuffer e)
    {
        // 받아온 데이터 포멧
        // 4byte 마우스를 클릭했을 때의 해당 좌표 온도 - 클릭하지 않았을때 0도
        // 8byte 영상에서의 최대 온도 (4) 최저 온도 (4)
        // 나머지 이미지 데이터
        BinaryReader r = new BinaryReader(e.BufStream);
        tempurture = r.ReadSingle();

        // 최대 온도
        max_T = r.ReadSingle();
        // 최저 온도
        min_T = r.ReadSingle();

        //Debug.Log("Max T " + maxT + " MIN T " + minT);


        float[] iBytes = new float[384 * 288];

        for (int i = 0; i < 384 * 288; i++)
        {
            iBytes[i] = r.ReadSingle();
        }
        idata = iBytes;
        falg = true;
    }

    private void OnDestroy()
    {
        foreach (Client client in ClientList)
        {
            if (client != null)
            {
                client.Close();
            }
        }
        ClientList.Clear();

        if (listener != null && listener.Running)
            listener.Stop();
    }


    public Image tmp;
    public static float gain = 1f / 256f;
    public static float offset = 0f;

    public static bool OnCalibrate = false;
    //public static bool isAIE;
    //public static float AIEval;
    //    public RawImage cameraImage;
    private Vector2 imageSize;
    private int width;
    private int height;
    public static int pos_x = 0;
    public static int pos_y = 0;
    public static bool isClicked = false;

    public Text txt_path;
    string path;
    void Start()
    {

        imageSize = tmp.GetComponent<RectTransform>().sizeDelta;

        //Debug.Log(tmp.transform.position);

        width = (int)imageSize.x;
        height = (int)imageSize.y;

        Debug.Log(width + " " + height);

        // 실행시킬 경로로 바꿔야함
        // mfc 실행시키기
        path = Application.dataPath;
        path = path + "/../Debug/MFCi3system.exe";
        path.Replace('/', '\\');
        Debug.Log(path);
       
        System.IO.FileInfo directoryInfo = new System.IO.FileInfo(path);
        if (directoryInfo.Exists)
            System.Diagnostics.Process.Start(path);
        else
            txt_path.text = "nope";
    }
     
    public Slider max_tSlider;
    public Slider min_tSlider;
    public Toggle fixedTempToggle;
    public Button CalibratingB;
    //public Toggle AIEToggle;
    //public Slider AIESlider;


    public void ChangeMax_t()
    {
        setMax_t = max_tSlider.value;
        // Debug.Log("gain change to " + gainSlider.value + " gain is now " + gain);
    }


    public void ChangeMin_t()
    {
        setMin_t = min_tSlider.value;
        // Debug.Log("gain change to " + offsetSlider.value + " gain is now " + offset);
    }


    //public void ToggleAIE()
    //{
    //    isAIE = AIEToggle.isOn;
    //    if (isAIE)
    //        AIESlider.enabled = true;
    //    else
    //        AIESlider.enabled = false;
    //}

    //public void ChangeAIEval()
    //{
    //    AIEval = AIESlider.value;
    //}

    public void ToggleFixedTemp()
    {
        isFixedTemp = fixedTempToggle.isOn;
        ChangeMax_t();
        ChangeMin_t();

    }

    public void Calibrating()
    {
        CalibratingB.gameObject.SetActive(false);
        c.SendAsync_Calibrate();
        CalibratingB.gameObject.SetActive(true);
    }

    public float max_T;
    public float min_T;
    public float tempurture;
    bool isFixedTemp = false;
    public float setMax_t;
    public float setMin_t;

    bool falg = false;
    // Update is called once per frame
    void Update()
    {
        // 마우스 좌표 받아오기
        // 적외선 영상이 90도 돌아가 있기 때문에 좀 복잡함
        if (Input.GetMouseButton(0) && c != null)
        {
            isClicked = true;
            pos_x = (int)((Screen.height - Input.mousePosition.y) - ((Screen.height / 2) - (tmp.transform.localPosition.y + (width / 2))));

            pos_y = (int)(Input.mousePosition.x - tmp.GetComponent<RectTransform>().position.x + height / 2);
            if (pos_y >= -2 && pos_y < 0)
            {
                pos_y = 0;
            }

            // Debug.Log(pos_x + " " + pos_y);
            if (c != null)
                c.SendAsync_position();
        }
        
        // 이미지 데이터 표현
        if (idata != null && falg != false)
        {
            Texture2D texture = new Texture2D(384, 288, TextureFormat.RGB24, false);
            //Debug.Log(ibyte.Length);
            // Debug.Log(texture.GetRawTextureData().Length);
            Color[] temp = new Color[384 * 288];
            float interval = max_T - min_T;
            if (!isFixedTemp)
                // 고성된 이미지 색상 표현
                for (int i = 0; i < 384 * 288; i++)
                {
                    temp[i] = (Gradi.g1.Evaluate((idata[i] - min_T) / interval));
                }
            else
                // 사용자가 설정 가능한 이미지 색상 표현
                for (int i = 0; i < 384 * 288; i++)
                {

                    temp[i] = (Gradi.g1.Evaluate((idata[i] - setMin_t) / (setMax_t - setMin_t)));
                }

            texture.SetPixels(temp);
            texture.Apply();
            tmp.sprite = Sprite.Create(texture, new Rect(0, 0, 384, 288), new Vector2(0, 0));

            falg = false;
        }

        //if (c != null)
        //{
        //    c.SendAsync_operand();
        //}
    }
}
