#include "geodb.h"
#include "hashmap.h"
#include "geopoint.h"
#include "geotools.h"

#include <string>

using namespace std;

GeoDatabase::GeoDatabase(){ }

GeoDatabase::~GeoDatabase(){ }

bool GeoDatabase::load(const std::string& map_data_file){
    ifstream infile(map_data_file);
    if (!infile)
        return false; // cannot load file, return false
    string street;
    while(getline(infile, street)){ // while there are still lines to be read in the file...
        string startLat, startLong, endLat, endLong;
        char c; // for consuming white spaces
        int numPoi; // number of POIs
        
        // read and store the startLat, startLong, endLat, endLong
        infile >> startLat;
        infile.get(c);
        infile >> startLong;
        GeoPoint start(startLat, startLong);
        
        infile.get(c);
        infile >> endLat;
        infile.get(c);
        getline(infile, endLong);
        GeoPoint end(endLat, endLong);
        
        // done reading lat/long info, now use the data to populate the hashmaps (bidirectionally)
        
        // for get_street_name (key: the 2 GeoPoints that define the street, value: street name)
        segmentMap.insert(start.to_string() + " " + end.to_string(), street);
        segmentMap.insert(end.to_string() + " " + start.to_string(), street);
        
        // for get_connected_points (key: GeoPoint, value: vector of GeoPoints that it's connected to)
        endpointMap[start.to_string()].push_back(end);
        endpointMap[end.to_string()].push_back(start);
        
        // read in POI data
        infile >> numPoi;
        infile.ignore(10000, '\n');
        GeoPoint mp;
        if(numPoi >= 1){
            // add a midpoint between each street segment that contains a POI
            mp = midpoint(start, end);
            
            // update segment and endpoint maps with this new connection (bidirectionally)
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
        
            // read in data for all the POIs on the street
            while(true){
                infile.get(c);
                if (c == '|')
                    break;
                poiName += c;
            }
            
            infile >> poiLat;
            infile.get(c);
            getline(infile, poiLong);
            GeoPoint poiPoint(poiLat, poiLong);
            
            // insert into POI map
            poiMap.insert(poiName, poiPoint);
            
            // make a connection between the POI and the midpoint of the street (bidirectionally)
            segmentMap.insert(poiPoint.to_string() + " " +  mp.to_string(), "a path");
            segmentMap.insert(mp.to_string() + " " + poiPoint.to_string(), "a path");
            
            endpointMap[mp.to_string()].push_back(poiPoint);
            endpointMap[poiPoint.to_string()].push_back(mp);
        }
    }
    return true; // loaded successfully
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
    if(endpointMap.find(pt.to_string()) == nullptr)
        return vector<GeoPoint>(); // GeoPoint does not exist in our database, return empty vector
    return *endpointMap.find(pt.to_string());
}
