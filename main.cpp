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
#include <set>
#include <ctime>
#include <queue>

using namespace std;

int g_k;
int g_n;
int g_states;
int g_countTiles;
enum Move {MOVE_UP = -1, MOVE_DOWN = 1, MOVE_LEFT = -2, MOVE_RIGHT = 2, NA = 0};

/*
 * =====================================================================================
 *        Class:  State
 *  Description:  A puzzle state
 * =====================================================================================
 */
class State
{
  public:
	int* m_tiles;
    Move  m_parentAction;
  private:
    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  GetBlankTilePosition
     *  Description:  Get the blank tile position
     * =====================================================================================
     */
    int GetBlankTilePosition ( )
    {
        for (int i = 0; i < g_countTiles; i++)
            if (m_tiles[i] == 0)
                return i;
    }	/* -----  end of function GetBlankTilePosition  ----- */

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  MoveBlankTile
     *  Description:  Move the blank tile
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
        {
        	newState->m_parentAction = move;
            successors.push_back(newState);
        }
        else
            delete newState;
    }	/* -----  end of function AddSuccessor  ----- */

  public:
    void SetTiles(int* tiles)
    {
        m_tiles = new int[g_countTiles];
        for (int i = 0; i < g_countTiles; i++)
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
    }

    State(){}

    ~State()
    {
        delete[] m_tiles;
    }


    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  IsSameState
     *  Description:  Check to see if the passed in state is the same as this state
     * =====================================================================================
     */
    bool IsSameState ( const State& state ) const
    {
        for (int i = 0; i < g_countTiles; i++)
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
        if (m_parentAction != MOVE_DOWN) AddSuccessor(successors, MOVE_UP);
        if (m_parentAction != MOVE_UP) AddSuccessor(successors, MOVE_DOWN);
        if (m_parentAction != MOVE_RIGHT) AddSuccessor(successors, MOVE_LEFT);
        if (m_parentAction != MOVE_LEFT) AddSuccessor(successors, MOVE_RIGHT);
    }	/* -----  end of function GenerateSuccessors  ----- */

    /*
	 * ===  FUNCTION  ======================================================================
	 *         Name:  Print
	 *  Description:  Print the state (FOR DEBUGGING ONLY)
	 * =====================================================================================
	 */
    void Print() const
    {
        for (int i = 0; i < g_countTiles; i++)
        {
            if (i % g_n == 0)
                cout << endl;
            cout << m_tiles[i] << " ";
        }

        cout << endl;
    }	/* -----  end of function Print  ----- */

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  HMisplacedTiles
     *  Description:  Calculate heuristic = the number of misplaced tiles
     * =====================================================================================
     */
    int HMisplacedTiles(const State& goalState)
    {
        int h = 0;
        for (int i = 0; i < g_countTiles; i++)
            if (m_tiles[i] != 0 && m_tiles[i] != goalState.m_tiles[i])
                h++;

        return h;
    }	/* -----  end of function HMisplacedTiles  ----- */

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
        for (int i = 0; i < g_countTiles; i++)
            if (m_tiles[i] != 0 && m_tiles[i] != goalState.m_tiles[i])
                for (int j = 0; j < g_countTiles; j++)
                    if (m_tiles[i] == goalState.m_tiles[j])
                    {
                        h += abs(float(i%g_n - j%g_n)) + abs(float(i/g_n - j/g_n));
                        break;
                    }

        return h;
    }	/* -----  end of function HManhattanDistance  ----- */

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  HCustom
     *  Description:  Calculate heuristic = ...
     * =====================================================================================
     */
    int HCustom(const State& goalState)
    {

    }	/* -----  end of function HCustom  ----- */

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
  private:
	class Compare_f
	{
		public:
			bool operator() ( const Node *x, const Node *y ) const
			{
				return x->m_f > y->m_f;
			}
	};

	class Compare_state
	{
		public:
			bool operator() ( const Node* x, const Node* y ) const
			{
				for (int i = 0; i < g_countTiles; i++)
					if (x->m_state.m_tiles[i] !=  y->m_state.m_tiles[i])
						return x->m_state.m_tiles[i] < y->m_state.m_tiles[i];

				return false;
			}
	};
  public:
    State m_goalState;
    priority_queue<Node*, vector<Node*>, Compare_f> m_openNodes;
    set<Node*, Compare_state> m_allNodes;		// set of closed nodes

    AStarSearch(const State& goalState, Node& initNode): m_goalState(goalState)
    {
    	initNode.m_state.m_parentAction = NA;
        m_openNodes.push(&initNode);
        m_allNodes.insert(&initNode);
    }

    ~AStarSearch()
    {/*
        cout << "Deleting " << m_openNodes.size() << " open nodes" << endl;
        for (set<Node*>::iterator i = m_openNodes.begin(); i != m_openNodes.end(); i++)
        {
            delete *i;
        }

        m_setOpenNode.clear();*/
/*
        cout << "Deleting " << m_allNodes.size() << " nodes" << endl;
        int a = 1;
		for (set<Node*>::iterator i = m_allNodes.begin(); i != m_allNodes.end(); i++)
		{
			cout << a << endl;
			a++;
			delete *i;
		}
        m_allNodes.clear();*/
    }

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  Solve
     *  Description:  Solve the puzzle using A* searching algorithm
     *                Return pathcost to the optimal solution, -1 if not solved
     * =====================================================================================
     */
    int Solve()
    {
        vector<State*> successors;
        set<Node*>::iterator existed;
        Node* newNode;
        int newPathCost;

        while(!m_openNodes.empty())
        {
            for (int i = 0; i < successors.size(); i++)
                delete successors[i];
            successors.clear();

            // get the node with the lowest F() value
            Node *bestNode = m_openNodes.top();
            m_openNodes.pop();

            // FOUND solution if it is the goal node
            if (bestNode->m_state.IsSameState(m_goalState))
                return bestNode->m_pathCost;

            // ...if not

            // generate its successors
            newPathCost = bestNode->m_pathCost+1;
            bestNode->m_state.GenerateSuccessors(successors);

            // check each successor State
            for (int i = 0; i < successors.size(); i++)
            {
            	newNode = new Node(*successors[i], newPathCost);

                // is it existed in m_allNodes ?
            	existed = m_allNodes.find(newNode);
                if (existed != m_allNodes.end())
                {
                    // is the existed one better ?
                    if ((*existed)->m_pathCost <= newPathCost)
                    {
                        // yes, the existed node is better than the new one
                        continue;
                    }
                    else
                    {
                    	m_allNodes.erase(existed);
                    }
                } else g_states++;

                // it passed all the tests
                newNode->ComputeF(m_goalState);

                m_openNodes.push(newNode);
                m_allNodes.insert(newNode);
            }

            /*
            if ((g_n == 3 && g_states >= 250000) ||
            	(g_n == 4 && g_states >= 600000) ||
            	(g_n >= 5 && g_states >= 1000000))
            	return -1;
            */
        }

        return -1;

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
    fin >> g_countTiles;
    g_countTiles++;
    fin >> g_k;
    g_n = sqrt(g_countTiles);
    cout << "n = " << g_n << ", k = " << g_k << endl;

    // allocate memory
    initTiles = new int[g_countTiles];
    goalTiles = new int[g_countTiles];

    // read init tiles
    for (int i = 0; i < g_countTiles; i++)
    {
        fin >> tile;
        initTiles[i] = tile;
    }

    // read goal tiles
    for (int i =0; i < g_countTiles; i++)
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
    int result;
    Input("nPuzzle.inp", goalState, initNode);

    AStarSearch as = AStarSearch(goalState, initNode);


    time_t tstart, tend;
    cout << "START STATE:";
    as.m_openNodes.top()->m_state.Print();
    tstart = time(0);
    cout << "=========== Solving ===========" << endl;
    result = as.Solve();
    cout << "=========== Finished Solving ===========" << endl;
    tend = time(0);
    cout << "It took " << difftime(tend, tstart) << " second(s)." << endl;

    cout << "=== RESULT ===" << endl;
    cout << result << " " << g_states << endl;
    return 0;
}
