#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <string>

#ifndef WFC_CORE
#define WFC_CORE

class WFC {
	int tileSize;
	bool status;
	std::vector<cv::Mat> tileCache;

	bool preprocess(std::string fileName);
	bool isTileInCache(cv::Mat tile);
	void buildTileCache(cv::Mat src, int tileSize);
	void addTileToCache(cv::Mat tile);

public:
	WFC(std::string fileName, int tileSize);

	void run();
};

std::string matType(cv::Mat mat);

void debugMat(cv::Mat mat);

bool compareMat(cv::Mat mat1, cv::Mat mat2);

void error(std::string errMessage);

void stub(std::string stubMessage);

#endif // !WFC_CORE
