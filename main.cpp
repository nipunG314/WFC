#include <iostream>

#include "core.h"

int main() {
	cv::Mat image = cv::imread("image.png"); // Read the file
	if (image.empty()) {
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	debugMat(image);

	return 0;
}
