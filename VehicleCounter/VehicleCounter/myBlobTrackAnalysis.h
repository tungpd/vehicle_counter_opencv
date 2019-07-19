#ifndef myBlobTrackAnalysis_H
#define myBlobTrackAnalysis_H
#include <cvaux.h>
#include <stdio.h>
//Create BlobTrackAnalysisModule base on Start and Stop position Distance
typedef struct myBlob
{
	CvBlob blob;
	int flag;
	int LastFrame;
}myBlob;

typedef struct myBlobTrack
{
	CvBlob blob;
	CvPoint StartPos;
	float state;
	int LastFrame;

} myBlobTrack;

class myBlobTrackAnalysisSSDist: public CvBlobTrackAnalysis
{
private:
	CvBlobSeq BlobList;
	int max_dist;
	int frame;
public:
	myBlobTrackAnalysisSSDist(int maxDist = 20):BlobList(sizeof(myBlobTrack))
	{
		max_dist = maxDist;
		frame = 0;
		SetModuleName("SSD");
	}
	~myBlobTrackAnalysisSSDist(){}

	void AddBlob(CvBlob* pBlob)
	{
		myBlobTrack* pFVBlob = (myBlobTrack*)BlobList.GetBlobByID(CV_BLOB_ID(pBlob));
		if(pFVBlob == NULL)
		{
			myBlobTrack BlobNew;
			BlobNew.blob = pBlob[0];
			BlobNew.LastFrame = frame;
			BlobNew.StartPos = cvPoint((int)pBlob->x, (int)pBlob->y);
			BlobNew.state = 0;
			BlobList.AddBlob((CvBlob*)&BlobNew);
			//pFVBlob = (myBlobTrack*)BlobList.GetBlobByID(CV_BLOB_ID(pBlob));
		}
		else
		{
			pFVBlob->blob = pBlob[0];
			pFVBlob->LastFrame = frame;
		}
	}

	void Process(IplImage* pImg, IplImage* pFG)
	{
		float dx, dy;
		for(int i = BlobList.GetBlobNum() - 1; i >= 0; --i)
		{
			
			//Delete unused blob
			myBlobTrack* pFVBlob = (myBlobTrack*)BlobList.GetBlob(i);
			if(pFVBlob->LastFrame < frame)
			{
				BlobList.DelBlob(i);
			}
			else//update state
			{
				//printf("update state");
				
				dx = pFVBlob->blob.x - pFVBlob->StartPos.x;
				dy = pFVBlob->blob.x - pFVBlob->StartPos.x;
				
				if(dx*dx + dy*dy > 2500)
				{
					
					pFVBlob->state = 1;
				}
				else pFVBlob->state = 0;

			}
		}
		
		frame++;
	}
	float GetState(int BlobID)
	{
		myBlobTrack* pB = (myBlobTrack*)BlobList.GetBlobByID(BlobID);
		//printf("test sucessed");
		return pB?pB->state:0.0f;
	}

	void Release() { delete this; }
};

CvBlobTrackAnalysis* myCreateBlobTrackAnalysisSSDist() {  return (CvBlobTrackAnalysis*) new myBlobTrackAnalysisSSDist(); }
#endif