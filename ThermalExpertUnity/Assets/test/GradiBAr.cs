using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GradiBAr : MonoBehaviour {
    
    int width;
    int height;
	// Use this for initialization
	void Start () {
        width = (int)this.GetComponent<RectTransform>().sizeDelta.x;
        height = (int)this.GetComponent<RectTransform>().sizeDelta.y;
    }
	
	// Update is called once per frame
	void Update () {
        Color[] c = new Color[width * height];
        
        
        
        for(int i=0;i<height;i++)
        {
            for(int j = 0; j<width;j++)
            {
                c[j+i*width] = Gradi.g1.Evaluate((float)i / (float)height);
            }
        }

        Texture2D tmp = new Texture2D(width, height, TextureFormat.RGB24, false);
        tmp.SetPixels(c);
        tmp.Apply();

        this.GetComponent<Image>().sprite = Sprite.Create(tmp, new Rect(0,0,width,height), new Vector2(0, 0));

	}
}
