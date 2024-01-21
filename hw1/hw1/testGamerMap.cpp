#include "GamerMap.h"
#include <cassert>
#include <iostream>

using namespace std;

int main () {
    GamerMap friends;
    assert(friends.numGamers() == 0);
    assert(friends.addGamer("Brandon"));
    assert(friends.addGamer("Ellen"));
    assert(friends.addGamer("Katie"));
    assert(friends.numGamers() == 3);
    assert(friends.play("Katie", 4));
    assert(friends.play("Katie", 7.6));
    assert(11.6 == friends.hoursSpent("Katie"));
    assert(0 == friends.hoursSpent("Brandon"));
    friends.print();
    cout << "Passed all tests" << endl;
}
