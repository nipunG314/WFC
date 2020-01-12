#include <vector>
#include <string>

#include "tile.h"

#ifndef WFC_WFC
#define WFC_WFC

struct AdjacencyRule {
	Tile t1, t2;
	Dir dir;

	AdjacencyRule(Tile t1, Tile t2, Dir dir) : t1(t1), t2(t2), dir(dir) {};
};

class WFC {
	int tileSize;
	bool status;
	std::vector<Tile> tileCache;
	std::vector<AdjacencyRule> adjacencyRules;

	bool preprocess(std::string fileName);

	void buildTileCache(cv::Mat src, int tileSize);
	std::vector<Tile> computeRotationsReflections(Tile src);
	int getTile(Tile tile);
	void addTileToCache(cv::Mat tileData);

	void buildAdjacencyRules();

public:
	WFC(std::string fileName, int tileSize);

	void run();
};

#endif // !WFC_WFC
