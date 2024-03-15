#ifndef GEO_DATABASE_H
#define GEO_DATABASE_H

#include "base_classes.h"
#include "hashmap.h"
#include <vector>

class GeoPoint;

class GeoDatabase: public GeoDatabaseBase {
public:
    GeoDatabase();
    virtual ~GeoDatabase();
    virtual bool load(const std::string& map_data_file);
    virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
    virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
    virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
    // for get_street_name
    // key: the 2 GeoPoints that define the street, value: street name
    HashMap<std::string> segmentMap;
    // for get_poi_location
    // key: POI name, value: GeoPoint
    HashMap<GeoPoint> poiMap;
    // for get_connected_points
    // key: GeoPoint, value: vector of GeoPoints that it's connected to
    HashMap<std::vector<GeoPoint>> endpointMap;
};

#endif
