#include "tour_generator.h"
#include "geotools.h"

#include <vector>
#include <string>

using namespace std;

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router)
: m_geoDatabase(geodb), m_router(router) {}

TourGenerator::~TourGenerator(){ }

vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const{
    
    // MARK: TESTER
    vector <TourCommand> tour;
    ofstream ofile("route.txt");
    
    for(int i = 0; i < stops.size(); i++){
        TourCommand comment;
        string poi, talkingPoints;
        stops.get_poi_data(i, poi, talkingPoints);
        comment.init_commentary(poi, talkingPoints);
        tour.push_back(comment);
        
        if(i == stops.size() - 1)
            break;
        
        vector<GeoPoint> path;
        string nextPoi, unused; // we don't use next POI's talking points
        stops.get_poi_data(i+1, nextPoi, unused);
        GeoPoint poiPoint, nextPoiPoint;
        m_geoDatabase.get_poi_location(poi, poiPoint);
        m_geoDatabase.get_poi_location(nextPoi, nextPoiPoint);
        path = m_router.route(poiPoint, nextPoiPoint);
        
        // every adjacent GeoPoint
        for(int j = 0; j < path.size() - 1; j++){
            TourCommand proceed;
            proceed.init_proceed(getLineDirection(angle_of_line(path[j], path[j+1])), m_geoDatabase.get_street_name(path[j], path[j+1]), distance_earth_miles(path[j], path[j+1]), path[j], path[j+1]);
            tour.push_back(proceed);
            
            // MARK: TESTER
            if(i == 0) {
                ofile << path[j].to_string() << endl;
            }
            ofile << path[j + 1].to_string() << endl;
            
            if(j <= path.size() - 3 && (m_geoDatabase.get_street_name(path[j], path[j+1]) != m_geoDatabase.get_street_name(path[j+1], path[j+2])) && angle_of_turn(path[j], path[j+1], path[j+2]) != 0){
                TourCommand turn;
                turn.init_turn(getTurnDirection(angle_of_turn(path[j], path[j+1], path[j+2])), m_geoDatabase.get_street_name(path[j+1], path[j+2]));
                tour.push_back(turn);
            }
        }
    }
    return tour;
}

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

string TourGenerator::getTurnDirection(double angle) const{
    if(angle >= 1 && angle < 180)
        return "left";
    return "right";
}
