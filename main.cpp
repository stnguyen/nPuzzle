/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Solve n-Puzzle using A* search algorithm
 *                  Assignment 01 - course AI, UET, VNU
 *
 *        Created:  04/13/2011 01:10:36 AM
 *
 *         Author:  Nguyen Son Tung (sontung23@gmail.com)
 *
 * =====================================================================================
 */

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

/*
 * =====================================================================================
 *        Class:  State
 *  Description:  A puzzle state
 * =====================================================================================
 */
class State {

  /* ===================== VARIABLES ===================== */
  private:
    vector<int> m_tiles;

  public:
    int m_n;   // size of the puzzle
    enum Move {MOVE_UP = -1, MOVE_DOWN = 1, MOVE_LEFT = -2, MOVE_RIGHT = 2};

  /* ===================== METHODS ===================== */
  private:

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  IsValidTile
     *  Description:  Check if the tile is in the board
     * =====================================================================================
     */
    bool IsValidTile ( int pos )
    {
        if (pos >= 0 && pos < m_n*m_n)
            return true;
        return false;
    }	/* -----  end of function IsValidTile  ----- */

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  GetBlankTilePosition
     *  Description:  Get the blank tile position
     * =====================================================================================
     */
    int GetBlankTilePosition ( )
    {
        for (int i = 0; i < m_n*m_n; i++)
        if (m_tiles[i] == 0)
            return i;
    }	/* -----  end of function GetBlankTilePosition  ----- */

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  MoveBlankTile
     *  Description:  Move the blank tile on the passed in duplicatedTiles
     *                Return true if succeeded, false otherwise
     * =====================================================================================
     */
    bool MoveBlankTile ( Move move, vector<int>& duplicatedTiles )
    {
        int blankPos = GetBlankTilePosition();
        int newPos;

        // new blank tile position
        if (move == MOVE_UP || move == MOVE_DOWN)
            newPos = blankPos + move*m_n;
        else // move MOVE_LEFT or MOVE_RIGHT
            newPos = blankPos + move/2;

        if (!IsValidTile(newPos))
            return false;
        else
        {
            // swap tiles
            duplicatedTiles[blankPos] = duplicatedTiles[newPos];
            duplicatedTiles[newPos] = 0;
            return true;
        }
    }		/* -----  end of function MoveBlankTile  ----- */

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  AddSuccessor
     *  Description:  Add the generated successor by moving blank tile to the given vector
     * =====================================================================================
     */
    void AddSuccessor ( vector<State>& successors, Move move )
    {
        vector<int> tiles = m_tiles;

        if (MoveBlankTile(move, tiles))
            successors.push_back(State(m_n, &tiles));
    }	/* -----  end of function AddSuccessor  ----- */

  public:

    State(int n, vector<int>* tiles)
    {
        m_tiles = *tiles;
        m_n = n;
        cout << "New State(n,*tiles)" << endl;
    };

    State(const State& state)
    {
        m_n = state.m_n;
        m_tiles = state.m_tiles;
        cout << "New State(State&)" << endl;
    }


    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  IsSameState
     *  Description:  Check to see if the passed in state is the same as this state
     * =====================================================================================
     */
    bool IsSameState ( State* state )
    {
        // std::vector can be deeply compare by using ==
        return this->m_tiles == state->m_tiles;
    }   /* -----  end of function IsSameState  ----- */



    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  GenerateSuccessors
     *  Description:  Generate all valid successors form this node
     * =====================================================================================
     */
    void GenerateSuccessors (vector<State>& successors)
    {
        successors.clear();
        // try 4 moves and get all possible succesors
        AddSuccessor(successors, MOVE_UP);
        AddSuccessor(successors, MOVE_DOWN);
        AddSuccessor(successors, MOVE_LEFT);
        AddSuccessor(successors, MOVE_RIGHT);
    }	/* -----  end of function GenerateSuccessors  ----- */

    void Print()
    {
        cout << "State: ";
        for (int i = 0; i < m_tiles.size(); i++)
        {
            if (i % m_n == 0)
                cout << endl;
            cout << m_tiles[i] << " ";
        }

        cout << endl;
    }

};

/*
 * =====================================================================================
 *        Class:  Node
 *  Description:  A node in the search tree.
 *                Because this program does not return the solution path (if found),
 *                components such as Action, Parent Node are ommited to save memory.
 * =====================================================================================
 */
class Node {
  private:
    State* m_state;
    int    m_pathCost;  // also is the depth of the node

  public:

    Node(State* state, int pathCost)
    {
        m_state = state;
        m_pathCost = pathCost;
    }


};



void Input(char* filename, vector<int>* tiles, int& n, int& h)
{
    cout << "Reading file: " << filename << "..." << endl;
    int tile;
    ifstream fin(filename);
    fin >> n;
    fin >> h;
    cout << "n = " << n << ", h = " << h << endl;


    for (int i =0; i < n*n; i++)
    {
        fin >> tile;
        tiles->push_back(tile);
    }
    fin.close();
    cout << "Finish reading file!" << endl;
}

int main()
{
    vector<int> tiles;
    int n, h;
    Input("input", &tiles, n, h);

    /* TEST State class */
    State s1(n, &tiles);
    cout << "s1 ";
    s1.Print();

    vector<State> successors;
    s1.GenerateSuccessors(successors);
    for (int i = 0; i < successors.size(); i++)
    {
        cout << "Successor: ";
        successors[i].Print();
    }

    s1 = successors[0];
    cout << "s1 ";
    s1.Print();
    s1.GenerateSuccessors(successors);
    for (int i = 0; i < successors.size(); i++)
    {
        cout << "Successor: ";
        successors[i].Print();
    }

    return 0;
}
