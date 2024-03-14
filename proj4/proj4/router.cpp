#include "Router.h"
#include "geopoint.h"
#include "HashMap.h"

#include <queue>
#include <vector>
#include <string>

using namespace std;

Router::Router(const GeoDatabaseBase& geo_db) 
: m_geoDatabase(geo_db) { }

Router::~Router() { }

vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const{
    HashMap <GeoPoint> prevPoint;
    queue<GeoPoint> toVisit;
    vector<GeoPoint> path;
    
    toVisit.push(pt1);
    
    GeoPoint curr = toVisit.front();
    while(!toVisit.empty()) {
        GeoPoint curr = toVisit.front();
        toVisit.pop();
        
        for(GeoPoint nextPoint : m_geoDatabase.get_connected_points(curr)) {
            if(prevPoint.find(nextPoint.to_string()) == nullptr){
                prevPoint.insert(nextPoint.to_string(), curr);
                if(nextPoint.to_string() == pt2.to_string()){
                    break;
                }
                toVisit.push(nextPoint);
            }
        }
    }
    
    curr = pt2;
    while(true){
        path.insert(path.begin(), curr);
        if(curr.to_string() == pt1.to_string())
            break;
        curr = *prevPoint.find(curr.to_string());
    }
    return path;
}
