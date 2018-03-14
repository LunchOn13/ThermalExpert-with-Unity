using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class ShowNUM : MonoBehaviour {

    public InputField InF;
    private Slider 주환이형극혐;
	// Use this for initialization
	void Start () {
        주환이형극혐 = this.GetComponent<Slider>();
        InF.text = 주환이형극혐.value.ToString();
	}

    public void ChangValue()
    {
        InF.text = 주환이형극혐.value.ToString();
    }

    public void ChangeValue_inF()
    {
        주환이형극혐.value = float.Parse(InF.text);
    }
}
