#include "GeoDatabase.h"
#include "HashMap.h"
#include "geopoint.h"

#include <iostream>
#include <fstream>

using namespace std;

GeoDatabase::GeoDatabase(){
}

GeoDatabase::~GeoDatabase(){
}

bool GeoDatabase::load(const std::string& map_data_file){
    ifstream infile("mapdata.txt");
    if (!infile) {
        cerr << "Error: Cannot open data.txt!" << endl;
    }
    
}
