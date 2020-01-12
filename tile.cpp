#include <iostream>

#include "tile.h"

Tile Tile::clone() {
	return Tile(index + 1, freq, data.clone());
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
bool Tile::cmp(Tile tile) {
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
