#include <iostream>
#include <cassert>
using namespace std;

bool somePredicate(double n){
    return n > 0;
}

bool anyFalse(const double a[], int n) {
    if(n <= 0)
        return false;
    if(!somePredicate(a[0]))
        return true;
    return anyFalse(a + 1, n - 1);
}

int countFalse(const double a[], int n) {
    if(n <= 0)
        return 0;
    if(!somePredicate(a[0]))
        return 1 + countFalse(a + 1, n - 1);
    return countFalse (a + 1, n - 1);
}

int firstFalse(const double a[], int n) {
    if(n <= 0)
        return -1;
    if(!somePredicate(a[0]))
        return 0;
    int pos = firstFalse(a + 1, n - 1);
    if(pos != -1)
        return pos + 1; // adding 1 corresponds to the index in the OG array
    return -1;
}

int locateMinimum(const double a[], int n) {
    if(n <= 0)
        return -1;
    if(n == 1)
        return 0;
    int min = locateMinimum(a + 1, n - 1);
    if(a[0] < a[min + 1])
        return 0;
    return min + 1;
}

bool includes(const double a1[], int n1, const double a2[], int n2) {
    if(n2 <= 0)
        return true;
    if(n1 <= 0)
        return false;
    if(a1[0] == a2[0])
        return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
    return includes(a1 + 1, n1 - 1, a2, n2);
}

int main() {
    // anyFalse
    double a [] = {0, 1, 1, -2, 4, -5, -10, 10, 0, -1};
    assert(anyFalse(a, 10) == true);
    assert(anyFalse(a + 1, 2) == false);
    assert(anyFalse(a + 3, 7) == true);
    double b [] = {1, 2, 3, 4, 5};
    double c [] = {0};
    assert(anyFalse(b, 5) == false);
    
    // countFalse
    assert(countFalse(a, 10) == 6);
    assert(countFalse(a + 3, 7) == 5);
    assert(countFalse(b, 5) == 0);
    assert(countFalse(c, 1) == 1);
    
    // firstFalse
    assert(firstFalse(a, 10) == 0);
    assert(firstFalse(a + 1, 9) == 2);
    assert(firstFalse(a + 7, 3) == 1);
    assert(firstFalse(b, 5) == -1);
    assert(firstFalse(c, 1) == 0);
    
    // locateMinimum
    assert(locateMinimum(a, 10) == 6);
    assert(locateMinimum(a + 3, 7) == 3);
    assert(locateMinimum(c, 1) == 0);
    assert(locateMinimum(b, -1) == -1);
    
    // includes
    double d [] = {10, 50, 40, 20, 50, 40, 30};
    double e [] = {50, 20, 30};
    assert(includes(d, 7, e, 3));
    e[1] = 40;
    e[2] = 40;
    assert(includes(d, 7, e, 3));
    e[1] = 30;
    e[2] = 20;
    assert(!includes(d, 7, e, 3));
    e[0] = 10;
    e[1] = 20;
    e[2] = 20;
    assert(!includes(d, 7, e, 3));
    cerr << "Passed all test cases." << endl;
}
