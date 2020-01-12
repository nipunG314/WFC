#include <opencv2/core.hpp>

#ifndef WFC_TILE
#define WFC_TILE

enum Dir {
	RIGHT,
	UP,
	LEFT,
	DOWN,

	END
};

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

	Tile clone();
	bool matchRefCount(int refCount);
	void flipData(int flipCode);
	void rotateData(int rotateCode);

	Tile crop(Dir dir);
	bool compatible(Tile tile, Dir dir);

	std::string matType();
	void debug();
	bool operator==(Tile tile);
};

#endif // !WFC_TILE
