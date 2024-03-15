#include "router.h"
#include "geopoint.h"
#include "geotools.h"
#include "hashmap.h"

#include <queue>
#include <vector>

using namespace std;

Router::Router(const GeoDatabaseBase& geo_db) 
: m_geoDatabase(geo_db) { }

Router::~Router() { }

vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const{
    // maps current point to previous point to track the route
    HashMap <GeoPoint> prevPoint;
    // queue for BFS
    queue<GeoPoint> toVisit;
    // path from one stop to the next
    vector<GeoPoint> path;
    
    toVisit.push(pt1);
    GeoPoint curr = toVisit.front();
    bool foundEndpoint = false;
    
    while(!toVisit.empty()) {
        curr = toVisit.front();
        toVisit.pop();
        // for each of current's connected points, push it onto the queue so we can traverse later
        for(GeoPoint nextPoint : m_geoDatabase.get_connected_points(curr)) {
            // checks if the point has been visited before; if it has, find will NOT return nullptr
            if(prevPoint.find(nextPoint.to_string()) == nullptr){
                prevPoint.insert(nextPoint.to_string(), curr);
                if(nextPoint.to_string() == pt2.to_string()){
                    // reached the destination, break
                    foundEndpoint = true;
                    break;
                }
                toVisit.push(nextPoint);
            }
        }
    }

    // never found the endpoint, return an empty vector
    if(!foundEndpoint)
        return path;
    
    // use the prevPoint hash map to compose the route (backwards)
    curr = pt2;
    while(true){
        path.insert(path.begin(), curr);
        if(curr.to_string() == pt1.to_string())
            break;
        curr = *prevPoint.find(curr.to_string());
    }
    return path;
}
