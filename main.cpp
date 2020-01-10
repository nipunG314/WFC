#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

std::string matType(cv::Mat mat) {
	std::string r;

	switch (mat.depth()) {
		case CV_8U:  r = "8U"; break;
		case CV_8S:  r = "8S"; break;
		case CV_16U: r = "16U"; break;
		case CV_16S: r = "16S"; break;
		case CV_32S: r = "32S"; break;
		case CV_32F: r = "32F"; break;
		case CV_64F: r = "64F"; break;
		default:     r = "User"; break;
	}

	r += "C";
	r += (mat.channels() + '0');

	return r;
}

void debugMatrix(cv::Mat mat) {
	std::string debugInfo = matType(mat);
	std::cout << "Matrix: " << debugInfo << ", Rows: " << mat.rows << ", Cols: " << mat.cols << "\n";
}

/*
	This function assumes that mat1 and mat2 have
	the same dimensions.

	This comparator is intended for matrices that
	are about 2-5 pixels wide and high. It may not
	be efficient for larger matrices.

*/
bool compareMat(cv::Mat mat1, cv::Mat mat2) {
	for (int r = 0; r < mat1.rows; r++) {
		cv::Vec3b *ptr1 = mat1.ptr<cv::Vec3b>(r);
		cv::Vec3b *ptr2 = mat2.ptr<cv::Vec3b>(r);

		for (int c = 0; c < mat1.cols; c++) {
			if (ptr1[c] != ptr2[c])
				return false;
		}
	}

	return true;
}

int main() {
	cv::Mat image = cv::imread("image.png", cv::IMREAD_COLOR); // Read the file
	if (image.empty()) {
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	cv::Mat m1(image, cv::Rect(0, 0, 10, 10));
	cv::Mat m2 = cv::Mat(image, cv::Rect(1, 3, 10, 10)).clone();

	std::cout << compareMat(m1, m2) << "\n";

	return 0;
}
