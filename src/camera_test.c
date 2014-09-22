#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//#include "opencv/cv.h"
//#include "opencv/highgui.h"

#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"

#include "libfreenect/libfreenect.h"

#include <pthread.h>

#include <time.h>
#include <sys/time.h>

#include <termios.h>
#include <unistd.h>

#include <fcntl.h>


#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <GLES2/gl2ext.h>
#include <EGL/eglext.h>


#define false 0
#define true 1

struct timeval start, end;
long mtime, seconds, useconds;   


/////// freenect vars
uint16_t t_gamma[2048];
pthread_t freenect_thread;
volatile int die = 0;

int depth_window;
int video_window;

pthread_mutex_t depth_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t video_mutex = PTHREAD_MUTEX_INITIALIZER;

// back: owned by libfreenect (implicit for depth)
// mid: owned by callbacks, "latest frame ready"
// front: owned by GL, "currently being drawn"
uint8_t *depth_mid, *depth_front;
uint8_t *rgb_back, *rgb_mid, *rgb_front;

GLuint gl_depth_tex;
GLuint gl_rgb_tex;

freenect_context *f_ctx;
freenect_device *f_dev;
int freenect_led;

freenect_video_format requested_format = FREENECT_VIDEO_YUV_RGB;
freenect_video_format current_format = FREENECT_VIDEO_YUV_RGB;
freenect_resolution requested_resolution = FREENECT_RESOLUTION_MEDIUM;
freenect_resolution current_resolution = FREENECT_RESOLUTION_MEDIUM;

pthread_cond_t gl_frame_cond = PTHREAD_COND_INITIALIZER;
int got_rgb = 0;
int got_depth = 0;
int depth_on = 1;


void rgb_cb			(freenect_device *dev, void *rgb, uint32_t timestamp);
void depth_cb			(freenect_device *dev, void *v_depth, uint32_t timestamp);
void *freenect_threadfunc	(void *arg);
//////////////////

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
	
	
	
	
	printf ("\nInitializing Kinect...");
	if (freenect_init(&f_ctx, NULL) < 0) {
		printf("Failed\n");
		return 1;
	}
	printf ("OK\n");

	freenect_set_log_level(f_ctx, FREENECT_LOG_DEBUG);
	freenect_select_subdevices(f_ctx, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));
	
	int nr_devices = freenect_num_devices (f_ctx);
	printf ("\t\tNumber of devices found: %d\n", nr_devices);

	int user_device_number = 0;
	if (argc > 1)
		user_device_number = atoi(argv[1]);

	if (nr_devices < 1)
		return 1;

	if (freenect_open_device(f_ctx, &f_dev, user_device_number) < 0) {
		printf("\t\tCould not open device\n");
		return 1;
	}
	
	int i;
	for (i=0; i<2048; i++) {
		float v = i/2048.0;
		v = powf(v, 3)* 6;
		t_gamma[i] = v*6*256;
	}
	
	
	
	
	
	
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

