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
#include <cmath>

using namespace std;

/*
 * =====================================================================================
 *        Class:  State
 *  Description:  A puzzle state
 * =====================================================================================
 */
class State
{

  /* ===================== VARIABLES ===================== */
  public:
    vector<int> m_tiles;
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
            successors.push_back(State(m_n, tiles));
    }	/* -----  end of function AddSuccessor  ----- */

  public:

    State(int n, const vector<int>& tiles)
    {
        m_n = n;
        m_tiles = tiles;
        cout << "New State(const vector<int>& tiles)" << endl;
    };

    State(const State& state)
    {
        m_n = state.m_n;
        m_tiles = state.m_tiles;
        cout << "New State(State&)" << endl;
    }

    State(){}

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

    void Print() const
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

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  HMisplacedTiles
     *  Description:  Calculate heuristic = the number of misplaced tiles
     * =====================================================================================
     */
    int HMisplacedTiles(const State& goalState)
    {
        Print();
        int h = 0;
        for (int i = 0; i < m_tiles.size(); i++)
            if (m_tiles[i] != goalState.m_tiles[i])
                h++;

        return h;
    }

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  HManhattanDistance
     *  Description:  Calculate heuristic = the sum of the distances of the tiles
     *                from their goal positions
     * =====================================================================================
     */
    int HManhattanDistance(const State& goalState)
    {
        Print();
        int h = 0;
        for (int i = 0; i < m_tiles.size(); i++)
            if (m_tiles[i] != goalState.m_tiles[i])
                for (int j = 0; j < goalState.m_tiles.size(); j++)
                    if (m_tiles[i] == goalState.m_tiles[j])
                    {
                        h += abs(float(i%m_n - j%m_n)) + abs(float(i/m_n - j/m_n));
                        break;
                    }

        return h;
    }

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  HCustom
     *  Description:  Calculate heuristic = ...
     * =====================================================================================
     */
    int HCustom(const State& goalState)
    {
        Print();
        int h = 0;
        for (int i = 0; i < m_tiles.size(); i++)
            if (m_tiles[i] != goalState.m_tiles[i])
                for (int j = 0; j < goalState.m_tiles.size(); j++)
                    if (m_tiles[i] == goalState.m_tiles[j])
                    {
                        h += abs(float(i%m_n - j%m_n)) + abs(float(i/m_n - j/m_n));
                        break;
                    }

        return h;
    }

    int H(const State& goalState)
    {
        return HManhattanDistance(goalState);
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
class Node
{
  public:
    State m_state;
    int   m_pathCost;  // also is the depth of the node

    Node()
    {
        cout << "New Node()" << endl;
    }

    Node(State state, int pathCost)
    {
        cout << "New Node(State state, int pathCost)" << endl;
        m_state = state;
        m_pathCost = pathCost;
    }

    int F(const State& goalState)
    {
        return m_pathCost + m_state.H(goalState);
    }
};

/*
 * =====================================================================================
 *        Class:  AStarSearch
 *  Description:  Solve the puzzle using a star search
 * =====================================================================================
 */
class AStarSearch
{
  public:
    State* m_goalState;
    vector<Node*> m_openNodes;
    vector<Node*> m_closedNodes;
    int m_n;
    int m_k;

  private:

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  Input
     *  Description:  Read input file
     * =====================================================================================
     */
    void Input(char* filename)
    {
        cout << "=========== Reading file: " << filename << "..." << endl;

        int tile;
        vector<int> initTiles, goalTiles;
        ifstream fin(filename);

        // read n, k
        fin >> m_n;
        fin >> m_k;
        cout << "n = " << m_n << ", k = " << m_k << endl;

        // read init state
        for (int i =0; i < m_n*m_n; i++)
        {
            fin >> tile;
            initTiles.push_back(tile);
        }
        m_openNodes.push_back(new Node(State(m_n, initTiles), 0));

        // read goal state
        for (int i =0; i < m_n*m_n; i++)
        {
            fin >> tile;
            goalTiles.push_back(tile);
        }
        m_goalState = new State(m_n, goalTiles);

        fin.close();

        cout << "============ Finish reading file!" << endl;
    }   /* -----  end of function Input  ----- */

  public:

    AStarSearch(char* filename)
    {
        Input(filename);
        //m_goalState->Print();

        //m_openNodes[0]->m_state.Print();
    }

    ~AStarSearch()
    {
        cout << "Deleting open nodes" << endl;
        for (int i = 0; i < m_openNodes.size(); i++)
        {
            cout << "   - delete node #" << i << endl;
            delete m_openNodes[i];
        }

        m_openNodes.clear();

        cout << "Deleting closed nodes" << endl;
        for (int i = 0; i < m_closedNodes.size(); i++)
        {
            cout << "delete node #" << i << endl;
            delete m_closedNodes[i];
        }
        m_openNodes.clear();

        cout << "Deleting goal state" << endl;
        delete m_goalState;
    }

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  Solve
     *  Description:  Solve the puzzle using A* searching algorithm
     *                Return true if found a solution, false otherwise
     * =====================================================================================
     */
    bool Solve()
    {
        cout << "=========== Solving ===========" << endl;

        while(!m_openNodes.empty())
        {

        }

        return false;
        cout << "=========== Finished Solving ===========" << endl;
    }   /* -----  end of function Solve  ----- */
};


int main()
{
    AStarSearch as = AStarSearch("input");
    cout << as.m_openNodes[0]->F(*as.m_goalState) << endl;

    cout << "=== GOODBYE ===" << endl;
    return 0;
}
