#include "GeoDatabase.h"
#include "HashMap.h"
#include "geopoint.h"
#include "geotools.h"

#include <iostream>
#include <fstream>

using namespace std;

GeoDatabase::GeoDatabase(){
}

GeoDatabase::~GeoDatabase(){
}

bool GeoDatabase::load(const std::string& map_data_file){
    ifstream infile(map_data_file);
    if (!infile ) {
        cerr << "Error: Cannot open map data file!" << endl;
        return false;
    }
    string street;
    while(getline(infile, street)){
        string startLat, startLong, endLat, endLong;
        char c; // to consume white spaces
        int numPoi; // number of POIs
        
        infile >> startLat;
        infile.get(c); // white space
        infile >> startLong;
        GeoPoint start(startLat, startLong);
        
        infile.get(c); // white space
        infile >> endLat;
        infile.get(c); // white space
        getline(infile, endLong); // consumes new line as well
        GeoPoint end(endLat, endLong);
        
        segments.insert(start.to_string() + end.to_string(), street);
        segments.insert(end.to_string() + start.to_string(), street);
        
        endpoints.insert(start.to_string(), end);
        endpoints.insert(end.to_string(), start);
        
        infile >> numPoi;
        infile.ignore(10000, '\n');
        GeoPoint mp;
        if(numPoi >= 1){
            mp = midpoint(start, end);
            
            segments.insert(start.to_string() + mp.to_string(), street);
            segments.insert(mp.to_string() + start.to_string(), street);
            segments.insert(end.to_string() + mp.to_string(), street);
            segments.insert(mp.to_string() + end.to_string(), street);
            
            endpoints.insert(start.to_string(), mp);
            endpoints.insert(mp.to_string(), start);
            endpoints.insert(end.to_string(), mp);
            endpoints.insert(mp.to_string(), end);
        }
        for(int i = 0; i < numPoi; i++){
            string poiName, poiLat, poiLong;
            infile >> poiName;
            infile.get(c); // |
            infile >> poiLat;
            infile.get(c); // space
            getline(infile, poiLong); // consumes new line as well
            GeoPoint poiPoint(poiLat, poiLong);
            poi.insert(poiName, poiPoint);
            
            segments.insert(poiPoint.to_string() + mp.to_string(), "path");
            segments.insert(mp.to_string() + poiPoint.to_string(), "path");
            
            endpoints.insert(mp.to_string(), poiPoint);
            endpoints.insert(poiPoint.to_string(), mp);
        }
    }
    return true;
}
