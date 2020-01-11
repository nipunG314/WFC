#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <string>

#ifndef WFC_CORE
#define WFC_CORE

class WFC {
	uint tileSize;
	std::vector<cv::Mat> tileCache;

	bool preprocess(std::string fileName);

public:
	WFC(std::string fileName);

	void run();
};

std::string matType(cv::Mat mat);

void debugMat(cv::Mat mat);

bool compareMat(cv::Mat mat1, cv::Mat mat2);

void error(std::string errMessage);

void stub(std::string stubMessage);

#endif // !WFC_CORE
