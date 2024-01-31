#include "Map.h"
#include <cassert>
#include <iostream>

using namespace std;

int main () {
    KeyType key = "";
    ValueType value = -1;

    /** default constructor and empty */
    Map test;
    assert(test.empty() && test.size() == 0); // should be empty

    /** insert */
    assert(test.insert("a", 23) && test.insert("b", 46) && test.insert("c", 93) && test.size() == 3 && !test.empty()); // tests insert(), insert 3 values
    assert(!test.insert("c", 1)); // should not insert if the key already exists

    /** copy constructor*/
    Map cpy = Map(test); // tests copy constructor
    assert(cpy.size() == 3); // cpy should have the same contents as test
    assert(cpy.contains("a") && cpy.contains("b") && cpy.contains("c")); // tests copy constructor and contains method

    /** update, insert, and insertOrUpdate*/
    assert(cpy.update("a", 32) && cpy.get("a", value) && value == 32); // update existing key
    assert(!cpy.update("d", 400) && cpy.size() == 3); // does not update if the key doesn't exist
    assert(cpy.insertOrUpdate("a", 23) && cpy.size() == 3 && cpy.get(0, key, value) && key == "a" && value == 23); // key already exists, should update
    assert(cpy.insertOrUpdate("d", 408) && cpy.size() == 4 && cpy.get("d", value) && value == 408); // key doesn't exist, should insert
    assert(!test.contains("d")); // tests copy constructor, "d" should not be added to test; cpy should not be a reference to test

    /** erase*/
    assert(!cpy.erase("z")); // erasing a key that doesn't exist does nothing
    assert(cpy.erase("d") && cpy.size() == 3 && !cpy.contains("d")); // erasing a key that exists

    /** get */
    assert(!test.get("z", value) && value == 408); // getting a key that doesn't exist, shouldn't change anything
    assert(test.get("c", value) && value == 93); // getting a value of a key that exists
    assert(cpy.size() == 3 && cpy.get(0, key, value) && key == "a" && value == 23 && cpy.get(1, key, value) && key == "b" && value == 46 && cpy.get(2, key, value) && key == "c" && value == 93); // tests get(i, key, value)
    assert(!cpy.get(-1, key, value) && !cpy.get(4, key, value) && key == "c" && value == 93); // if i is greater than size or negative, parameters should not change

    /** assignment operator */
    Map numbers;
    assert(numbers.insert("one", 1.1) && numbers.insert("two", 2.2) && numbers.insert("three", 3.3));
    Map temp(numbers); // for testing swap() later
    numbers = cpy;
    assert(cpy.size() == 3 && cpy.get(0, key, value) && key == "a" && value == 23 && cpy.get(1, key, value) && key == "b" && value == 46 && cpy.get(2, key, value) && key == "c" && value == 93); // numbers should now have the contents of cpy, testing assignment operator
    assert(cpy.insert("d", 123.6) && !numbers.contains("d")); // numbers should not be a reference to cpy

    /** swap() */
    temp.swap(cpy); // swap copy and temp
    assert(temp.size() == 4 && temp.get(0, key, value) && key == "a" && value == 23 && temp.get(1, key, value) && key == "b" && value == 46 && temp.get(2, key, value) && key == "c" && value == 93 && temp.get(3, key, value) && key == "d" && value == 123.6); // test should have cpy elements
    assert(cpy.get("one", value) && value == 1.1 && cpy.get(1, key, value) && key == "three" && value == 3.3 && cpy.size() == 3); // cpy should have temp contents now

    /** merge */
    Map names;
    assert(names.insert("Fred", 123) && names.insert("Ethel", 456) && names.insert("Lucy",789));
    Map names1;
    assert(names1.insert("Lucy", 789) && names1.insert("Ricky", 321));
    Map result(cpy); // result has initial values that should be removed
    assert(merge(names, names1, result));
    assert(result.size() == 4 && result.get(0, key, value) && key == "Ethel" && value == 456 && result.get(1, key, value) && key == "Fred" && value == 123 && result.get(2, key, value) && key == "Lucy" && value == 789 && result.get(3, key, value) && key == "Ricky" && value == 321); // all matching keys have corresponding values
    names1.insertOrUpdate("Lucy",654);
    assert(!merge(names, names1, result)); // all matching keys do not have corresponding values; return false
    assert(result.size() == 3 && result.get(0, key, value) && key == "Ethel" && value == 456 && result.get(1, key, value) && key == "Fred" && value == 123 && result.get(2, key, value) && key == "Ricky" && value == 321); // even if the keys are not corresponding, result map must still be constructed

    /** reassign() */
    Map reassignResult = result; // result has initial values that should be removed
    assert(names.insert("Ricky", 321));
    reassign(names, reassignResult);
    KeyType k1, k2, k3;
    ValueType v1, v2, v3;
    assert(reassignResult.size() == 4 && reassignResult.get(0, key, value) && reassignResult.get(1, k1, v1) && reassignResult.get(2, k2, v2) && reassignResult.get(3, k3, v3) && value != v1 != v2 != v3 && value != 456 && v1 != 123 && v2 != 789 && v3 != 321 && reassignResult.get("Fred", value) && value != 123); // all values should be different from each other
    assert(names.erase("Ricky"));
    assert(names.insertOrUpdate("Lucy", 456));
    reassign(names, reassignResult);
    assert(reassignResult.size() == 3 && reassignResult.get(0, key, value) && reassignResult.get(1, k1, v1) && reassignResult.get(2, k2, v2) && ((value == 123 && v1 == 456 && v2 == 456) || (value == 456 && v1 == 456 && v2 == 123))); // if two keys have the same value

    cerr << "Passed all test cases." << endl;
}
