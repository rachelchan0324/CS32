#include "tour_generator.h"
#include "geotools.h"
#include "tourcmd.h"

#include <vector>
#include <string>

using namespace std;

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router)
: m_geoDatabase(geodb), m_router(router) {}

TourGenerator::~TourGenerator(){ }

vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const{
    vector <TourCommand> tour;
    
    // for each stop/POI...
    for(int i = 0; i < stops.size(); i++){
        // make a comment about the current POI by inserting it into the command vector
        TourCommand comment;
        string poi, talkingPoints;
        stops.get_poi_data(i, poi, talkingPoints);
        comment.init_commentary(poi, talkingPoints);
        tour.push_back(comment);
        
        // execute the rest of the method only if  there is another POI in the list
        if(i == stops.size() - 1)
            break;
        
        // get the path from the current POI to the next POI
        vector<GeoPoint> path;
        string nextPoi, unused; // we don't need nextPOI's talking points
        stops.get_poi_data(i+1, nextPoi, unused);
        GeoPoint poiPoint, nextPoiPoint;
        m_geoDatabase.get_poi_location(poi, poiPoint);
        m_geoDatabase.get_poi_location(nextPoi, nextPoiPoint);
        path = m_router.route(poiPoint, nextPoiPoint);
        
        // for every adjacent GeoPoint... (iterate n-1 times)
        for(int j = 0; j < path.size() - 1; j++){
            // proceed on the current street
            TourCommand proceed;
            proceed.init_proceed(getLineDirection(angle_of_line(path[j], path[j+1])), m_geoDatabase.get_street_name(path[j], path[j+1]), distance_earth_miles(path[j], path[j+1]), path[j], path[j+1]);
            tour.push_back(proceed);
            
            // if there is a GeoPoint p3 directly after p2 AND its a different street AND its a different direction, then insert a turn command
            if(j <= path.size() - 3 && (m_geoDatabase.get_street_name(path[j], path[j+1]) != m_geoDatabase.get_street_name(path[j+1], path[j+2])) && angle_of_turn(path[j], path[j+1], path[j+2]) != 0){
                TourCommand turn;
                turn.init_turn(getTurnDirection(angle_of_turn(path[j], path[j+1], path[j+2])), m_geoDatabase.get_street_name(path[j+1], path[j+2]));
                tour.push_back(turn);
            }
        }
    }
    return tour; // return vector of tour commands
}

// get direction of the line based on angle
string TourGenerator::getLineDirection(double angle) const{
    if(0 <= angle && angle < 22.5)
        return "east";
    else if(22.5 <= angle && angle < 67.5)
        return "northeast";
    else if (67.5 <= angle && angle < 112.5)
        return "north";
    else if (112.5 <= angle && angle < 157.5)
        return "northwest";
    else if (157.5 <= angle && angle < 202.5)
        return "west";
    else if (202.5 <= angle && angle < 247.5)
        return "southwest";
    else if (247.5 <= angle && angle < 292.5)
        return "south";
    else if(292.5 <= angle && angle < 337.5)
        return "southeast";
    else
        return "east";
}

// get direction of the turn based on angle
string TourGenerator::getTurnDirection(double angle) const{
    if(angle >= 1 && angle < 180)
        return "left";
    return "right";
}
