#include "GamerMap.h"
#include "Map.h"

#include <iostream>

using namespace std;

GamerMap::GamerMap() {
    // Create an empty gamer map.
    m_size = 0;
}
    
bool GamerMap::addGamer(std::string name) {
    // If a person with the specified name is not currently in the map,
    // and there is room in the map, add an entry for that person
    // recording that they have spent 0 hours gaming, and return true.
    // Otherwise, make no change to the map and return false.
    if(m_gamerMap.insert(name, 0)){
        m_size++;
        return true;
    }
    return false;
}

double GamerMap::hoursSpent(std::string name) const {
    // If a person with the specified name is in the map, return how
    // many hours they have spent gaming; otherwise, return -1.
    double hrsSpent;
    if(m_gamerMap.contains(name)){
        m_gamerMap.get(name, hrsSpent);
        return hrsSpent;
    }
    return -1;
}

bool GamerMap::play(std::string name, double hours) {
    // If no person with the specified name is in the map or if hours
    // is negative, make no change to the map and return false.
    // Otherwise, increase by the hours parameter the number of hours
    // the indicated person has spent gaming and return true.
    if(hours < 0 || !m_gamerMap.contains(name))
        return false;
    double currHours;
    m_gamerMap.get(name, currHours);
    m_gamerMap.update(name, currHours + hours);
    return true;
}

int GamerMap::numGamers() const {
    // Return the number of people in the GamerMap.
    return m_size;
}

void GamerMap::print() const {
    // Write to cout one line for every person in the map. Each line
    // consists of the person's name, followed by one space,followed by
    // the number of hours that person has spent gaming. Write no other
    // text. The lines need not be in any particular order.
    for(int i = 0; i < m_size; i++) {
        string name;
        double hrsSpent;
        m_gamerMap.get(i, name, hrsSpent);
        cout << name << " " << hrsSpent << endl;
    }
}
