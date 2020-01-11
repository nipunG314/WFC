#include <iostream>
#include <vector>

#include "core.h"

WFC::WFC(std::string fileName, int tileSize) : tileSize(tileSize) {
	if (!preprocess(fileName))
		return;
}

std::vector<cv::Mat> fetchTiles(cv::Mat src, int tileSize) {
	assert(tileSize >= 0);

	std::vector<cv::Mat> result;
	{
		cv::Mat tile;
		cv::Mat concatColMat, concatRowMat;

		for (int j = 0; j < src.rows; j++) {
			for (int i = 0; i < src.cols; i++) {
				if (j <= src.rows - tileSize) {
					if (i <= src.cols - tileSize) {
						tile = cv::Mat(src, cv::Rect(i, j, tileSize, tileSize)).clone();
					} else {
						int tileIncrement = i + tileSize - src.cols;
						tile = cv::Mat(src, cv::Rect(i, j, tileSize - tileIncrement, tileSize)).clone();
						concatColMat = cv::Mat(tileSize, tileIncrement, src.type());

						for (int m = 0; m < tileSize; m++) {
							for (int k = 0; k < tileIncrement; k++) {
								concatColMat.at<uchar>(cv::Point(k, m)) = src.at<uchar>(cv::Point(k, m + j));
							}
						}

						cv::hconcat(tile, concatColMat, tile);
					}
				} else {
					if (i <= src.cols - tileSize) {
						int tileIncrement = j + tileSize - src.rows;
						tile = cv::Mat(src, cv::Rect(i, j, tileSize, tileSize - tileIncrement)).clone();
						concatRowMat = cv::Mat(tileIncrement, tileSize, src.type());

						for (int m = 0; m < tileIncrement; m++) {
							for (int k = 0; k < tileSize; k++) {
								concatRowMat.at<uchar>(cv::Point(k, m)) = src.at<uchar>(cv::Point(k + i, m));
							}
						}

						tile.push_back(concatRowMat);
					} else {
						int tileIncrementX = i + tileSize - src.cols;
						int tileIncrementY = j + tileSize - src.rows;
						tile = cv::Mat(src, cv::Rect(i, j, tileSize - tileIncrementX, tileSize - tileIncrementY)).clone();
						concatColMat = cv::Mat(tileSize - tileIncrementY, tileIncrementX, src.type());
						concatRowMat = cv::Mat(tileIncrementY, tileSize - tileIncrementX, src.type());

						for (int m = 0; m < tileSize - tileIncrementY; m++) {
							for (int k = 0; k < tileIncrementX; k++) {
								concatColMat.at<uchar>(cv::Point(k, m)) = src.at<uchar>(cv::Point(k, m + j));
							}
						}

						for (int m = 0; m < tileIncrementY; m++) {
							for (int k = 0; k < tileSize - tileIncrementX; k++) {
								concatRowMat.at<uchar>(cv::Point(k, m)) = src.at<uchar>(cv::Point(k + i, m));
							}
						}

						cv::Mat concatCornerMat = cv::Mat(src, cv::Rect(0, 0, tileIncrementX, tileIncrementY)).clone();
						cv::hconcat(concatRowMat, concatCornerMat, concatRowMat);

						cv::hconcat(tile, concatColMat, tile);
						tile.push_back(concatRowMat);
					}
				}
				result.push_back(tile);
			}
		}
	}

	// Ensure that all the tiles are
	// unique in memory
	//
	// If untrue, return empty vector

	for (uint j = 0; j < result.size(); j++) {
		if (result[j].u->refcount != 1)
			return std::vector<cv::Mat>();
	}

	return result;
}

std::vector<cv::Mat> computeRotationsReflections(cv::Mat src) {
	std::vector<cv::Mat> result;
	{
		cv::Mat dest = cv::Mat(src).clone();
		result.push_back(dest);

		for (uint i = 0; i < 3; i++) {
			dest = cv::Mat(dest).clone();
			cv::rotate(dest, dest, cv::ROTATE_90_CLOCKWISE);
			result.push_back(dest);
		}

		for (uint i = 0; i < 4; i++) {
			dest = cv::Mat(result[i]).clone();
			cv::flip(dest, dest, 0);
			result.push_back(dest);
		}
	}

	// Ensure that all the rotations and reflections
	// unique in memory
	//
	// If untrue, return empty vector

	for (uint j = 0; j < result.size(); j++) {
		if (result[j].u->refcount != 1)
			return std::vector<cv::Mat>();
	}

	return result;
}

bool WFC::preprocess(std::string fileName) {
	cv::Mat source = cv::imread(fileName);

	if (source.empty()) {
		error("Could not open or find the image");
		return false;
	}

	stub("WFC::preprocess: Fetch tiles");
	stub("WFC::preprocess: RotateReflect tiles and add unique tiles to cache");

	return false;
}

void WFC::run() {
	stub("STUB: Implement WFC::run()");
}

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

void debugMat(cv::Mat mat) {
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

void error(std::string errMessage) {
	std::cout << "Error occurred: " << errMessage << "!\n";
}

void stub(std::string stubMessage) {
	std::cout << "STUB: " << stubMessage << ".\n";
}
