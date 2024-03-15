#ifndef TOUR_GENERATOR_H
#define TOUR_GENERATOR_H

#include "base_classes.h"

#include <string>

class TourGenerator: public TourGeneratorBase {
public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
    virtual ~TourGenerator();
    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
    const GeoDatabaseBase& m_geoDatabase;
    const RouterBase& m_router;
    std::string getLineDirection(double angle) const; // get direction of the line based on angle
    std::string getTurnDirection(double angle) const; // get direction of the turn based on angle
};

#endif
