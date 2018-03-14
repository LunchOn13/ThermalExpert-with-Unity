using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Gradi : MonoBehaviour
{
    static public Gradient  g1;
    static public Gradient zebra;
    static public float gain = 0.2f;
    // Use this for initialization
    void Start()
    {
        GradientColorKey[] gck;
        GradientAlphaKey[] gak;
        g1 = new Gradient();
        gck = new GradientColorKey[6];
        gck[0].color = Color.black;
        gck[0].time = 0.0F;
        gck[1].color = Color.magenta;
        gck[1].time = 0.2f;
        gck[2].color = Color.blue;
        gck[2].time = 0.4f;
        gck[3].color = Color.green;
        gck[3].time = 0.6f;
        gck[4].color = Color.yellow;
        gck[4].time = 0.8f;
        gck[5].color = Color.red;
        gck[5].time = 1f;

        gak = new GradientAlphaKey[6];
        gak[0].alpha = 1.0f;
        gak[0].time = 0f;
        gak[1].alpha= 1.0f;
        gak[1].time = 1.0f;
        gak[2].alpha = 1.0f;
        gak[2].time = 1.0f;
        gak[3].alpha = 1.0f;
        gak[3].time = 1.0f;
        gak[4].alpha = 1.0f;
        gak[4].time = 1.0f;
        gak[5].alpha = 1.0f;
        gak[5].time = 1.0f;

        g1.SetKeys(gck, gak);



        GradientColorKey[] gck_z;
        GradientAlphaKey[] gak_z;
        zebra = new Gradient();
        gck_z = new GradientColorKey[2];
        gck_z[0].color = Color.black;
        gck_z[0].time = 0.0f;
        gck_z[1].color = Color.white;
        gck_z[1].time = 1.0f;
            
        gak_z = new GradientAlphaKey[2];
        gak_z[0].alpha = 1.0f;
        gak_z[0].time = 0f;
        gak_z[1].alpha= 1.0f;
        gak_z[1].time = 1.0f;

        zebra.SetKeys(gck_z, gak_z);



    }
}
