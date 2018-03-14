using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class LocationTemp : MonoBehaviour {

    public Server sockerServer;
	// Use this for initialization
	void Start () {
        this.GetComponent<Text>().text = "That Location's Temp is ";
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (sockerServer.tempurture != 0) 
            this.GetComponent<Text>().text = "That Location's Temp is " + string.Format("{0:F2}", sockerServer.tempurture);
    }
}
