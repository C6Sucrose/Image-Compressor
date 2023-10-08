#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>


using namespace cv;
using namespace std;

int main() {
	
	string img_1;
	string img_2;
	cout << "MAKE SURE YOU ENTER THE CORRECT IMAGE NAME" << endl;
	cout << "Image_1 name : ";
	cin >> img_1;
	cout << endl;
	cout << "Image_2 name : ";
	cin >> img_2;
	cout << endl;
	

	Mat og_img = imread(img_1, IMREAD_GRAYSCALE);//original image 

	int** og_DMA = new int*[og_img.rows];

	for (int i = 0; i < og_img.rows; i++) {
		og_DMA[i] = new int[og_img.cols];
	}

	for (int i = 0; i < og_img.rows; i++) {
		for (int j = 0; j < og_img.cols; j++) {
			og_DMA[i][j] = og_img.at<uchar>(i, j);
		}
	}

	Mat new_img = imread(img_2, IMREAD_GRAYSCALE);//new image

	int** new_DMA = new int* [new_img.rows];

	for (int i = 0; i < new_img.rows; i++) {
		new_DMA[i] = new int[new_img.cols];
	}

	for (int i = 0; i < new_img.rows; i++) {
		for (int j = 0; j < new_img.cols; j++) {
			new_DMA[i][j] = new_img.at<uchar>(i, j);
		}
	}

	double n = og_img.rows * og_img.cols;

	double dice_coefficient = 0;

	for (int i = 0; i < og_img.rows; i++) {//Formula for X
		for (int j = 0; j < og_img.cols; j++) {
			dice_coefficient = dice_coefficient + pow((og_DMA[i][j] - new_DMA[i][j]), 2);
		}
	}
	dice_coefficient = dice_coefficient / n;

	dice_coefficient = 100 * (1 - (dice_coefficient / pow(255, 2)));//Formula for Y

	cout << "Similarity between " << img_1 << " and " << img_2 << endl;
	cout << "Dice Coefficient : " << dice_coefficient << endl;

	return 0;
}