#include <iostream>

#include "tile.h"

Tile Tile::clone() {
	return Tile(index, freq, data.clone());
}

bool Tile::matchRefCount(int refCount) {
	return data.u->refcount == refCount;
}

void Tile::flipData(int flipCode) {
	cv::flip(data, data, flipCode);
}

void Tile::rotateData(int rotateCode) {
	cv::rotate(data, data, rotateCode);
}

Tile Tile::crop(Dir dir) {
	Tile t = clone();
	switch (dir) {
		case RIGHT:
			t.data = cv::Mat(t.data, cv::Rect(0, 0, t.data.cols - 1, t.data.rows));
			break;
		case UP:
			t.data = cv::Mat(t.data, cv::Rect(0, 1, t.data.cols, t.data.rows - 1));
			break;
		case LEFT:
			t.data = cv::Mat(t.data, cv::Rect(1, 0, t.data.cols - 1, t.data.rows));
			break;
		case DOWN:
			t.data = cv::Mat(t.data, cv::Rect(0, 0, t.data.cols, t.data.rows - 1));
			break;
		default:
			break;
	}
	return t;
}

bool Tile::compatible(Tile tile, Dir dir) {
	Dir crop1 = (Dir)((dir + 2) % 4);
	Dir crop2 = dir;

	return crop(crop1) == tile.crop(crop2);
}

std::string Tile::matType() {
	std::string r;

	switch (data.depth()) {
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
	r += (data.channels() + '0');

	return r;
}

void Tile::debug() {
	std::string debugInfo = matType();
	std::cout << "Matrix: " << debugInfo << ", Rows: " << data.rows << ", Cols: " << data.cols << "\n";
}

/*
	This function assumes that both the tiles have
	the same dimensions.

	This comparator is intended for tiles that
	are about 2-5 pixels wide and high. It may not
	be efficient for larger matrices.

*/
bool Tile::operator==(Tile tile) {
	for (int r = 0; r < data.rows; r++) {
		cv::Vec3b *ptr1 = data.ptr<cv::Vec3b>(r);
		cv::Vec3b *ptr2 = tile.data.ptr<cv::Vec3b>(r);

		for (int c = 0; c < data.cols; c++) {
			if (ptr1[c] != ptr2[c])
				return false;
		}
	}

	return true;
}
