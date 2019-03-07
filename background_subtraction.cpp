#pragma once
//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/video/background_segm.hpp>

//C
#include <stdio.h>

//C++
#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <fstream>

//Windows
#include <Windows.h>

using namespace cv;
using namespace std;


// Global variables
Mat frame; //current frame
Mat fgMaskMOG; //fg mask generated by MOG method
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Mat bgImg;
Ptr<BackgroundSubtractor> pMOG; //MOG Background subtractor
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
bool debug = true;

int keyboard; //input from keyboard
void help();
void processVideo(char* videoFilename);
void processImages(char* firstFrameFilename);
void processImagesDir(string dir, string ext, string txtDir, string dstDir);
void processList(string img_list_path, string random_bg_list_path, string base_bg_dir, string write_path);
void processList2(string img_list_path, string base_bg_dir, string write_path);

void help()
{
	cout 
		<< "--------------------------------------------------------------------------" << endl
		<< "This program shows how to use background subtraction methods provided by " << endl
		<< " OpenCV. You can process both videos (-vid) and images (-img)." << endl
		<< endl
		<< "Usage:" << endl
		<< "./bs {-vid <video filename>|-img <image filename>|-dir <dir_path> <repeatedStirng.extention>  }" << endl
		<< "for example: ./bs -vid video.avi" << endl
		<< "or: ./bs -img /data/images/1.png" << endl
		<< "or: ./bs -dir D:/Document/ *.jpg" << endl
		<< "or: ./bs -dir D:/Document/ 173\\*.jpg" << endl
		<< "--------------------------------------------------------------------------" << endl
		<< endl;
}

int main(int argc, char* argv[])

{
/*
	//////////////////////////////////////////////////////
	//  PART 1. GENERATE BACKGROUND FROM NON-BASE IMAGES

	// Set Base and Source. Result will be Source (cropped image) + Base

	//Mat bg_base = imread("D:/32_TunaCan_raw/TunaCan_1_1829.jpg");
	//Mat bg_source = imread("D:/32_TunaCan_raw/TunaCan_1_2586.jpg");


	//// Crop

	int width = bg_source.cols;
	int height = bg_source.rows;
	Rect bg_source_roi = Rect(Point(0, height*0.6), Point(width / 5, height - 1));


	//// Show middle result

	imshow("bg_base", bg_base);
	imshow("bg_source", bg_source);

	cout << "rect : " << bg_source_roi << endl;
	Mat bg_source_crop = bg_source(bg_source_roi);

	Mat bg;
	bg_base.copyTo(bg);
	bg_source_crop.copyTo(bg(bg_source_roi));

	imshow("bg_base", bg_base);
	imshow("bg_source", bg_source);
	imshow("bg_source_crop", bg_source_crop);
	imshow("result", bg);
	imwrite("D:\\background_sub\\backgrounds\\Vita500_add2_4_bg.jpg", bg);

	waitKey();

	//////////////////////////////////////////////////////
*/
	
	//////////////////////////////////////////////////////
	//  PART 2. SUBTRACT BACKGROUND USING BASE BACKGROUND INFO

	// Create GUI windows
	//namedWindow("Frame");
	//namedWindow("bgImg");
	//namedWindow("FG Mask MOG 2");
	//namedWindow("frameBG");

	// Create Background Subtractor Instance
	pMOG2 = createBackgroundSubtractorMOG2(1, 16, true); //MOG2 approach
	
	//// SAMPLE : Read Background and target image
	//Mat bg = imread("D:\\background_sub\\backgrounds\\Vita500_0_bg.jpg");
	//Mat target = imread("D:\\background_sub\\Vita500_0_\\Vita500_0_32.jpg");
	//pMOG2->apply(bg, target, 0);

	//// SAMPLE : Show Result
	//imshow("bg", bg);
	//imshow("target", target);
	//keyboard = waitKey(0);
	
	// FIX filename
	string filename = "soohong";

	// taehyun 112
	// byeonggi 810
	// hyunggi 1375
	// hyunsung 846
	// soohong 708

	// Set paths 
	string random_bg_list_path = "C:/Users/Taehyun Kim/Desktop/Project/3.Background_Subtraction/random_pics.txt";

	string img_list_path = "C:/Users/Taehyun Kim/Desktop/Project/1.Split_Video/";
	img_list_path.append(filename);
	img_list_path.append("/target.txt");

	string base_bg_dir = "C:/Users/Taehyun Kim/Desktop/Project/1.Split_Video/";
	base_bg_dir.append(filename);
	base_bg_dir.append("/");
	base_bg_dir.append(filename);
	// FIX base background image index
	base_bg_dir.append("_708.jpg");

	string write_path = "C:/Users/Taehyun Kim/Desktop/Project/3.Background_Subtraction/Result/";
	write_path.append(filename);
	write_path.append("/");

	// DEBUG mode
	debug = false;

	// OPTION 1. Randomize background
	//processList(img_list_path, random_bg_list_path, base_bg_dir, write_path);

	// OPTION 2. Make background dark
	processList2(img_list_path, base_bg_dir, write_path);
	
	// destroy GUI windows
	destroyAllWindows();

	return EXIT_SUCCESS;

}

// randomize background
void processList(string img_list_path, string random_bg_list_path, string base_bg_dir, string write_path) {
	
	if(debug){
		cout << "img_list_path : " << img_list_path << endl;
		cout << "random_bg_list_path: " << random_bg_list_path << endl;
		cout << "img_list_path : " << base_bg_dir << endl << endl;
	}

	ifstream img_list(img_list_path);
	ifstream random_bg_list(random_bg_list_path);
	string img_path, random_bg_path;
	int cnt = 0;

	while (getline(img_list, img_path) && getline(random_bg_list, random_bg_path)) {

		cnt++;

		if (debug) {
			cout << "img path : " << img_path << endl;
			cout << "random_bg path : " << random_bg_path << endl << endl;
		}

		frame = imread(img_path);
		//imshow("Frame", frame);
		//keyboard = waitKey(0);

		if (frame.empty()) {
			//error in opening the first image
			cerr << "Unable to open first image frame: " << img_path << endl;
			exit(EXIT_FAILURE);
		}

		size_t index = img_path.find_last_of("/");
		if (index == string::npos) {
			index = img_path.find_last_of("\\");
		}

		string file_name = img_path.substr(index + 1);
		size_t index2 = img_path.find_last_of(".");
		string prefix = img_path.substr(0, index + 1);
		string suffix = img_path.substr(index2);

		Mat bg_base = imread(base_bg_dir);
		Mat dummy;

		//imshow("BG base", bg_base);
		//keyboard = waitKey(0);
		
		pMOG2->apply(bg_base, dummy, 1);

		Mat runtime_bg;
		pMOG2->apply(frame, fgMaskMOG2, 0);
		pMOG2->getBackgroundImage(runtime_bg);

		//imshow("runtime bg", runtime_bg);
		//keyboard = waitKey(0);

		Mat mask;
		fgMaskMOG2.copyTo(mask);
		Mat mask_inv;
		bitwise_not(fgMaskMOG2, mask_inv);
		Mat randImg = imread(random_bg_path);

		//imshow("randImg", randImg);
		//keyboard = waitKey(0);

		Mat newWholeImg;

		frame.copyTo(newWholeImg, mask);
		randImg.copyTo(newWholeImg, mask_inv);

		/*imshow("bg_base", bg_base);
		imshow("Frame", frame);
		imshow("FG Mask MOG 2", fgMaskMOG2);
		imshow("random bg", randImg);
		imshow("newWholeImg", newWholeImg);*/
		
		size_t idx = img_path.find_last_of("/");
		size_t idx2 = img_path.find_last_of(".");
		string file_path = img_path.substr(0, idx);
		string file_name2 = img_path.substr(0, idx2);
		string file_name_only = file_name2.substr(idx + 1, idx2 - idx);

		// write name
		string write_img = write_path;
		write_img.append("/");
		write_img.append(file_name_only);
		write_img.append("_BS.jpg");
		imwrite(write_img, newWholeImg);
		
		if(debug) cout << "Write to " << write_img << endl << endl;

		if (cnt % 20 == 0) {
			cout << "checkpoint : " << cnt << " complete ..." << endl;
		}
	}

}

// make background black
void processList2(string img_list_path, string base_bg_dir, string write_path){

	if (debug) {
		cout << "img_list_path : " << img_list_path << endl;
		cout << "img_list_path : " << base_bg_dir << endl << endl;
	}

	ifstream img_list(img_list_path);
	string img_path;
	int cnt = 0;

	while (getline(img_list, img_path)) {

		cnt++;

		if (debug) {
			cout << "img path : " << img_path << endl;
		}

		frame = imread(img_path);

		if (frame.empty()) {
			//error in opening the first image
			cerr << "Unable to open first image frame: " << img_path << endl;
			exit(EXIT_FAILURE);
		}

		size_t index = img_path.find_last_of("/");
		if (index == string::npos) {
			index = img_path.find_last_of("\\");
		}

		string file_name = img_path.substr(index + 1);
		size_t index2 = img_path.find_last_of(".");
		string prefix = img_path.substr(0, index + 1);
		string suffix = img_path.substr(index2);

		Mat bg_base = imread(base_bg_dir);
		Mat dummy;

		//imshow("BG base", bg_base);
		//keyboard = waitKey(0);

		pMOG2->apply(bg_base, dummy, 1);

		Mat runtime_bg;
		pMOG2->apply(frame, fgMaskMOG2, 0);
		pMOG2->getBackgroundImage(runtime_bg);

		//imshow("runtime bg", runtime_bg);
		//keyboard = waitKey(0);

		Mat mask;
		fgMaskMOG2.copyTo(mask);
		Mat mask_inv;
		bitwise_not(fgMaskMOG2, mask_inv);
		//Mat randImg = imread(random_bg_path);

		//imshow("randImg", randImg);
		//keyboard = waitKey(0);

		Mat newWholeImg;

		frame.copyTo(newWholeImg, mask);
		//randImg.copyTo(newWholeImg, mask_inv);

		/*imshow("bg_base", bg_base);
		imshow("Frame", frame);
		imshow("FG Mask MOG 2", fgMaskMOG2);
		imshow("random bg", randImg);
		*/
		if (debug) {
			imshow("newWholeImg", newWholeImg);
			keyboard = waitKey(0);
		}

		size_t idx = img_path.find_last_of("/");
		size_t idx2 = img_path.find_last_of(".");
		string file_path = img_path.substr(0, idx);
		string file_name2 = img_path.substr(0, idx2);
		string file_name_only = file_name2.substr(idx + 1, idx2 - idx);

		// write name
		string write_img = write_path;
		write_img.append("/");
		write_img.append(file_name_only);
		write_img.append("_BS.jpg");
		imwrite(write_img, newWholeImg);

		if (debug) cout << "Write to " << write_img << endl << endl;

		if (cnt % 20 == 0) {
			cout << "checkpoint : " << cnt << " complete ..." << endl;
		}
	}

}

