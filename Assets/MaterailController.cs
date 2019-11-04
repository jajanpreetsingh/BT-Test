using System.Collections;
using UnityEngine;

//[ExecuteInEditMode]
public class MaterailController : MonoBehaviour
{
    [SerializeField]
    private Material Material;

    private float RippleMagnitude = 0;

    [SerializeField]
    private float AnimTimeInSeconds = 2f;

    [SerializeField]
    private float StopDistortionInSeconds = 15;

    private float tempTime = 0;

    // Start is called before the first frame update
    private void Start()
    {
        tempTime = 0;
        StartCoroutine(StartDistort());
    }

    // Update is called once per frame
    private void Update()
    {
        tempTime += Time.deltaTime;
    }

    private IEnumerator StartDistort()
    {
        float temp = 0;
        float direc = 1;

        while (tempTime < StopDistortionInSeconds)
        {
            yield return new WaitForEndOfFrame();

            temp = temp + (direc * Time.deltaTime);
            RippleMagnitude = temp;

            if (temp <= -AnimTimeInSeconds)
            {
                direc = 1;
            }
            else if (temp >= AnimTimeInSeconds)
            {
                direc = -1;
                temp = AnimTimeInSeconds - 0.05f;
            }
            Material.SetFloat("_Magnitude", RippleMagnitude / AnimTimeInSeconds);
        }

        StartCoroutine(StopDistort());
    }

    private IEnumerator StopDistort()
    {
        StopCoroutine(StartDistort());
        float dir = 1;
        if (RippleMagnitude > 0)
        {
            dir = -1;
        }

        while (decimal.Round((decimal)RippleMagnitude, 2) != 0)
        {
            yield return new WaitForEndOfFrame();

            RippleMagnitude = RippleMagnitude + (dir * Time.deltaTime);

            Material.SetFloat("_Magnitude", RippleMagnitude); dir = 1;
        }

        Material.SetFloat("_Magnitude", 0);
    }

    public void StopDistortion()
    {
        tempTime = 0.95f * StopDistortionInSeconds;
    }
}