//opencv Libs
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
//my libs
#include "MouseCallback.h"
#include "DefModules.h"
#include "DrawPlayBar.h"

//C libs
#include <stdio.h>


CvRect rect1;
//Run blob tracking auto system

static int RunBlobTrackingAuto(CvBlobTrackerAuto* pTracker, CvCapture* pCap, const float threshold = 0/*for classifying*/)
{
	int                     OneFrameProcess = 0;
    //int                     key;
    int                     FrameNum = 0;
	//static int				counter = 0;
	Counter					_counter;
	char					s[1024]; // counter in string 
	char					*AppName="Vehicle Counter (Version 2.1.0)", 
							*GroupName="Developed by THSV-TT2";
	IplImage                *temp, *temp1;
							
	//for draw infor on frames
	int         line_type = CV_AA;
    CvFont      font;
	cvInitFont( &font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0, 0.7, line_type );
	
	CvSize frameSize = cvSize(360, 280);
   

	IplImage*   _pImg  = NULL;
	IplImage*   pImg  = cvCreateImage(frameSize, IPL_DEPTH_8U, 3);
	
	_pImg = cvQueryFrame(pCap);
	cvResize(_pImg, pImg);
	
	temp = cvCloneImage(pImg);
	temp1 = cvCloneImage(pImg);
	
	 cvNamedWindow( "Tracking", 0);
	// Set Mouse Callback
	cvSetMouseCallback("Tracking", on_mouse, (void*) temp);
	//
	//set Region of Interesting
	while(1)
	{
		rect1 = rect;
		if(isRMouseButtonClicked ==true) 
		{
			cvCopyImage(pImg, temp);
			isRMouseButtonClicked = false;
		}
		cvCopy(temp, temp1);
		if(isMouseMove1) cvRectangle(temp1, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), cvScalar(255,255,255));
		
		cvShowImage("Tracking", temp1);
		if(cvWaitKey(30)==27) break;
	}
	if(pImg->width < rect1.x + rect1.width || rect1.x + rect1.width < 10 || pImg->height < rect1.y + rect1.height || rect1.y + rect1.height < 10)
	{
		rect1.x = 0;
		rect1.y = 0;
		rect1.width = pImg->width;
		rect1.height = pImg->height;
	}
	CvSize frameSize1 = cvSize(rect1.width, rect1.height);   //new frame size
	

	IplImage* pImg1 = cvCreateImage(frameSize1, 8, 3);
	IplImage* temp2 = cvCreateImage(frameSize1, 8, 3);
	//reset mouse callback
	cvSetMouseCallback("Tracking", on_mouse, (void*) pImg1);
	//main loop
    for( FrameNum=0; pCap; FrameNum++)
    {   /* Main loop: */
		
		cvWaitKey(1);	
		_pImg = cvQueryFrame(pCap);
		
		//Seting last Frame
        if(_pImg == NULL)
		{
			IplImage* LastImg = cvCreateImage(frameSize1, 8, 3);
			cvSet(LastImg, cvScalar(255, 0, 0));
			cvPutText(LastImg, 
				"ohhhh! Mission Completed!",
				cvPoint(frameSize1.width/2 - 100, 
				frameSize1.height/2),
				&font,
				cvScalar(0, 255, 255)
				);

			cvPutText(LastImg, AppName, cvPoint(20,50), &font, cvScalar(0, 0, 255));
			cvPutText(LastImg, GroupName, cvPoint(20,80), &font, cvScalar(0, 0, 255));
			cvPutText(LastImg, "Result: ", cvPoint(20, 110), &font, cvScalar(0, 0, 255));
			cvPutText(LastImg, "Vehicles", cvPoint(180, 110), &font, cvScalar(0, 0, 255));
			cvPutText(LastImg, s, cvPoint(120, 110), &font, cvScalar(0, 0, 255));

			cvShowImage("Tracking", LastImg);

			cvWaitKey();
				break;
		
		}//end
		else cvResize(_pImg, pImg, 1);
		
		/*
  		cvSetImageROI(pImg, rect1);
		cvCopyImage(pImg, pImg1);
		cvResetImageROI(pImg);
		*/
		
		pTracker->Process(pImg);
	
		
        if(pImg)
        {   /* Draw all information about test sequence: */
            char        str[1024];
            int         i=0;
            IplImage*   pI = cvCloneImage(pImg);
		
			cvPutText(pI, AppName, cvPoint(10,50), &font, cvScalar(0, 0, 255));
			cvPutText(pI, GroupName, cvPoint(10,80), &font, cvScalar(0, 0, 255));
				//
			cvInitFont( &font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0, 0.7, line_type );
			
            for(i=pTracker->GetBlobNum(); i>0; i--)
            {
	
                CvSize  TextSize;
				
                CvBlob* pB = pTracker->GetBlob(i-1);
                CvPoint p = cvPoint(cvRound(pB->x*256),cvRound(pB->y*256));
				
                CvSize  s = cvSize(MAX(1,cvRound(CV_BLOB_RX(pB)*256)), MAX(1,cvRound(CV_BLOB_RY(pB)*256)));
				
                int c = cvRound(255*pTracker->GetState(CV_BLOB_ID(pB)));
			
				//printf("%f\n", pTracker->GetState(CV_BLOB_ID(pB)));
                cvEllipse( pI,
                    p,
                    s,
                    0, 0, 360,
                    CV_RGB(c,255-c,0), cvRound(1+(3*0)/255), CV_AA, 8 
					);
				//

				//
                p.x >>= 8;
                p.y >>= 8;
                s.width >>= 8;
                s.height >>= 8;
                sprintf(str,"%03d",CV_BLOB_ID(pB)+1);
				//set counter
				
				if(c==255)
				{
					_counter.AddBlob(pB);
				}
				//
				cvGetTextSize( str, &font, &TextSize, NULL );
				//Classifying
				if(c==255)
				{
					int classificator = ClassifyingVehicle(pB, threshold);
					if(classificator == 0) cvPutText( pI, "", p, &font, CV_RGB(0,255,255));
					else{
						if(classificator == 1) cvPutText( pI, "motorbike", p, &font, CV_RGB(0,255,255));
						else  cvPutText( pI, "car", p, &font, CV_RGB(0,255,255));
						}
				}
				/*
                //draw blobID
                p.y += cvRound(s.height/2);
				p.x -= cvRound(s.width/2);
                cvPutText( pI, str, p, &font, CV_RGB(0,255,255));
				//
				*/

			
            }   /* Next blob. */

			//Draw counter
			
			itoa(_counter.GetCounter(), s, 10);
			cvInitFont( &font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0, 0.7, line_type );
			cvPutText(pI, "Result: ", cvPoint(20, 110), &font, cvScalar(0, 0, 255));
			cvPutText(pI, s, cvPoint(120, 110), &font, cvScalar(0, 0, 255));
			cvPutText(pI, "Vehicles", cvPoint(180, 110), &font, cvScalar(0, 0, 255));
				
			if(isMouseMove > 0)
			{
				DrawPlayBar(pCap, pI, temp2);
			}
			
			else
				cvShowImage( "Tracking", pI );

            cvReleaseImage(&pI);
			
        }   /* Draw all information about test sequence. */
		_counter.Process();
    }   /*  Main loop. */
	
	cvReleaseImage(&pImg);
	cvReleaseImage(&temp);
	cvReleaseImage(&temp1);
	cvReleaseImage(&temp2);
    return 0;
}


int main(int argc, const char** argv)
{
	
	CvBlobTrackerAuto* BlobtrackerAuto;
	CvBlobTrackerAutoParam1 param={0};

	DefModule_FGDetector* FGDectector=0;
	DefModule_BlobDetector* BlobDetector=0;
	DefModule_BlobTracker* BlobTracker=0;
	DefModule_BlobTrajectoryPostProcessingModule* BlobTrackPostProcModule=0;
	DefModule_BlobTrajectoryAnalysisModule* BlobTrackAnalysisModule=0;

	const char* FGDetectorName=0;
	const char* BlobDetectorName=0;
	const char* BlobTrackerName=0;
	const char* BlobTrackPostProcModuleName=0;
	const char* BlobTrackAnalysisModuleName=0;
	const char* AVI_name = "D:\\Temp\\Video5.avi";
	float threshold;

	if(argc==8)
	{
		FGDetectorName = argv[1];
		BlobDetectorName = argv[2];
		BlobTrackerName = argv[3];
		BlobTrackPostProcModuleName = argv[4];
		BlobTrackAnalysisModuleName = argv[5];
		AVI_name = argv[6];
		threshold = (float)atoi(argv[7]);
	}else
	{

		//print help
		cvWaitKey();
		return 1;
		
	}
	//Create Video Capture
	CvCapture* cap = cvCaptureFromFile(AVI_name);
	if(cap==NULL)
	{
			printf("cannot load the video file");
			cvWaitKey();
			return 1;
	}
	//Set up Modules
	for(FGDectector=FGDetectionModules; FGDectector->nickname; FGDectector++)
	{
		if((FGDetectorName && stricmp(FGDetectorName, FGDectector->nickname))==0) break;
		
	}
	for(BlobDetector=BlobDetectionModules; BlobDetector->nickname; BlobDetector++)
	{
		if((BlobDetectorName && stricmp(BlobDetectorName, BlobDetector->nickname))==0) break;
	}
	for(BlobTracker=BlobTrackerModules; BlobTracker->nickname; BlobTracker++)
	{
		if((BlobTrackerName && strcmp(BlobTrackerName, BlobTracker->nickname))==0) break;
	}
	for(BlobTrackPostProcModule=BlobTrackPostProcModules; BlobTrackPostProcModule->nickname; BlobTrackPostProcModule++)
	{
		if((BlobTrackPostProcModuleName && strcmp(BlobTrackPostProcModuleName, BlobTrackPostProcModule->nickname)) == 0) break;
	}
	for(BlobTrackAnalysisModule=BlobTrackAnalysisModules; BlobTrackAnalysisModule->nickname; BlobTrackAnalysisModule++)
	{
		if((BlobTrackAnalysisModuleName && strcmp(BlobTrackAnalysisModuleName, BlobTrackAnalysisModule->nickname))==0) break;
	}
	//Set defaul Modules
	if((FGDetectorName && FGDectector->nickname) == NULL) FGDectector = FGDetectionModules; //Gaussian Mixture model
	if((BlobDetectorName && BlobDetector->nickname) == NULL) BlobDetector = BlobDetectionModules; //Detect new blob tracking cc of FG mask
	if((BlobTrackerName && BlobTracker->nickname) == NULL) BlobTracker = BlobTrackerModules;     //Simple Connected component tracking
	
	//
	
	//
	//Set parameter for Blob Tracking Auto System
	param.pFG = FGDectector->create();
	param.pFG->SetNickName(FGDectector->nickname);

	param.pBD = BlobDetector->create();
	param.pBD->SetNickName(BlobDetector->nickname);
	param.pBT = BlobTracker->create();
	param.pBT->SetNickName(BlobTracker->nickname);
	
	if(BlobTrackPostProcModule->nickname)
	{
		param.pBTPP = BlobTrackPostProcModule->create();
		param.pBTPP->SetNickName(BlobTrackPostProcModule->nickname);
	}else param.pBTPP->SetNickName("None");
	
	if(BlobTrackAnalysisModule->nickname)
		{
			param.pBTA = BlobTrackAnalysisModule->create();
			param.pBTA->SetNickName(BlobTrackAnalysisModule->nickname);
	}else param.pBTA->SetNickName("None");
	//Print Parameters on screen
	printf("System Parameters\n");
	printf("FG Dectection Module: %s\n", param.pFG->GetNickName());
	printf("Blob Detect Module: %s\n", param.pBD->GetNickName());
	printf("Blob Track Module: %s\n", param.pBT->GetNickName());
	printf("Blob Track Post processsing Module: %s\n", param.pBTPP->GetNickName());
	printf("Blob Trajectory Analysis Module: %s\n", param.pBTA->GetNickName());
	
	BlobtrackerAuto = cvCreateBlobTrackerAuto1(&param);
	
	RunBlobTrackingAuto(BlobtrackerAuto, cap, threshold);
	
	cvReleaseCapture(&cap);
	return 0;
	
}