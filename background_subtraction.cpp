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

using namespace cv;
using namespace std;


// Global variables
Mat frame; //current frame
Mat frame_tmp;
Mat fgMaskMOG; //fg mask generated by MOG method
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Mat bgImg;
Ptr<BackgroundSubtractor> pMOG; //MOG Background subtractor
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor

bool debug = true;
bool debug2 = false;

int keyboard; //input from keyboard
void help();
void processVideo(char* videoFilename);
void processImages(char* firstFrameFilename);
void processImagesDir(string dir, string ext, string txtDir, string dstDir);
void processList(string img_list_path, string random_bg_list_path, string base_bg_dir, string write_path);
void processList2(string img_list_path, string base_bg_dir, string write_path);
void processList3(string img_list_path, string random_bg_list_path, string write_path);

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
	// Create Background Subtractor Instance
	pMOG2 = createBackgroundSubtractorMOG2(1, 16, true); //MOG2 approach

	// Set paths 
	string random_bg_list_path = "/data1/kimth/random_pic.txt";
	string img_list_path = "/data1/kimth/hand_formal.txt";
	string write_path = "/data1/kimth/input/";

	// DEBUG mode
	debug = false;
	debug2 = false;

	// OPTION 1. Randomize background
	//processList(img_list_path, random_bg_list_path, base_bg_dir, write_path);
	
	// OPTION 2. Remove background and make it dark
	//processList2(img_list_path, base_bg_dir, write_path);

	// OPTION 3. Randomize background from black background
	processList3(img_list_path, random_bg_list_path, write_path);

	// destroy GUI windows
	destroyAllWindows();

	return EXIT_SUCCESS;

}

// randomize background
void processList(string img_list_path, string random_bg_list_path, string base_bg_dir, string write_path) {

	if (debug) {
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

		if (debug) cout << "Write to " << write_img << endl << endl;

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

// Randomize background with black background. Set number of random images to use per image
void processList3(string img_list_path, string random_bg_list_path, string write_path) {

	if (debug) {
		cout << "img_list_path : " << img_list_path << endl;
		cout << "random_bg_list_path: " << random_bg_list_path << endl;
		cout << "Writing to : " << write_path << endl << endl;
	}

	ifstream img_list(img_list_path);
	string img_path, random_bg_path;

	int img_width, img_height;
	int cnt = 0;
	int bg_cnt;
	int random_num = 50;

	while (getline(img_list, img_path)) {

		// read image
		Mat frame = imread(img_path);

		// get filename
		size_t idx = img_path.find_last_of("/");
		size_t idx2 = img_path.find_last_of(".");
		string file_path = img_path.substr(0, idx);
		string file_name2 = img_path.substr(0, idx2);
		string file_name_only = file_name2.substr(idx + 1, idx2 - idx);

		// write name
		string write_img = write_path;
		string write_label;
		
		write_img.append("/");
		write_img.append(file_name_only);
		write_img.append("_");

		// count vars
		bg_cnt = 0;
		cnt++;

		// read label
		string read_label = img_path.substr(0, img_path.find_last_of("."));
		read_label.append(".txt");

		ifstream random_bg_list(random_bg_list_path);
		
		if(cnt%100 == 0) cout << cnt << endl;
		// iterate over random images
		while (getline(random_bg_list, random_bg_path) && bg_cnt < random_num) {

			if(cnt%100 == 0 && bg_cnt % 10 == 9) cout << "***" << bg_cnt+1 << "/" << random_num << endl;
			// Black image of size 640x480
			Mat black_bg(480, 640, CV_8UC3, Scalar(0, 0, 0));
			Mat dummy(480, 640, CV_8UC3, Scalar(0, 0, 0));

			if (debug) {
				cout << "img path : " << img_path << endl;
				cout << "random_bg path : " << random_bg_path << endl << endl;
			}
		
			pMOG2->apply(black_bg, dummy, 1);

			Mat runtime_bg;
			pMOG2->apply(frame, fgMaskMOG2, 0);
			pMOG2->getBackgroundImage(runtime_bg);
		
			Mat mask;
			fgMaskMOG2.copyTo(mask);
			Mat mask_inv;
			bitwise_not(fgMaskMOG2, mask_inv);
			Mat randImg = imread(random_bg_path);
			
			Mat newWholeImg;

			// copy masked area from frame, unmasked area from new random image
			if(debug){
				cout << frame.size() << " " << randImg.size() << " " << newWholeImg.size() << endl;
			}

			frame.copyTo(newWholeImg, mask);
			randImg.copyTo(newWholeImg, mask_inv);

			// write image
			string tmp_img = write_img;
			tmp_img.append(to_string(bg_cnt));
			tmp_img.append(".jpg");
			imwrite(tmp_img, newWholeImg);

			// write label
			write_label = tmp_img.substr(0, tmp_img.find_last_of("."));
			write_label.append(".txt");

			char buf[BUFSIZ];
			size_t size;

			FILE* source = fopen(read_label.c_str(), "rb");
			FILE* dest = fopen(write_label.c_str(), "wb");

			while (size = fread(buf, 1, BUFSIZ, source)) {
				fwrite(buf, 1, size, dest);
			}

			fclose(source);
			fclose(dest);

			bg_cnt++;
		}
	}
}


