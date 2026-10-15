#include<iostream>
#include<opencv2/opencv.hpp>
#include<fstream>
#include<vector>
using namespace std;
using namespace cv;
int main()
{
	vector<Mat>imgs;
	string imagename;
	ifstream fin("calibdata.txt");
	while (getline(fin, imagename))
	{

		Mat img = imread(imagename);
		imgs.push_back(img);
	}
	Size board_size = Size(10,7);
	vector<vector<Point2f>>imgspoints;
	for (int i = 0;i < imgs.size();i++)
	{
		Mat img1 = imgs[i];
		Mat gray1;
		cvtColor(img1, gray1, COLOR_BGR2GRAY);
		vector<Point2f> img1points;
		findChessboardCorners(gray1, board_size, img1points);
		find4QuadCornerSubpix(gray1, img1points, Size(5, 5));
		bool pattern = true;
		drawChessboardCorners(img1, board_size, img1points, pattern);
		imshow("img1", img1);
		waitKey(0);
		imgspoints.push_back(img1points);
	}
	Size squaresize = Size(16, 16);
	vector<vector<Point3f>> objectpoints;
	for (int k = 0; k < imgspoints.size();k++)
	{
		vector<Point3f>temppoints;
		for (int j = 0; j < board_size.height;j++)
		{
			for (int k = 0; k < board_size.width;k++)
			{
				Point3f realpoint;
				realpoint.x = j * squaresize.width;
				realpoint.y = k * squaresize.height;
				realpoint.z = 0;
				temppoints.push_back(realpoint);
			}
		}
		objectpoints.push_back(temppoints);
	}
	Size imagesize(imgs[0].cols, imgs[0].rows);
	Mat cameramatrix = Mat(3, 3, CV_32FC1, Scalar::all(0));
	Mat distcoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0));
	vector<Mat>rvecs;
	vector<Mat>tvecs;
	calibrateCamera(objectpoints, imgspoints,imagesize, cameramatrix, distcoeffs, rvecs, tvecs,0);
	cout << "内参矩阵" << endl << cameramatrix << endl;
	cout << "畸变系数" << endl << distcoeffs << endl;
	cout << "height" << imgs[0].rows << "width" << imgs[0].cols;

	waitKey(0);
	return 0;

}