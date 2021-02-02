#include "LevelsXML.h"

void LevelsXML::BrickInfoFill()
{
	if (string(e1->Attribute("Id")) == "I") {
		bricksInfo[make_pair(e1->Attribute("Id"), "Texture")] = e1->Attribute("Texture");
		bricksInfo[make_pair(e1->Attribute("Id"), "HitSound")] = e1->Attribute("HitSound");

	}
	else {
		bricksInfo[make_pair(e1->Attribute("Id"), "Texture")] = e1->Attribute("Texture");
		bricksInfo[make_pair(e1->Attribute("Id"), "HitPoints")] = e1->Attribute("HitPoints");
		bricksInfo[make_pair(e1->Attribute("Id"), "HitSound")] = e1->Attribute("HitSound");
		bricksInfo[make_pair(e1->Attribute("Id"), "BreakSound")] = e1->Attribute("BreakSound");
		bricksInfo[make_pair(e1->Attribute("Id"), "BreakScore")] = e1->Attribute("BreakScore");
	}
}

LevelsXML::LevelsXML(int x) {

		string a = "XMLfiles/Level"+ to_string(x)+".xml";
		doc.LoadFile(a.c_str());

	
		if (doc.Error()) cout << "Error: " << doc.ErrorStr() << endl;
		else{ 
			e1 = doc.FirstChildElement("Level")->FirstChildElement("BrickTypes")->FirstChildElement("BrickType");
			BrickInfoFill();
			while (e1->NextSiblingElement("BrickType") != NULL) {
				e1 = e1->NextSiblingElement("BrickType");
				BrickInfoFill();
			}

			rowCount = stoi(doc.FirstChildElement("Level")->Attribute("RowCount"));
			columnCount = stoi(doc.FirstChildElement("Level")->Attribute("ColumnCount"));
			rowSpacing = stoi(doc.FirstChildElement("Level")->Attribute("RowSpacing"));
			columnSpacing = stoi(doc.FirstChildElement("Level")->Attribute("ColumnSpacing"));
			backgroundTexture = doc.FirstChildElement("Level")->Attribute("BackgroundTexture");

			bricksLayout = doc.FirstChildElement("Level")->FirstChildElement("BrickTypes")->NextSiblingElement("Bricks")->GetText();


		}
	
}







