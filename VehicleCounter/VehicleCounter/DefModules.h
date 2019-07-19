#ifndef DefModules_H
#define DefModules_H

#include <cvaux.h>

#include "myBlobTrackAnalysis.h"
//#include <cxtypes.h>

//List of Foreground detector
CvFGDetector* CreateFGDetectionModule_FGD() { return cvCreateFGDetectorBase(CV_BG_MODEL_FGD, NULL); }
CvFGDetector* CreateFGDetectionModule_GM() { return cvCreateFGDetectorBase(CV_BG_MODEL_MOG, NULL); }

//Define Foreground Detector
typedef struct DefModule_FGDetector
{
	CvFGDetector* (*create)();
	const char* nickname;
	const char* desciption;
} DefModule_FGDetector;

DefModule_FGDetector FGDetectionModules[]=
{
	{CreateFGDetectionModule_GM, "GM", "Adaptive Background mixture models for realtime tracking"},
	{CreateFGDetectionModule_FGD, "FOD", "Foreground Object Detection from Videos containing complex Background"},
	{NULL, NULL, NULL}
};
//List of Blob Detector
typedef struct DefModule_BlobDetector
{
	CvBlobDetector* (*create)();
	const char* nickname;
	const char* description;
} DefModule_BlobDetector;

DefModule_BlobDetector BlobDetectionModules[]=
{
	{cvCreateBlobDetectorCC, "BD_CC", "Detect new blob tracking cc of FG mask"},
	{NULL, NULL, NULL}
};

//List of blob tracking Modules
typedef struct DefModule_BlobTracker
{
	CvBlobTracker* (*create)();
	const char* nickname;
	const char* description;
}DefModule_BlobTracker;

DefModule_BlobTracker BlobTrackerModules[]=
{
	{cvCreateBlobTrackerCC, "BT_CC", "Simple Connected component tracking"},
	{cvCreateBlobTrackerCCMSPF, "BT_CCMSPF", "connected component tracking and MSPF resolver for collision"},
	{NULL, NULL, NULL}
};

//List of Blob Trajectory post processing modules

typedef struct DefModule_BlobTrajectoryPostProcessingModule
{
	CvBlobTrackPostProc* (*create)();
	const char* nickname;
	const char* desciption;
}DefModule_BlobTrajectoryPostProcessingModule;

DefModule_BlobTrajectoryPostProcessingModule BlobTrackPostProcModules[]=
{
	{cvCreateModuleBlobTrackPostProcKalman, "Kalman", "Kalman filtering of blob position and size"},
	{NULL, NULL, NULL}
};

//List of Blob Trajectory analysis modules

typedef struct DefModule_BlobTrajectoryAnalysisModule
{
	CvBlobTrackAnalysis* (*create)();
	const char* nickname;
	const char* description;
}DefModule_BlobTrajectoryAnalysisModule;

DefModule_BlobTrajectoryAnalysisModule BlobTrackAnalysisModules[]=
{
	{cvCreateModuleBlobTrackAnalysisHistSS, "SS", "Base on Histogram analysis of 4D FV (startPos, StopPos)"},
	{cvCreateModuleBlobTrackAnalysisHistPVS, "PVS", "Base on Histogram analysis of 5D hist Pos, velocity, state"},
	{cvCreateModuleBlobTrackAnalysisHistPV, "PV", "Base on Histogram analysis of 4D hist pos, velocity"},
	{cvCreateModuleBlobTrackAnalysisHistP, "P", "Base on Histogram analysis of 2D hist position(x, y)"},
	{cvCreateModuleBlobTrackAnalysisTrackDist, "TD", "none"},
	{myCreateBlobTrackAnalysisSSDist, "SSD", "Base on analysis of Start and Stop position Distance"},
	{NULL, NULL, NULL}
};

//Define Vehicle Classifying Module

int ClassifyingVehicle(CvBlob* blob, float threshold = 0)
{
	if(threshold <=0 ) return 0; //undefinable
	else{
		int BlobArea = blob->h*blob->w;
		if(BlobArea < threshold) return 1; //motorbike
		else return 2; //car
	}
}
//Define Counter
class Counter
{
private:
	CvBlobSeq      BlobList;
	int            counter;
	int            frame;
public:
	Counter(int maxDist = 20):BlobList(sizeof(myBlob))
	{ 
		counter = 0;
		frame = 0;
	}
	~Counter(){}
	void AddBlob(CvBlob* pB)
	{
		myBlob* Blob = (myBlob*)BlobList.GetBlobByID(CV_BLOB_ID(pB));
		if(Blob == NULL)
		{
			myBlob blobNew;
			blobNew.blob = pB[0];
			blobNew.flag = 1;
			blobNew.LastFrame = frame;
			BlobList.AddBlob((CvBlob*)&blobNew);
			counter++;
		}
		else
		{
			Blob->blob = pB[0];
			Blob->LastFrame =frame;
		}
		
	}
	void Process()
	{
		//delete unused blob
		for(int i = BlobList.GetBlobNum() - 1; i >= 0; --i)
		{
			
			//Delete unused blob
			myBlob* Blob = (myBlob*)BlobList.GetBlob(i);
			if(Blob->LastFrame < frame)
			{
				BlobList.DelBlob(i);
			}
		}
		frame++;
	}
	int GetCounter()
	{
		return counter;
	}
};
#endif