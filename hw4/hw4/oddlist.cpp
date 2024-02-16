#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
using namespace std;

void removeOdd(list<int>& li){
    list<int>::iterator it = li.begin();
    while(it != li.end()){
        if(*it % 2 == 1) // odd
            it = li.erase(it);
        else
            it++;
    }
}

void test() {
    int a[9] = { 5, 2, 8, 9, 6, 7, 3, 4, 1 };
    list<int> x(a, a+9);  // construct x from the array
    assert(x.size() == 9 && x.front() == 5 && x.back() == 1);
    removeOdd(x);
    assert(x.size() == 4);
    vector<int> v(x.begin(), x.end());  // construct v from x
    sort(v.begin(), v.end());
    int expect[4] = { 2, 4, 6, 8 };
    for (int k = 0; k < 4; k++)
        assert(v[k] == expect[k]);
}

int main() {
    test();
    cout << "Passed" << endl;
}
