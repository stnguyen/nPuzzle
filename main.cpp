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
#include <algorithm>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

int g_k;
int g_n;

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
        if (pos >= 0 && pos < g_n*g_n)
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
        for (int i = 0; i < g_n*g_n; i++)
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
            newPos = blankPos + move*g_n;
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
    void AddSuccessor ( vector<State*>& successors, Move move )
    {
        vector<int> tiles = m_tiles;
        if (MoveBlankTile(move, tiles))
            successors.push_back(new State(tiles));
    }	/* -----  end of function AddSuccessor  ----- */

  public:

    State(const vector<int>& tiles)
    {
        m_tiles = tiles;
        //cout << "=== New State(const vector<int>& tiles)" << endl;
        //Print();
    };

    State(const State& state)
    {
        m_tiles = state.m_tiles;
       // cout << "=== New State(State&)" << endl;
        //Print();
    }

    State(){}

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  IsSameState
     *  Description:  Check to see if the passed in state is the same as this state
     * =====================================================================================
     */
    bool IsSameState ( const State& state )
    {
        // std::vector can be deeply compare by using ==
        return m_tiles == state.m_tiles;
    }   /* -----  end of function IsSameState  ----- */



    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  GenerateSuccessors
     *  Description:  Generate all valid successors form this node
     * =====================================================================================
     */
    void GenerateSuccessors (vector<State*>& successors)
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
        for (int i = 0; i < m_tiles.size(); i++)
        {
            if (i % g_n == 0)
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
        int h = 0;
        for (int i = 0; i < m_tiles.size(); i++)
            if (m_tiles[i] != goalState.m_tiles[i])
                for (int j = 0; j < goalState.m_tiles.size(); j++)
                    if (m_tiles[i] == goalState.m_tiles[j])
                    {
                        h += abs(float(i%g_n - j%g_n)) + abs(float(i/g_n - j/g_n));
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

    }

    int H(const State& goalState)
    {
        if (g_k == 1)
            return HMisplacedTiles(goalState);
        else if (g_k == 2)
            return HManhattanDistance(goalState);
        else
            return HCustom(goalState);
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
  private:


  public:
    State m_state;
    int   m_pathCost;  // also is the depth of the node
    int   m_f;

    Node(State state, int pathCost)
    {
        //cout << "New Node(State state, int pathCost)" << endl;
        m_state = state;
        m_pathCost = pathCost;
    }

    void ComputeF(const State& goalState)
    {
        m_f = m_pathCost + m_state.H(goalState);
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
    State m_goalState;
    vector<Node*> m_openNodes;
    vector<Node*> m_closedNodes;
    int m_states;   // states count

  private:

    class HeapCompare_f
	{
		public:

			bool operator() ( const Node *x, const Node *y ) const
			{
				return x->m_f > y->m_f;
			}
	};

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
        Node* startNode;

        // read n, k
        fin >> g_n;
        fin >> g_k;
        cout << "n = " << g_n << ", k = " << g_k << endl;

        // read init state
        for (int i =0; i < g_n*g_n; i++)
        {
            fin >> tile;
            initTiles.push_back(tile);
        }

        // read goal state
        for (int i =0; i < g_n*g_n; i++)
        {
            fin >> tile;
            goalTiles.push_back(tile);
        }

        m_goalState = State(goalTiles);

        startNode = new Node(State(initTiles), 0);
        m_states = 0;
        startNode->ComputeF(m_goalState);
        m_openNodes.push_back(startNode);
        // sort to make a heap
        push_heap(m_openNodes.begin(), m_openNodes.end(), HeapCompare_f());

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
        cout << "Deleting " << m_openNodes.size() << " open nodes" << endl;
        for (int i = 0; i < m_openNodes.size(); i++)
        {
            delete m_openNodes[i];
        }

        m_openNodes.clear();

        cout << "Deleting " << m_closedNodes.size() << " closed nodes" << endl;
        for (int i = 0; i < m_closedNodes.size(); i++)
        {
            delete m_closedNodes[i];
        }
        m_openNodes.clear();
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

        vector<State*> successors;
        vector<Node*>::iterator existedInOpen;
        vector<Node*>::iterator existedInClosed;
        Node* newNode;
        int newPathCost;

        while(!m_openNodes.empty())
        {
            for (int i = 0; i < successors.size(); i++)
                delete successors[i];
            successors.clear();

            // get the node with the lowest F() value
            Node *bestNode = m_openNodes.front();
            pop_heap( m_openNodes.begin(), m_openNodes.end(), HeapCompare_f() );
            m_openNodes.pop_back();

            // is it the goal node?...
            if (bestNode->m_state.IsSameState(m_goalState))
            {
                cout << "FOUND!!!!! HURRAAA :D" << endl;
                cout << bestNode->m_pathCost << " " << m_states << endl;
                return true;
            }

            // ...if not

            newPathCost = bestNode->m_pathCost+1;
            // generate its successor States
            bestNode->m_state.GenerateSuccessors(successors);

            // check each successor State
            for (int i = 0; i < successors.size(); i++)
            {
                // is it existed in m_openNodes ?
                for( existedInOpen = m_openNodes.begin(); existedInOpen != m_openNodes.end(); existedInOpen ++ )
                    if (successors[i]->IsSameState((*existedInOpen)->m_state)) break;
                if (existedInOpen != m_openNodes.end())
                {
                    // is the existed one better ?
                    if ((*existedInOpen)->m_pathCost <= newPathCost)
                    {
                        // yeah, the existed one is better, let's ignore this State

                        continue;
                    }
                }

                // is it existed in m_closedNodes ?
                for( existedInClosed = m_closedNodes.begin(); existedInClosed != m_closedNodes.end(); existedInClosed ++ )
                    if (successors[i]->IsSameState((*existedInClosed)->m_state)) break;
                if (existedInClosed != m_closedNodes.end())
                {
                    // is the existed one better ?
                    if ((*existedInClosed)->m_pathCost <= newPathCost)
                    {
                        // yeah, the existed one is better, let's ignore this State

                        continue;
                    }
                    else
                    {
                        delete (*existedInClosed);
                        m_closedNodes.erase(existedInClosed);
                    }
                }

                // remove existing Nodes that are worse
                if (existedInOpen != m_openNodes.end())
                {
                    delete (*existedInOpen);
                    m_openNodes.erase(existedInOpen);
                    make_heap(m_openNodes.begin(), m_openNodes.end(), HeapCompare_f());
                }

                // generate a Node with the corresponding State
                newNode = new Node(*successors[i],bestNode->m_pathCost+1);
                newNode->ComputeF(m_goalState);
                // newNode is the best so far compare to those having the same State
                // add it to m_openNodes
                m_openNodes.push_back(newNode);
                m_states++;
                // sort to remain the heap
                push_heap(m_openNodes.begin(), m_openNodes.end(), HeapCompare_f());

                cout << "# states: " << m_states << endl;
            }

            m_closedNodes.push_back(bestNode);
        }

        return false;
        cout << "=========== Finished Solving ===========" << endl;
    }   /* -----  end of function Solve  ----- */
};


int main()
{
    AStarSearch as = AStarSearch("input");
    cout << "START STATE:";
    as.m_openNodes.front()->m_state.Print();
    as.Solve();


    cout << "=== GOODBYE ===" << endl;
    return 0;
}
