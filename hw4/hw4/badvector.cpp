#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
using namespace std;

vector<int> destroyedOnes;

class Movie {
    public:
        Movie(int r) : m_rating(r) {}
        ~Movie() { destroyedOnes.push_back(m_rating); }
        int rating() const { return m_rating; }
    private:
        int m_rating;
};

void removeBad(vector<Movie*>& v){
    vector<Movie*>::iterator it = v.begin();
    while(it != v.end()){
        if((*it)->rating() < 50){
            delete *it; // delete movie pointed to by Movie*
            v.erase(it); // delete Movie*
        }
        else
            it++;
    }
}

void test() {
    int a[9] = { 25, 85, 80, 30, 70, 20, 15, 90, 10 };
    vector<Movie*> x;
    for (int k = 0; k < 9; k++)
        x.push_back(new Movie(a[k]));
    assert(x.size() == 9 && x.front()->rating() == 25 && x.back()->rating() == 10);
    removeBad(x);
    assert(x.size() == 4 && destroyedOnes.size() == 5);
    vector<int> v;
    for (int k = 0; k < 4; k++)
        v.push_back(x[k]->rating());
    sort(v.begin(), v.end());
    int expect[4] = { 70, 80, 85, 90 };
    for (int k = 0; k < 4; k++)
        assert(v[k] == expect[k]);
    sort(destroyedOnes.begin(), destroyedOnes.end());
    int expectGone[5] = { 10, 15, 20, 25, 30 };
    for (int k = 0; k < 5; k++)
        assert(destroyedOnes[k] == expectGone[k]);
    for (vector<Movie*>::iterator p = x.begin(); p != x.end(); p++)
        delete *p;
}

int main() {
    test();
    cout << "Passed" << endl;
}

