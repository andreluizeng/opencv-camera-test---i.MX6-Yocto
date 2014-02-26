#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//#include "opencv/cv.h"
//#include "opencv/highgui.h"

#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"

#include <time.h>
#include <sys/time.h>

#include <termios.h>
#include <unistd.h>

#include <fcntl.h>

#define false 0
#define true 1

struct timeval start, end;
long mtime, seconds, useconds;    


int Kbhit (void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	CvCapture *capture1;
	CvCapture *capture2;
	CvCapture *capture3;
	CvCapture *capture4;
	
	IplImage *frame1;
	IplImage *frame2;
	IplImage *frame3;
	IplImage *frame4;
	
	char key;
	
	capture1 = cvCaptureFromCAM (CV_CAP_ANY);

	
	if (! capture1) 
	{
	  printf ("\nError creating capture\n");
	  return 0;
	}
	//capture2 = cvCreateCameraCapture (1);
	//capture3 = cvCreateCameraCapture (2);
	//capture4 = cvCreateCameraCapture (3);
	
	// create a window
	//cvNamedWindow ("1", CV_WINDOW_AUTOSIZE); 
	//cvMoveWindow ("1", 0, 0 );

	//cvNamedWindow ("2", CV_WINDOW_AUTOSIZE); 
	//cvMoveWindow ("2", 320, 0 );

	//cvNamedWindow ("3", CV_WINDOW_AUTOSIZE); 
	//cvMoveWindow ("3", 0, 240 );

	//cvNamedWindow ("4", CV_WINDOW_AUTOSIZE); 
	//cvMoveWindow ("4", 320, 240 );
	
	while (! Kbhit())
	{
	  	//gettimeofday(&start, NULL);
		//frame1 = cvQueryFrame(capture1);
		int i = cvGrabFrame(capture1);
		
		if (! i)
		{
		  printf ("\nError grabbing frame\n\n");
		  return 0;
		}
		
		
		frame1 = cvRetrieveFrame(capture1, 0);
		//frame2 = cvQueryFrame(capture2);
		//frame3 = cvQueryFrame(capture3);
		//frame4 = cvQueryFrame(capture4);
		
		cvShowImage ("test", frame1);
		//cvShowImage ("2", frame2);
		//cvShowImage ("3", frame3);
	//	cvShowImage ("4", frame4);

		//gettimeofday(&end, NULL);
		
		// compute and print the elapsed time in millisec
		//seconds  = end.tv_sec  - start.tv_sec;	
		//useconds = end.tv_usec - start.tv_usec;
		//mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	
		//printf( "\nProcess time (gpu) = %ld fps (%ld ms)\n", 1000/mtime, mtime);
		
  		// wait for a key
  		key = cvWaitKey(33);
	}

	// release the image and capture
  	cvDestroyAllWindows();
	cvReleaseCapture (&capture1);
	//cvReleaseCapture (&capture2);
	//cvReleaseCapture (&capture3);
	//cvReleaseCapture (&capture4);
	
	printf ("\n\nHERE !\n\n");
	
	return 0;
}

