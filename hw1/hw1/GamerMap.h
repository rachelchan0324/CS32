#ifndef GAMER_MAP_H
#define GAMER_MAP_H

#include "Map.h"

class GamerMap {
    public:
        GamerMap();
        bool addGamer(std::string name);
        double hoursSpent(std::string name) const;
        bool play(std::string name, double hours);
        int numGamers() const;
        void print() const;

    private:
        int m_size;
        Map m_gamerMap;
};

#endif

