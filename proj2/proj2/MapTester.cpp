#include "Map.h"
#include <cassert>
#include <iostream>

using namespace std;

int main () {
    // MARK: Smallbergs's Test Cases
    Map msd; // KeyType is std::string, ValueType is double
    assert(msd.insert("ccc", 80));
    assert(msd.insert("aaa", 40));
    assert(msd.insert("ddd", 60));
    assert(msd.insert("bbb", 20));
    KeyType k = "xxx";
    ValueType v;
    assert(!msd.get(4, k, v)  &&  k == "xxx");  // x is unchanged
    assert(msd.get(1, k, v)  &&  k == "bbb");   // "bbb" is greater than exactly 1 item
    
    /** merge()*/
    Map names;
    assert(names.insert("Fred", 123));
    assert(names.insert("Ethel", 456));
    assert(names.insert("Lucy",789));
    Map names1;
    assert(names1.insert("Lucy", 789));
    assert(names1.insert("Ricky", 321));
    Map result;
    assert(merge(names, names1, result));
    assert(result.size() == 4 && result.get(0, k, v) && k == "Ethel" && v == 456 && result.get(1, k, v) && k == "Fred" && v == 123 && result.get(2, k, v) && k == "Lucy" && v == 789 && result.get(3, k, v) && k == "Ricky" && v == 321);
    names1.insertOrUpdate("Lucy",654);
    assert(!merge(names, names1, result));
    assert(result.size() == 3 && result.get(0, k, v) && k == "Ethel" && v == 456 && result.get(1, k, v) && k == "Fred" && v == 123 && result.get(2, k, v) && k == "Ricky" && v == 321);
    
    Map m;
    assert(m.insert("Fred", 123));
    assert(m.insert("Ethel", 456));
    assert(m.size() == 2);
    ValueType v_1 = 42;
    assert(!m.get("Lucy", v_1)  &&  v_1 == 42);
    assert(m.get("Fred", v_1)  &&  v_1 == 123);
    v_1 = 42;
    KeyType x = "Lucy";
    assert(m.get(0, x, v_1)  &&  x == "Ethel"  &&  v_1 == 456);
    KeyType x2 = "Ricky";
    assert(m.get(1, x2, v_1)  &&  x2 == "Fred"  &&  v_1 == 123);
    
    /** reassign() */
    Map reassignResult;
    assert(names.insert("Ricky", 321));
    reassign(names, reassignResult);
    KeyType k1, k2, k3;
    ValueType v1, v2, v3;
    assert(reassignResult.size() == 4 && reassignResult.get(0, k, v) && reassignResult.get(1, k1, v1) && reassignResult.get(2, k2, v2) && reassignResult.get(3, k3, v3) && v != v1 != v2 != v3 && v != 456 && v1 != 123 && v2 != 789 && v3 != 321);
    
    assert(names.erase("Ricky"));
    assert(names.insertOrUpdate("Lucy", 456));
    reassign(names, reassignResult);
    assert(reassignResult.size() == 3 && reassignResult.get(0, k, v) && reassignResult.get(1, k1, v1) && reassignResult.get(2, k2, v2) && ((v == 123 && v1 == 456 && v2 == 456) || (v == 456 && v1 == 456 && v2 == 123)));
    
    // MARK: Rachel's Test Cases
    
    KeyType key = "";
    ValueType value = -1;
    
    /** insert() */
    Map test;
    assert(test.insert("a", 23));
    assert(test.insert("b", 46));
    assert(test.insert("c", 93));
    assert(!test.insert("c", 1)); // should not insert if the key already exists
    
    /**Map(const Map& other)**/
    Map cpy = Map(test);
    
    /** update() */
    assert(test.update("c", 7));
    assert(test.get("c", value));
    assert(value == 7);
    
    /** erase() */
    msd.erase("b"); // erase
    assert(!msd.get("b", value) && value == 7);
    
    /** swap() */
    Map test1; // swap test and test1
    assert(test1.insert("d", 109));
    assert(test1.insert("e", 58));
    test.swap(test1);
    assert(test.get(0, key, value) && key == "d" && value == 109);
    assert(test.get(1, key, value) && key == "e" && value == 58);
    
    // MARK: Brandon's Test Cases
    Map bMap;
    // Empty Map Test Cases
    assert(bMap.size() == 0);
    assert(bMap.empty());
    
    // Insert Test Cases
    assert(bMap.insert("Brandon", 1.1));
    assert(bMap.insert("Rachel", 2));
    assert(bMap.insert("Sanjay", 3));
    assert(bMap.insert("Brayden", 4));
    assert(!bMap.insert("Brandon", 1.1));
    assert(bMap.size() == 4);
    
    // Update Test Cases
    assert(bMap.update("Rachel", 2.2));
    assert(!bMap.update("Jon", 1));
    assert(bMap.size() == 4);
    
    // InsertOrUpdate Test Cases
    assert(bMap.insertOrUpdate("Brayden", 4.4));
    assert(bMap.insertOrUpdate("Jon", 5));
    
    // Erase Test Cases
    assert(bMap.erase("Brayden"));
    assert(!bMap.erase("Andrew"));
    assert(bMap.size() == 4);
    
    // Contains Test Cases
    assert(bMap.contains("Brandon"));
    assert(!bMap.contains(""));
    assert(!bMap.contains("Brayden"));
    
    // 1st Get Test Cases
    KeyType key1 = "Ellen";
    ValueType value1 = 9.99;
    assert(!bMap.get(key1, value1));
    assert(value1 == 9.99);
    key1 = "Rachel";
    assert(bMap.get(key1, value1));
    assert(value1 == 2.2);
    
    // 2nd Get Test Cases
    KeyType key2 = "Katie";
    ValueType value2 = 9.99;
    assert(!bMap.get(-1, key2, value2));
    assert(!bMap.get(bMap.size(), key2, value2));
    assert(key2 == "Katie" && value2 == 9.99);
    assert(bMap.get(2, key2, value2));
    assert(key2 == "Rachel" && value2 == 2.2);
    
    // Swap Test Cases
    Map m1;
    assert(m1.insert("a", 1));
    assert(m1.insert("b", 2));
    Map m2;
    assert(m2.insert("x", 3));
    m1.swap(m2);
    assert(m1.size() == 1 && m2.size() == 2);
    KeyType key3 = "a";
    ValueType value3;
    assert(!m1.get(key3, value3));
    assert(m2.get(key3, value3) && value3 == 1);
    
    // Merge Test Cases
    Map m3;
    assert(m3.insert("Fred", 123));
    assert(m3.insert("Ethel", 456));
    assert(m3.insert("Lucy", 789));
    Map m4;
    assert(m4.insert("Lucy", 789));
    assert(m4.insert("Ricky", 321));
    Map bResult;
    assert(bResult.insert("Your Mother", 69420));
    assert(merge(m3, m4, bResult));
    assert(bResult.size() == 4);
    Map m5;
    assert(m5.insert("Fred", 321));
    assert(!merge(m3, m5, bResult));
    assert(bResult.size() == 2);
    
    // Reassign Test Cases
    Map dance;
    assert(dance.insert("Fred", 1));
    assert(dance.insert("Ethel", 2));
    assert(dance.insert("Lucy", 3));
    assert(dance.insert("Ricky", 4));
    Map scrambled;
    assert(scrambled.insert("Love", 92523));
    reassign(dance, scrambled);
    ValueType fredNum;
    ValueType ethelNum;
    ValueType lucyNum;
    ValueType rickyNum;
    assert(scrambled.get("Fred", fredNum));
    assert(scrambled.get("Ethel", ethelNum));
    assert(scrambled.get("Lucy", lucyNum));
    assert(scrambled.get("Ricky", rickyNum));
    assert(fredNum != 1 && ethelNum != 2 && lucyNum != 3 && rickyNum != 4);
    assert(fredNum != ethelNum != lucyNum != rickyNum);
    
    cerr << "Passed all test cases." << endl;
}

