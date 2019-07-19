#ifndef DrawPlayBar_H
#define DrawPlayBar_H

#include <cv.h>
#include <highgui.h>
#include "MouseCallback.h"
			/*===========================================================================
			 *=========      Create Play Button and Track Bar     =======================
			 *===========================================================================*/
void DrawPlayBar(CvCapture* pCap, IplImage* pI, IplImage* temp2)
{
	
			int pos_frame = (int)cvGetCaptureProperty(pCap, CV_CAP_PROP_POS_FRAMES);
			int frames = (int)cvGetCaptureProperty(pCap, CV_CAP_PROP_FRAME_COUNT);
			
			//
			//
			
			CvPoint points[3] = {cvPoint(pI->width/2 - 15, pI->height - 45),
					cvPoint(pI->width/2 - 15, pI->height - 5),
					cvPoint(pI->width/2 + 15, pI->height - 25)};
			
		
			{
				
				cvCopyImage(pI, temp2);
				//drawing a sider bar
				
				cvLine(temp2, cvPoint(0, temp2->height - 50),
					cvPoint(temp2->width, temp2->height - 50),
					cvScalar(255, 255, 255)
					);
				
				cvLine(temp2, 
					cvPoint(0, temp2->height - 54),
					cvPoint(temp2->width, temp2->height - 54), 
					cvScalar(255, 255, 255)
					);
				
				int siderPos = cvRound((pos_frame*(temp2->width))/frames);
			
				cvLine(temp2,
					cvPoint(0, temp2->height - 52), 
					cvPoint(siderPos, temp2->height - 52),
					cvScalar(255, 200, 100),
					2
					);
				
				if(isMouseMoveOnSiderBar == true)
				{
					cvLine(temp2,
						cvPoint(0, temp2->height - 52),
						cvPoint(siderPos, temp2->height - 52),
						cvScalar(255, 100, 50),
						3
						);
					
					if(isSiderMoved==true)
					{
						cvSetCaptureProperty(pCap, CV_CAP_PROP_POS_FRAMES,
							cvRound(p*frames/temp2->width)
							);
					}
				}
				//
				cvSetImageROI(temp2, cvRect(0,temp2->height - 50, temp2->width, 50));
				cvAddS(temp2, cvScalar(100), temp2);
				cvResetImageROI(temp2);
				

				while(isPlayButtonClicked == false)
				{
					
					if(isMouseMove > 0)
					{
					
						if(isHighLighButton == 1)
						{	
						
							cvFillConvexPoly(temp2, points, 3, cvScalar(150, 180, 0));
						
						}
						else 
						{
						
							cvFillConvexPoly(temp2, points, 3, cvScalar(255, 255, 255));
						
						}
						isMouseMove--;
						cvShowImage("Tracking", temp2);
					}
					else
						cvShowImage("Tracking", pI);
					if(cvWaitKey(1) == 32 ) break;
				}
				
				if(isHighLighButton == 1)
				{	
					cvLine(temp2, 
						cvPoint(pI->width/2 - 5, 
						pI->height - 40), 
						cvPoint(pI->width/2 - 5, pI->height - 10),
						cvScalar(150, 180, 0),
						5
						);
					cvLine(temp2,
						cvPoint(pI->width/2 + 5, pI->height - 40), 
						cvPoint(pI->width/2 + 5, pI->height - 10),
						cvScalar(150, 180, 0),
						5
						);	
				}
				else 
				{	
					cvLine(temp2,
						cvPoint(pI->width/2 - 5, pI->height - 40),
						cvPoint(pI->width/2 - 5, pI->height - 10),
						cvScalar(255, 255, 255),
						5
						);
					cvLine(temp2,
						cvPoint(pI->width/2 + 5, pI->height - 40), 
						cvPoint(pI->width/2 + 5, pI->height - 10),
						cvScalar(255, 255, 255),
						5
						); 	
				}
				isMouseMove--;
				cvShowImage("Tracking", temp2);
				
			}
			
}
			/*===========================================================================
			 *=========      End Creating Play Button and Track Bar     =================
			 *===========================================================================*/
#endif