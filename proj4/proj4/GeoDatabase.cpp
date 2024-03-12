#include "GeoDatabase.h"
#include "HashMap.h"
#include "geopoint.h"
#include "geotools.h"

#include <iostream>
#include <fstream>

using namespace std;

GeoDatabase::GeoDatabase(){ }

GeoDatabase::~GeoDatabase(){ }

bool GeoDatabase::load(const std::string& map_data_file){
    ifstream infile(map_data_file);
    if (!infile)
        return false;
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
        
        segmentMap.insert(start.to_string() + " " + end.to_string(), street);
        segmentMap.insert(end.to_string() + " " + start.to_string(), street);
        
        endpointMap[start.to_string()].push_back(end);
        endpointMap[end.to_string()].push_back(start);
        
        infile >> numPoi;
        infile.ignore(10000, '\n');
        GeoPoint mp;
        if(numPoi >= 1){
            mp = midpoint(start, end);
            
            segmentMap.insert(start.to_string() + " " + mp.to_string(), street);
            segmentMap.insert(mp.to_string() + " " + start.to_string(), street);
            segmentMap.insert(end.to_string() + " " + mp.to_string(), street);
            segmentMap.insert(mp.to_string() + " " + end.to_string(), street);
            
            endpointMap[start.to_string()].push_back(mp);
            endpointMap[mp.to_string()].push_back(start);
            endpointMap[end.to_string()].push_back(mp);
            endpointMap[mp.to_string()].push_back(end);
        }
        for(int i = 0; i < numPoi; i++){
            string poiName, poiLat, poiLong;
        
            while(true){
                infile.get(c);
                if (c == '|')
                    break;
                poiName += c;
            }
            
            infile >> poiLat;
            infile.get(c); // space
            getline(infile, poiLong); // consumes new line as well
            GeoPoint poiPoint(poiLat, poiLong);
            poiMap.insert(poiName, poiPoint);
            
            segmentMap.insert(poiPoint.to_string() + " " +  mp.to_string(), "a path");
            segmentMap.insert(mp.to_string() + " " + poiPoint.to_string(), "a path");
            
            endpointMap[mp.to_string()].push_back(poiPoint);
            endpointMap[poiPoint.to_string()].push_back(mp);
        }
    }
    return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi,GeoPoint& point) const{
    const GeoPoint* ptr = poiMap.find(poi);
    if(ptr == nullptr)
        return false;
    point = *ptr;
    return true;
}

string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const{
    const string* ptr = segmentMap.find(pt1.to_string() + " " + pt2.to_string());
    if(ptr == nullptr)
        return "";
    return *ptr;
}
std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
    return *endpointMap.find(pt.to_string());
}
