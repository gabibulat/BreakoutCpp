#include <iostream>
#include <map>
#include <string>
#include <tinyxml2.h>

using namespace std;
using namespace tinyxml2;

class LevelsXML {

public:
	LevelsXML(int x);

	int rowCount, columnCount, rowSpacing, columnSpacing;
	string bricksLayout, backgroundTexture;

	map<pair<string,string>, string> bricksInfo;

private:
	XMLDocument doc;
	XMLElement* e1;
	void BrickInfoFill();

};


