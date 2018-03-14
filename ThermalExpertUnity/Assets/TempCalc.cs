using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class TempCalc : MonoBehaviour
{

    public Text TempMax;
    public Text Temp1;
    public Text Temp2;
    public Text Temp3;
    public Text Temp4;
    public Text TempMin;

    float tmp;

    public Toggle fixedT;
    
    public Server socketServer;
    // Use this for initialization
    void Start()
    {
        tmp = (socketServer.max_T - socketServer.min_T) / 5;
        TempMax.text = socketServer.max_T.ToString();
        Temp1.text = (socketServer.max_T - tmp).ToString();
        Temp2.text = (socketServer.max_T - 2 * tmp).ToString();
        Temp3.text = (socketServer.max_T - 3 * tmp).ToString();
        Temp4.text = (socketServer.max_T - 4 * tmp).ToString();
        TempMin.text = socketServer.min_T.ToString();

    }

    // Update is called once per frame
    void Update()
    {
        if (!fixedT.isOn)
        {
            tmp = (socketServer.max_T - socketServer.min_T) / 5;
            TempMax.text =
            string.Format("{0:F2}", socketServer.max_T);
            Temp1.text =
            string.Format("{0:F2}", (socketServer.max_T - tmp));
            Temp2.text = string.Format("{0:F2}", (socketServer.max_T - 2 * tmp));
            Temp3.text = string.Format("{0:F2}", (socketServer.max_T - 3 * tmp));
            Temp4.text = string.Format("{0:F2}", (socketServer.max_T - 4 * tmp));
            TempMin.text = string.Format("{0:F2}", socketServer.min_T);
        }
        else
        {
            tmp = (socketServer.setMax_t - socketServer.setMin_t) / 5;
            TempMax.text =
            string.Format("{0:F2}", socketServer.setMax_t);
            Temp1.text =
            string.Format("{0:F2}", (socketServer.setMax_t - tmp));
            Temp2.text = string.Format("{0:F2}", (socketServer.setMax_t - 2 * tmp));
            Temp3.text = string.Format("{0:F2}", (socketServer.setMax_t - 3 * tmp));
            Temp4.text = string.Format("{0:F2}", (socketServer.setMax_t - 4 * tmp));
            TempMin.text = string.Format("{0:F2}", socketServer.setMin_t);
        }
    }
}
