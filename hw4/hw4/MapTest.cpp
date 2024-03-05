#include "Map.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

void test()
{
    Map<int, double> mid;
    Map<string, int> msi;
    assert(mid.empty());
    assert(msi.empty());
    assert(mid.size() == 0);
    assert(msi.size() == 0);
    assert(mid.insert(10, 3.5));
    assert(msi.insert("Hello", 10));
    assert(mid.update(10, 4.75));
    assert(msi.update("Hello", 20));
    assert(mid.insertOrUpdate(20, 5.25));
    assert(msi.insertOrUpdate("Goodbye", 30));
    assert(mid.erase(20));
    assert(msi.erase("Goodbye"));
    assert(mid.contains(10));
    assert(msi.contains("Hello"));
    double d;
    assert(mid.get(10, d));
    int k;
    assert(msi.get("Hello", k));
    assert(mid.get(0, k, d));
    string s;
    assert(msi.get(0, s, k));
    Map<int, double> mid2(mid);
    Map<string, int> msi2(msi);
    mid2.swap(mid);
    msi2.swap(msi);
    mid2 = mid;
    msi2 = msi;
    merge(mid,mid,mid);
    merge(msi,msi2,msi);
    reassign(mid,mid);
    reassign(msi,msi2);
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
