#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

#include "wfc.h"
#include "util.h"

WFC::WFC(std::string fileName, int tileSize) : tileSize(tileSize) {
	if (!preprocess(fileName)) {
		status = false;
		return;
	}

	status = true;
}

std::vector<Tile> computeRotationsReflections(Tile src) {
	std::vector<Tile> result;
	{
		Tile dest = src.clone();
		result.push_back(dest);

		for (uint i = 0; i < 3; i++) {
			dest = dest.clone();
			dest.rotateData(cv::ROTATE_90_CLOCKWISE);
			result.push_back(dest);
		}

		for (uint i = 0; i < 4; i++) {
			dest = result[i].clone();
			dest.flipData(0);
			result.push_back(dest);
		}
	}

	// Ensure that all the rotations and reflections
	// unique in memory
	//
	// If untrue, return empty vector

	for (uint j = 0; j < result.size(); j++) {
		if (!result[j].matchRefCount(1))
			return std::vector<Tile>();
	}

	return result;
}

bool WFC::preprocess(std::string fileName) {
	cv::Mat source = cv::imread(fileName);

	if (source.empty()) {
		error("Could not open or find the image");
		return false;
	}

	buildTileCache(source, tileSize);

	return true;
}

int WFC::getTile(Tile tile) {
	for (auto storedTile : tileCache) {
		if (tile.cmp(storedTile))
			return tile.getIndex();
	}
	return -1;
}

void WFC::buildTileCache(cv::Mat src, int tileSize) {
	assert(tileSize >= 0);

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
				addTileToCache(tile);
			}
		}
	}

	// Ensure that all the tiles are
	// unique in memory
	//
	// If untrue, return empty vector

	for (uint j = 0; j < tileCache.size(); j++) {
		if (!tileCache[j].matchRefCount(1)) {
			error("Tiles in TileCache with multiple references");
			return;
		}
	}
}

void WFC::addTileToCache(cv::Mat tileData) {
	Tile tile((int)tileCache.size(), 1, tileData);

	int index = getTile(tile);
	if (index == -1) {
		auto tiles = computeRotationsReflections(tile);
		if (tiles.empty()) {
			error("Tiles in the result from computeRotationsReflections() with multiple references");
			return;
		}

		tileCache.reserve(tileCache.size() + tiles.size());
		tileCache.insert(tileCache.end(), tiles.begin(), tiles.end());
	} else {
		index = (index / 8) * 8;
		for (int i = index; i < index + 8; i++)
			tileCache[i].setFreq(tileCache[i].getFreq() + 1);
	}
}

void WFC::run() {
	stub("STUB: Implement WFC::run()");
}
