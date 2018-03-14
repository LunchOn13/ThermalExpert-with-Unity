using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CalculateImagePosition : MonoBehaviour
{

    public RawImage cameraImage;
    private Vector2 imageSize;
    private int width;
    private int height;
    public static int pos_x = 0;
    public static int pos_y = 0;
    public static bool isClicked = false;
    // Use this for initialization
    void Start()
    {
        imageSize = cameraImage.GetComponent<RectTransform>().sizeDelta;

        Debug.Log(cameraImage.transform.position);

        width = (int)imageSize.x;
        height = (int)imageSize.y;

        Debug.Log(width + " " + height);

    }

    // Update is called once per frame
    void Update()
    {

    }

    private void OnMouseDown()
    {
        isClicked = true;
        pos_x =(int)(Input.mousePosition.x - (Screen.width / 2) + (width / 2));
        pos_y =(int)(Input.mousePosition.y - (Screen.height / 2) + (height / 2));
        Debug.Log("" + (Input.mousePosition.x - (Screen.width / 2) + (width / 2)) + " " + (Input.mousePosition.y - (Screen.height / 2) + (height / 2)));
    }
}