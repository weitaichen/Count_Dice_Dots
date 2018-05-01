// Count Dice Dots.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "opencv2\opencv.hpp"
typedef unsigned char uchar;
using namespace cv;
using namespace std;

cv::Mat weighted_otsu(cv::Mat output);
cv::Mat find_dice(cv::Mat output,cv::Mat input2);
int main(int argc, const char *argv[])
{
	cv::Mat colorimg,result,grayimg,binaryimg;
	colorimg=cv::imread("dice1.png",6);
	vector<Mat> bgr_planes; 
	split(colorimg, bgr_planes);
	bgr_planes[0].copyTo(grayimg); //grayimage is from blue channel of input color image

	binaryimg=weighted_otsu(grayimg); //automatic binarize grayimage
	result=find_dice(binaryimg,colorimg); // find contour of dice and numbers of dots
	cv::imshow("result",result);
	waitKey();
	return 0;
}

cv::Mat weighted_otsu(cv::Mat input){
	cv::Mat output;
	input.convertTo(input,CV_8UC1); //convert input image to 8-bit 1-channel, gray level from 0 to 255
	input=255-input; //inverse input image
	float pi=0,ui=0,u=0;
	float n=input.rows*input.cols;
	vector <float> p0; //cumulated probability for gray level less than selected threshold
	vector <float> p1; //cumulated probability for gray level greater than selected threshold
	vector <float> u0; //mean gray level with gray level less than selected threshold
	vector <float> u1; //mean gray level with gray level greater than selected threshold
	vector <float> delta; //between-class variance for selected threshold
	float h[256];
	for(int k=0;k<256;k++)
		h[k]=0;
	for(int i=0;i<input.rows;i++){ //compute histogram of image
		for(int j=0;j<input.cols;j++){
			h[input.ptr<uchar>(i)[j]]++;
		}
	}
	for(int k=0;k<256;k++){
		pi+=h[k]/n;
		p0.push_back(pi);
		p1.push_back(1-pi);
	}
	for(int k=0;k<256;k++){
		if(p0[k]!=0)
			for(int l=0;l<k+1;l++)
				ui+=float(l)*h[l]/n/p0[k];
		else
			ui=0;
		u0.push_back(ui);
		if(p1[k]!=0)
			for(int l=k+1;l<256;l++)
				u+=float(l)*h[l]/n/p1[k];
		else
			u=0;
		u1.push_back(u);
		u=0;
		ui=0;
	}
	for(int k=0;k<256;k++){ //between-class variance with weighted Otsu's method formula
		delta.push_back(p0[k]*p0[k]*u0[k]*u0[k]+p1[k]*u1[k]*u1[k]);
	}
	float max=0;
	int TH=0;
	for(int i=0;i<256;i++){  //find maximum between-class variance
		if(delta[i]>max){
			max = delta[i];
			TH=i;
		}
	}
	threshold(input, output,TH,255,CV_THRESH_BINARY_INV); //binarize image with computed threshold
	return output;
}

cv::Mat find_dice(cv::Mat binaryimg,cv::Mat colorimg){
	vector< vector<cv::Point> > contourTmp;
	vector< cv::Vec4i > hierarchy;
	cv::findContours(binaryimg.clone(), contourTmp, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, Point(0, 0) ); 
	for(size_t idx_Ctr = 0; idx_Ctr < contourTmp.size(); ++idx_Ctr) {
		cv::Moments mm = cv::moments(cv::Mat(contourTmp.at(idx_Ctr))); 
		if(mm.m00>10){ //remove contour with area too small
			//------------------Find dice---------------------------
			cv::Point2f boxPoint[4];
			cv::RotatedRect box = cv::minAreaRect(cv::Mat(contourTmp.at(idx_Ctr)));
			box.points(boxPoint);
			for(unsigned int i = 0; i < 4; i++)
				cv::line(colorimg, boxPoint[i], boxPoint[(i+1)%4], cv::Scalar(0,255,0), 3);
			//------------------Find dots---------------------------
			cv::Mat drawing=cv::Mat(binaryimg.rows,binaryimg.cols,CV_8UC1,cv::Scalar(0));
			drawContours( drawing, contourTmp, idx_Ctr, cv::Scalar(255), -1, 8, hierarchy, 0, Point() );
			cv::Mat singledice;
			binaryimg.copyTo(singledice,drawing); //mask one dice at a time
			vector<Vec3f> circles;
			cv::GaussianBlur(singledice,singledice,cv::Size(31,31),3); //denoise
			HoughCircles( singledice, circles, CV_HOUGH_GRADIENT, 1,  3, 240, 30, 0, 80 ); //find circle shape features
			int count=0;
			for( size_t i = 0; i < circles.size(); i++ )
			{
				Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
				int radius = cvRound(circles[i][2]);
				circle( colorimg, center, 3, Scalar(0,255,0), -1, 8, 0 ); // circle center
				circle( colorimg, center, radius, Scalar(0,0,255), 1, 8, 0 ); // circle outline
			}
			//------------------Put text of dots number---------------------------
			cv::putText(colorimg,to_string(circles.size()),contourTmp[idx_Ctr][0],cv::FONT_HERSHEY_SIMPLEX,1,cv::Scalar(255),1);
		}
	}
	return colorimg;
}