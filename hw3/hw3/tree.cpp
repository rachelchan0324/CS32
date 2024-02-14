#include <cassert>
#include <iostream>
using namespace std;

int countIncludes(const double a1[], int n1, const double a2[], int n2) {
    if(n2 <= 0)
        return 1;
    if(n1 <= 0)
        return 0;
    
    if(a1[0] == a2[0]){
        int remainingWays = countIncludes(a1 + 1, n1 - 1, a2, n2); // get the remaining ways to solve this problem
        return remainingWays + countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
    }
    return countIncludes(a1 + 1, n1 - 1, a2, n2); // does not match, iterate
}

// Exchange two doubles
void exchange(double& x, double& y) {
    double t = x;
    x = y;
    y = t;
}

void divide(double a[], int n, double divider, int& firstNotGreater, int& firstLess) {
    if (n < 0)
        n = 0;
    
    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > divider
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == divider
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < divider
    
    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess) {
        if (a[firstUnknown] < divider) {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else {
            if (a[firstUnknown] > divider) {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}

void order(double a[], int n) {
    if(n <= 1) // base case
        return;
    int firstNotGreater, firstLess;
    divide(a, n, a[n/2], firstNotGreater, firstLess);
    order(a, firstNotGreater);
    order(a + firstLess, n - firstLess);
}

int main(){
    // countIncludes()
    double a1 [] = {10, 50, 40, 20, 50, 40, 30};
    double a2 [] = {10, 20, 40};
    assert(countIncludes(a1, 7, a2, 3) == 1);
    double a3 [] = {10, 40, 30};
    assert(countIncludes(a1, 7, a3, 3) == 2);
    double a4 [] = {20, 10, 40};
    assert(countIncludes(a1, 7, a4, 3) == 0);
    double a5 [] = {50, 40, 30};
    assert(countIncludes(a1, 7, a5, 3) == 3);
    
    // order()
    order(a1, 7);
    assert(a1[6] == 10 && a1[5] == 20 && a1[4] == 30 && a1[3] == 40 && a1[2] == 40 && a1[1] == 50 && a1[0] == 50);
    order(a2, 3);
    assert(a2[2] == 10 && a2[1] == 20 && a2[0] == 40);
    
    cerr << "All test cases passed." << endl;
}
