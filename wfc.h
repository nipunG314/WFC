#include <vector>
#include <string>

#include "tile.h"

#ifndef WFC_WFC
#define WFC_WFC

class WFC {
	int tileSize;
	bool status;
	std::vector<Tile> tileCache;

	bool preprocess(std::string fileName);
	int getTile(Tile tile);
	void buildTileCache(cv::Mat src, int tileSize);
	void addTileToCache(cv::Mat tileData);

public:
	WFC(std::string fileName, int tileSize);

	void run();
};

#endif // !WFC_WFC
