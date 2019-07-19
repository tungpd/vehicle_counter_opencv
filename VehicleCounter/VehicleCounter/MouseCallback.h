#ifndef MouseCallback_H
#define MouseCallback_H

#include <cv.h>
#include <highgui.h>

int isMouseMove1 = false;
int isMouseMove  = true;
int isHighLighButton = false;
int isPlayButtonClicked = false;
int isMouseMoveOnSiderBar = false;
int isSiderMoved = true;
int isRMouseButtonClicked = false;

CvRect rect = cvRect(0, 0, 0, 0);
int p = 0;
//
//
//Create Mouse Callback
//
void on_mouse(int event, int x, int y, int flags, void* param)
{
	p=x;
	IplImage* img = (IplImage*) param;
	/*
	if(event == CV_EVENT_MOUSEMOVE)
	{
		isMouseMove = 100;
		//
		rect.height = y - rect.y;
		rect.width = x - rect.x;
	}
	*/
	switch(event)
	{
	case CV_EVENT_LBUTTONDOWN:
		{
			isMouseMove1 = true;
			rect = cvRect(x, y, 0, 0);
			
		}
		break;
		
	case CV_EVENT_MOUSEMOVE:
		{
			isMouseMove = 100;
			if(isMouseMove1){
				rect.height = y - rect.y;
				rect.width = x - rect.x;
			}

		}
		break;
		
	case CV_EVENT_LBUTTONUP:
		{
			isMouseMove1 = false;
			if(rect.width<0)
			{
				rect.x += rect.width;
				rect.width*=-1;
			}
			if(rect.height<0)
			{
				rect.y += rect.height;
				rect.height*=-1;
			}
			
			cvRectangle(img, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), cvScalar(255,255,255));
			
			cvSetImageROI(img, rect);
			cvAddS(img, cvScalar(50), img);
			cvResetImageROI(img);
		
		}
		break;
	case CV_EVENT_RBUTTONDOWN:
		{
			isRMouseButtonClicked = true;
		}
	}
	//is mouse move on PlayButton
	if((img->width/2 - 15 < x)&&(x < img->width/2 + 15)&&(img->height-45<y)&&(img->height-5>y))
	{
		isHighLighButton = 1;
		if(event == CV_EVENT_LBUTTONDOWN)
		{
			if(isPlayButtonClicked ==false)
			{
				isPlayButtonClicked = true;
			}else isPlayButtonClicked = false;
		}
	}
	else isHighLighButton = 0;
	//is mouse move on Sider bar
	if(img->height-49 > y && img->height-55 < y)
	{
		isMouseMoveOnSiderBar = true;
		if(event == CV_EVENT_LBUTTONDOWN)
			isSiderMoved = true;
		else isSiderMoved = false;
	}
	else isMouseMoveOnSiderBar = false;

}

#endif