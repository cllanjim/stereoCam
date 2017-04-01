#ifndef __DEPTH_BOKEH_EFFECT_H__
#define __DEPTH_BOKEH_EFFECT_H__


extern "C" {


/* Image data type defination */      
#define DBE_IMAGE_TYPE_NV12            0
#define DBE_IMAGE_TYPE_NV21            1
#define DBE_IMAGE_TYPE_GRAY            2
#define DBE_IMAGE_TYPE_YV12            3

/* SDK error code */
#define DBE_SUCCESS                    0
#define DBE_ERR_UNINITIALIZED         -1
#define DBE_ERR_PARAMETER             -2
#define DBE_ERR_PROCESS               -3


typedef enum tagdbeOrientation
{
	ORI_NONE = -1,
	ORI_0,
	ORI_90,
	ORI_180,
	ORI_270
}dbeOrientation;

/*image data structure defination */
typedef struct tagdbeImageData
{
	unsigned char *pImageBuffer;   //pointer to image buffer;
	unsigned int   nImageBufferSize;    //size of image buffer
	unsigned int   nWidth;         //width of image;
	unsigned int   nHeight;        //height of image;
	unsigned int   nImageType;     //image type;
	unsigned int   nWidthStride;   //width stride of image;
	unsigned int   nHeightStride;  //height stride of image;
}dbeImageData;

/*disparity image data structure defination */
typedef struct tagdbeDisparityData
{
	unsigned char *pImageBuffer;   //pointer to image buffer;
	unsigned int   nWidth;         //width of image;
	unsigned int   nHeight;        //height of image;		 
}dbeDisparityData;


#define _In_
#define _In_opt_
#define _Inout_
#define _Out_

/*
* Remarks:      Get the version of Depth Bokeh Effect SDK.
* Return Value: SDK version
*/
unsigned int dbeGetVersion();

/*
* Remarks:      Initialize Depth Bokeh Effect SDK.
* Return Value: error codes indicating "dbeInit" status.
* Parameters:   pParameterFileList  -- parameter file
                bEnableLog          -- Enable log, log TAG: BlackSesame
*/	
int dbeInit(_In_ const char *pParameterFile, _In_ bool bEnableLog);

/*
* Remarks:      Release Depth Bokeh Effect SDK.
* Return Value: error codes indicating "dbeRelease" status.
* Parameters:   n.a.
*/
int dbeRelease();


/*
* Remarks:      Prepare computation, please call this function before dbeDisparityImage and dbeBokehImage.
* Return Value: error codes indicating "dbePrepareComputation" status.
* Parameters:   pMainImage         -- main image input
                pSecondImage       -- second image input
                fUpScale            -- upscale ratio of the main image.
                fDownScale          -- downscale ratio of the second image
				nReserved           -- reserved parameter (Set 0 to compute disparity)
				orientation         -- phone orientation when take photo
				bCameraAlongX       -- if bCameraAlongX==true, dual camera configured along X(Horizontal),
				                       if bCameraAlongX==false, dual camera configured along Y(Vertical)

*/


int dbePrepareComputation(_In_ dbeImageData* pMainImage, _In_ dbeImageData* pSecondImage, _In_ float fUpScale,
	_In_ float fDownScale, _In_ int nReserved, _In_ dbeOrientation orientation, _In_ bool bCameraAlongX);



/*
* Remarks:      Create Bokeh image.
* Return Value: error codes indicating "dbeBokehImage" status.
* Parameters:   nFocusX            -- X coordinate of focus point
*               nFocusY            -- Y coordinate of focus point
*               nRefocusWinW       -- Refocus Window Size
                fNumber            -- fNumber control the blur strength
				pBokehImage        -- bokeh image output.
*/

int dbeBokehImage(_In_ int nFocusX, _In_ int nFocusY, _In_ int nRefocusWinW, _In_ float fFNumber, _Out_ dbeImageData* pBokehImage);


}

#endif
