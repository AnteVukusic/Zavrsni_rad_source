
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include "opencv2\core\core.hpp"
#include <stdio.h>
#include <time.h>
#include <iomanip>
#include <cmath>
#include <string.h>




#define _CRT_SECURE_NO_WARNINGS
using namespace std;
using namespace cv;
#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS
#endif
Point2f AbsDotCenter(Point,Mat*);

Point2f CircleFitting(float[15][2]);
Point2f Center(Mat);

int main(int argv, char** argc)
{
	
	Mat Slika = imread("POMACI_test/pomak0.tiff", CV_LOAD_IMAGE_GRAYSCALE);
	Point2f DefaultValues;
	DefaultValues = Center(Slika);
	
	string temp;
	
	Point2f NewValues;
	while (1)
	{
		cout << "\n\tUnesi ime slike:\t";
		cin >> temp;
		clock_t begin = clock();
		
	
		Mat Slika_New = imread("POMACI_test/"+temp+".tiff", CV_LOAD_IMAGE_GRAYSCALE);
		NewValues = Center(Slika_New);

		printf("\n\tPomak u odnosu na default(pomak0) u ovome slucaju je: %f milimetara ", sqrt(((NewValues.y - DefaultValues.y)*(NewValues.y - DefaultValues.y)+(NewValues.x-DefaultValues.x)*(NewValues.x - DefaultValues.x)))*0.814);
		temp = "";
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("\nVijeme izvrsavanja:\t%f\n----------------------------------------------------------\n", time_spent);
	}

	return 0;
}
Point2f Center(Mat dst)

{

	

	float Dots[15][2];
	Point p1, p2;
	
	Point2f CenterOfCircle;
	double a, b;
	int k = 0;
	int l = 0;
	b = 255;
	FileStorage fs("out_camera_data.xml", FileStorage::READ);

	

	
	
	while (b > 10 )
	{
		
		minMaxLoc(dst, &a, &b, &p1, &p2);//Trazenje prvog piksela sa najmanjim intenzitetom i onog najveceg
		
			Point2f T = AbsDotCenter(p2, &dst);
			
				Dots[l][0] = T.x;
				Dots[l][1] = T.y;
				l++;
			
			
	}	
	CenterOfCircle=CircleFitting(Dots);

	
	
	return CenterOfCircle;
	
}

Point2f AbsDotCenter(Point p,Mat *Default)
{
	Mat k = *Default;
	
	float kx=0;
	float ky=0;
	float sum_k=0;
	float X, Y;

	for (int i = p.x - 10; i < p.x + 10; i++)
	{
		for (int j = p.y - 10; j < p.y + 10; j++)
		{
			
				kx += p.x*k.at<uchar>(j,i);
				ky += p.y*k.at<uchar>(j,i);
				sum_k += k.at<uchar>(j,i);
				k.at<uchar>(j,i) = 0;
			
			
				
		}
	}
	
	X = kx / sum_k;
	Y = ky / sum_k;
	
	
	Point2f AbsCenter;
	AbsCenter.x = X;
	AbsCenter.y = Y;
	return AbsCenter;
}
Point2f CircleFitting(float Dots[15][2])
{
	Point2f CircleCenter;
	float o11 = 0;
	float o20 = 0;
	float o02 = 0;
	float o30 = 0;
	float o03 = 0;
	float o21 = 0;
	float o12 = 0;
	float sumx = 0;
	float sumy = 0;
	float sumx2 = 0;
	float sumy2 = 0;
	float sumx3 = 0;
	float sumy3 = 0;
	float timesxy = 0;
	float timesx2y = 0;
	float timesxy2 = 0;
	double x, y=0;
	int n = 14;
	for (int i = 0; i < n; i++)
	{
		//printf("\n%f\t%f", Dots[i][0], Dots[i][1]);
		sumx += Dots[i][0];
		sumy += Dots[i][1];
		sumx2 +=( Dots[i][0] * Dots[i][0]);
		sumy2 += (Dots[i][1] * Dots[i][1]);
		sumx3 += (Dots[i][0] * Dots[i][0]* Dots[i][0]);
		sumy3 += (Dots[i][1] * Dots[i][1] * Dots[i][1]);
		timesxy += (Dots[i][1] * Dots[i][0]);
		timesx2y += (Dots[i][0] * Dots[i][0] * Dots[i][1]);
		timesxy2 += (Dots[i][1] * Dots[i][1] * Dots[i][0]);
	}
	o11 = n*timesxy - sumx*sumy;
	o20 = n*sumx2 - sumx*sumx;
	o02 = n*sumy2 - sumy*sumy;
	o30 = n*sumx3 - sumx2*sumx;
	o03 = n*sumy3 - sumy2*sumy;
	o21 = n*timesx2y - sumx2*sumy;
	o12 = n*timesxy2 - sumy2*sumx;
	CircleCenter.x = ((o30 + o12)*o02 - (o03 + o21)*o11) / (2 * (o20*o02 - o11*o11));
	CircleCenter.y= ((o03 + o21)*o20 - (o30 + o12)*o11) / (2 * (o20*o02 - o11*o11));
	
	return CircleCenter;
}



