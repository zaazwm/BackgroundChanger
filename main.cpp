
#include <OpenCV/OpenCV.h>
#include <stdio.h>
//#include "seam.h"

#define SQUA(x) ((x)*(x))

const char  * WINDOW_NAME  = "Background";

int mathabs(int a)
{
	if (a>=0) {
		return a;
	}
	else {
		return 0-a;
	}
}

int comparex(uchar* p,uchar* q)
{
	int i,mark=0;
	int aver=0;
	for (i=0; i<3; i++) {
		//aver+=mathabs(p[i]-q[i]);
		aver+=SQUA(p[i]-q[i]);
	}
	aver/=3;
	//if (aver>23) {
	if (aver>500) {
		mark=1;
	}
	return 1-mark;
}

void copypoint(uchar* q, uchar* p)
{
	int i;
	for (i=0; i<3; i++) {
		p[i]=q[i];
	}
}

int main (int argc, char * const argv[]) 
{	
    // create all necessary instances
    cvNamedWindow (WINDOW_NAME, CV_WINDOW_AUTOSIZE);
    CvMemStorage* storage = cvCreateMemStorage(0);
    assert (storage);
	
	IplImage * backgroundx=NULL;
	IplImage *  backgrounds=NULL;
	IplImage * current_frame=NULL;
	IplImage * backgroundgray=NULL;
	
	if (argc!=4) {
		abort ();
	}
	else {
		backgrounds=cvLoadImage(argv[2], 4);
		backgroundx=cvLoadImage(argv[3], 4);
		backgroundgray=cvLoadImage(argv[3], 0);
		current_frame=cvLoadImage(argv[1],4);
		if (backgroundx==NULL) {
			abort();
		}
	}
	
	
	//get background
	
	cvShowImage (WINDOW_NAME, backgrounds);
	
	//backgroundx=seam(backgroundx, backgrounds->width, backgrounds->height);
	
    // get an initial frame and duplicate it for later work
    IplImage *  draw_image    = cvCreateImage(cvSize (current_frame->width, current_frame->height), IPL_DEPTH_8U, 3);
    IplImage *  get_image    = cvCreateImage(cvSize (current_frame->width, current_frame->height), IPL_DEPTH_8U, 3);
    cvCopy(backgrounds, get_image, NULL);
	
	int h,w;
	
	
    // as long as there are images ...
   		for (w=0; w<=backgrounds->width; w++) {
			for (h=0; h<=backgrounds->height; h++) {
				uchar* p=&CV_IMAGE_ELEM(backgrounds,uchar,h,w*3);
				uchar* x=&CV_IMAGE_ELEM(backgroundx,uchar,h,w*3);
				uchar* q=&CV_IMAGE_ELEM(current_frame,uchar,h,w*3);
				uchar* temp=&CV_IMAGE_ELEM(draw_image,uchar,h,w*3);
				if (comparex(p,q)==1) {
					copypoint(x,temp);
				}
				else {
					copypoint(q,temp);
				}
			}
		}
        
        // just show the image
        cvShowImage (WINDOW_NAME, draw_image);
        
        // wait a tenth of a second for keypress and window drawing
        int key = cvWaitKey (0);
        if (key == 'q' || key == 'Q')
            abort();
    
    
    // be nice and return no error
    return 0;
}
