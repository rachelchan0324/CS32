#include <stack>
#include <string>
#include <iostream>

using namespace std;

class Coord{
    public:
        Coord(int r, int c) : m_r(r), m_c(c) {}
        int r() const { return m_r; }
        int c() const { return m_c; }
    private:
        int m_r;
        int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    stack <Coord> coordStack;
    coordStack.push(Coord(sr, sc));
    maze[sr][sc] = 'X';
    while(!coordStack.empty()){
        Coord curr = coordStack.top();
        cerr << "(" << curr.r() << ", " << curr.c() << ")" << endl;
        coordStack.pop();
        if(curr.r() == er && curr.c() == ec)
            return true;
        if(maze[curr.r() - 1][curr.c()] == '.'){ // NORTH
            coordStack.push(Coord(curr.r() - 1, curr.c()));
            maze[curr.r() - 1][curr.c()] = 'X';
        }
        if(maze[curr.r()][curr.c() - 1] == '.'){ // WEST
            coordStack.push(Coord(curr.r(), curr.c() - 1));
            maze[curr.r()][curr.c() - 1] = 'X';
        }
        if(maze[curr.r() + 1][curr.c()] == '.') { // SOUTH
            coordStack.push(Coord(curr.r() + 1, curr.c()));
            maze[curr.r() + 1][curr.c()] = 'X';
        }
        if(maze[curr.r()][curr.c() + 1] == '.'){ // EAST
            coordStack.push(Coord(curr.r(), curr.c() + 1));
            maze[curr.r()][curr.c() + 1] = 'X';
        }
    }
    return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XXXXX.X.XX",
        "X...X.X..X",
        "X.X...X..X",
        "X.XXX.XXXX",
        "XXX....X.X",
        "X.XX.XXX.X",
        "X....X...X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 6,5, 1,8))
        cerr << "Solvable!" << endl;
    else
        cerr << "Out of luck!" << endl;
}
