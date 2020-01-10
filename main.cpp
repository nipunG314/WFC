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

int main() {
	cv::Mat m2, m3, m4;
	{
		int arr[3][4] = {
			{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}
		};

		cv::Mat m1(3, 4, CV_32SC1);
		std::memcpy(m1.data, arr, 3 * 4 * sizeof(int));

		m2 = cv::Mat(m1, cv::Rect(0, 0, 1, 1)).clone();
		m3 = cv::Mat(m1, cv::Rect(1, 1, 1, 1)).clone();
		m4 = cv::Mat(m1, cv::Rect(2, 2, 1, 1)).clone();
	}

	std::cout << m2 << "\n";
	std::cout << m3 << "\n";
	std::cout << m4 << "\n";
	debugMatrix(m4);

	int RefCount = m2.u ? (m2.u->refcount) : 0;
	std::cout << RefCount << "\n";
	RefCount = m3.u ? (m3.u->refcount) : 0;
	std::cout << RefCount << "\n";
	RefCount = m4.u ? (m4.u->refcount) : 0;
	std::cout << RefCount << "\n";

	return 0;
}
