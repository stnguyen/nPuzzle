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
int g_states;

/*
 * =====================================================================================
 *        Class:  State
 *  Description:  A puzzle state
 * =====================================================================================
 */
class State
{
  private:
    int* m_tiles;
    enum Move {MOVE_UP = -1, MOVE_DOWN = 1, MOVE_LEFT = -2, MOVE_RIGHT = 2};

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
    bool MoveBlankTile ( Move move )
    {
        int blankPos = GetBlankTilePosition();
        int newPos;

        // is it a valid move ?
        if (move == MOVE_UP && blankPos / g_n == 0          ||
            move == MOVE_DOWN && blankPos / g_n == g_n - 1  ||
            move == MOVE_LEFT && blankPos % g_n == 0        ||
            move == MOVE_RIGHT && blankPos % g_n == g_n - 1)
            return false;

        // new blank tile position
        if (move == MOVE_UP || move == MOVE_DOWN)
            newPos = blankPos + move*g_n;
        else // move MOVE_LEFT or MOVE_RIGHT
            newPos = blankPos + move/2;

        // swap tiles
        m_tiles[blankPos] = m_tiles[newPos];
        m_tiles[newPos] = 0;
        return true;
    }		/* -----  end of function MoveBlankTile  ----- */

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  AddSuccessor
     *  Description:  Add the generated successor by moving blank tile to the given vector
     * =====================================================================================
     */
    void AddSuccessor ( vector<State*>& successors, Move move )
    {
        State* newState = new State(m_tiles);
        if (newState->MoveBlankTile(move))
            successors.push_back(newState);
        else
            delete newState;
    }	/* -----  end of function AddSuccessor  ----- */

  public:
    void SetTiles(int* tiles)
    {
        m_tiles = new int[g_n*g_n];
        for (int i = 0; i < g_n*g_n; i++)
            m_tiles[i] = tiles[i];
    }

    State(int* tiles)
    {
        SetTiles(tiles);
        //cout << "=== New State(int* tiles)" << endl;
        //Print();
    }

    State(const State& state)
    {
        SetTiles(state.m_tiles);
        //cout << "=== New State(State&)" << endl;
        //Print();
    }

    State(){}

    ~State()
    {
        //cout << "=== Delete State" << endl;
        //Print();
        delete[] m_tiles;
    }


    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  IsSameState
     *  Description:  Check to see if the passed in state is the same as this state
     * =====================================================================================
     */
    bool IsSameState ( const State& state )
    {
        for (int i = 0; i < g_n*g_n; i++)
            if (m_tiles[i] != state.m_tiles[i])
                return false;

        return true;
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
        for (int i = 0; i < g_n*g_n; i++)
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
        for (int i = 0; i < g_n*g_n; i++)
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
        for (int i = 0; i < g_n*g_n; i++)
            if (m_tiles[i] != goalState.m_tiles[i])
                for (int j = 0; j < g_n*g_n; j++)
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

    Node(State state, int pathCost): m_state(state), m_pathCost(pathCost)
    {
        //cout << "New Node(State state, int pathCost)" << endl;
    }

    Node(): m_pathCost(0) {}

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

  private:

    class HeapCompare_f
	{
		public:

			bool operator() ( const Node *x, const Node *y ) const
			{
				return x->m_f > y->m_f;
			}
	};



  public:

    AStarSearch(const State& goalState, Node& initNode): m_goalState(goalState)
    {
        m_openNodes.push_back(&initNode);
        // sort to make a heap
        push_heap(m_openNodes.begin(), m_openNodes.end(), HeapCompare_f());
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

            //cout << "======= WORKING WITH ==========" << endl;
            //bestNode->m_state.Print();

            // is it the goal node?...
            if (bestNode->m_state.IsSameState(m_goalState))
            {
                cout << "FOUND!!!!! HURRAAA :D" << endl;
                cout << bestNode->m_pathCost << " " << g_states << endl;
                return true;
            }

            // ...if not

            newPathCost = bestNode->m_pathCost+1;
            //cout << "generate its successor States" << endl;
            bestNode->m_state.GenerateSuccessors(successors);
            //cout << successors.size() << " successors generated" << endl;

            // check each successor State
            for (int i = 0; i < successors.size(); i++)
            {
                //cout << "    checking ...." << endl;
                //successors[i]->Print();

                // is it existed in m_openNodes ?
                for( existedInOpen = m_openNodes.begin(); existedInOpen != m_openNodes.end(); existedInOpen ++ )
                    if (successors[i]->IsSameState((*existedInOpen)->m_state)) break;
                if (existedInOpen != m_openNodes.end())
                {
                    // is the existed one better ?
                    if ((*existedInOpen)->m_pathCost <= newPathCost)
                    {
                        // yeah, the existed one is better, let's ignore this State
                        //cout << "    ...existed in open list!" << endl;
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
                        //cout << "    ...existed in closed list!" << endl;
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

                //cout << "    ...passed!" << endl;

                // generate a Node with the corresponding State
                newNode = new Node(*successors[i], newPathCost);
                newNode->ComputeF(m_goalState);
                //cout << "    f = " << newNode->m_f << endl;

                // newNode is the best so far compare to those having the same State
                // add it to m_openNodes
                m_openNodes.push_back(newNode);
                g_states++;
                // sort to remain the heap
                push_heap(m_openNodes.begin(), m_openNodes.end(), HeapCompare_f());
            }

            m_closedNodes.push_back(bestNode);
            //cout << "# open nodes: " << m_openNodes.size() << endl;
            //cout << "# open nodes: " << m_closedNodes.size() << endl;
            if (g_states % 1000 == 0) cout << g_states << endl;
            //if (g_states > 60 ) return false;
        }

        return false;
        cout << "=========== Finished Solving ===========" << endl;
    }   /* -----  end of function Solve  ----- */
};


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  Input
 *  Description:  Read input file
 * =====================================================================================
 */
void Input(char* filename, State& goalState, Node& initNode)
{
    cout << "=========== Reading file: " << filename << "..." << endl;

    int tile;
    int* initTiles;
    int* goalTiles;
    ifstream fin(filename);

    // read n, k
    fin >> g_n;
    fin >> g_k;
    cout << "n = " << g_n << ", k = " << g_k << endl;

    // allocate memory
    initTiles = new int[g_n*g_n];
    goalTiles = new int[g_n*g_n];

    // read init tiles
    for (int i = 0; i < g_n*g_n; i++)
    {
        fin >> tile;
        initTiles[i] = tile;
    }

    // read goal tiles
    for (int i =0; i < g_n*g_n; i++)
    {
        fin >> tile;
        goalTiles[i] = tile;
    }

    goalState.SetTiles(goalTiles);

    initNode.m_state.SetTiles(initTiles);
    initNode.ComputeF(goalState);

    g_states = 0;


    delete[] initTiles;
    delete[] goalTiles;
    fin.close();

    cout << "============ Finish reading file!" << endl;
}   /* -----  end of function Input  ----- */

int main()
{
    State goalState;
    Node  initNode;
    Input("input", goalState, initNode);
    AStarSearch as = AStarSearch(goalState, initNode);

    cout << "START STATE:";
    as.m_openNodes.front()->m_state.Print();
    as.Solve();


    cout << "=== GOODBYE ===" << endl;
    return 0;
}

