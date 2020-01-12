#include <opencv2/core.hpp>

#ifndef WFC_TILE
#define WFC_TILE

class Tile {
	int index;
	int freq;
	cv::Mat data;

public:
	Tile(int index, int freq, cv::Mat data) : index(index), freq(freq), data(data) {};

	int getIndex() { return index; }
	void setIndex(int newIndex) { index = newIndex; }
	int getFreq() { return freq; }
	void setFreq(int newFreq) { freq = newFreq; }
	cv::Mat getData() { return data; }

	Tile clone();

	std::string matType();
	void debug();
	bool cmp(Tile tile);
};

#endif // !WFC_TILE
