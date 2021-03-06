/*
 * =====================================================================================
 *
 *       Filename:  nPuzzle.cpp
 *
 *    Description:  Solve n-Puzzle using A* search algorithm
 *                  Assignment 01 - course AI
 *
 *        Started:  04/13/2011 01:10:36 AM
 *       Finished:  04/21/2011 10:15:36 PM
 *
 *         Author:  Nguyen Son Tung (sontung23@gmail.com)
 *          Class:  K53CA, UET, VNU
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

int g_k;			// which heuristic function is used
int g_n;			// n-Puzzle
int g_countTiles;	// number of tiles, including the blank one. If n == 3 then countTiles == 9
int g_states;		// count generated states

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
     *  Description:  Take the move to create a new State, then add the State to vector
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

    /*
	 * ===  FUNCTION  ======================================================================
	 *         Name:  SetTiles
	 *  Description:  Set the tiles of this State, do a deep copy
	 * =====================================================================================
	 */
    void SetTiles(int* tiles)
    {
        m_tiles = new int[g_countTiles];
        for (int i = 0; i < g_countTiles; i++)
            m_tiles[i] = tiles[i];
    }	/* -----  end of function SetTiles  ----- */

    State(int* tiles)
    {
        SetTiles(tiles);
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
     *  Description:  Check to see if the passed in state is the same as this one
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
     *  Description:  Generate all valid successors from this node
     * =====================================================================================
     */
    void GenerateSuccessors (vector<State*>& successors)
    {
        successors.clear();
        // try 4 moves and get all possible successors
        if (m_parentAction != MOVE_DOWN) AddSuccessor(successors, MOVE_UP);
        if (m_parentAction != MOVE_UP) AddSuccessor(successors, MOVE_DOWN);
        if (m_parentAction != MOVE_RIGHT) AddSuccessor(successors, MOVE_LEFT);
        if (m_parentAction != MOVE_LEFT) AddSuccessor(successors, MOVE_RIGHT);
    }	/* -----  end of function GenerateSuccessors  ----- */

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  HMisplacedTiles
     *  Description:  H1 - Calculate heuristic = the number of misplaced tiles
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
     *  Description:  H2 - Calculate heuristic = the sum of the distances of the tiles
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
     * ===  FUNCTION  =======================================================================
     *         Name:  HCustom
     *  Description:  H3 - Calculate heuristic = the sum of the FLYING distances of the tiles
     *                from their goal positions
     * ======================================================================================
     */
    int HCustom(const State& goalState)
    {
    	int h = 0;
		for (int i = 0; i < g_countTiles; i++)
			if (m_tiles[i] != 0 && m_tiles[i] != goalState.m_tiles[i])
				for (int j = 0; j < g_countTiles; j++)
					if (m_tiles[i] == goalState.m_tiles[j])
					{
						h += sqrt((i%g_n - j%g_n)*(i%g_n - j%g_n) + (i/g_n - j/g_n)*(i/g_n - j/g_n));
						break;
					}

		return h;
    }	/* -----  end of function HCustom  ----- */

    /*
	 * ===  FUNCTION  ======================================================================
	 *         Name:  H
	 *  Description:  Calculate heuristic
	 * =====================================================================================
	 */
    int H(const State& goalState)
    {
        if (g_k == 1)
            return HMisplacedTiles(goalState);
        else if (g_k == 2)
            return HManhattanDistance(goalState);
        else if (g_k == 3)
            return HCustom(goalState);
    }	/* -----  end of function H  ----- */
};

/*
 * =====================================================================================
 *        Class:  Node
 *  Description:  A node in the search tree.
 *                Because this program does not return the solution path (if found),
 *                the link to the parent node is ommited to save memory.
 * =====================================================================================
 */
class Node
{
  public:
    State m_state;
    int   m_pathCost;  // also is the depth of the node
    int   m_f;

    Node(State state, int pathCost): m_state(state), m_pathCost(pathCost){}

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

	/*
	 * =====================================================================================
	 *        Class:  Compare_f
	 *  Description:  A comparator, it first compare Nodes' estimated solution cost,
	 *  			  then the Nodes' path costs are considered
	 * =====================================================================================
	 */
	class Compare_f
	{
		public:
			bool operator() ( const Node *x, const Node *y ) const
			{
 				 if (x->m_f == y->m_f)
					 return x->m_pathCost < y->m_pathCost;
				 else
					 return x->m_f > y->m_f;
			}
	};

	/*
	 * =====================================================================================
	 *        Class:  Compare_state
	 *  Description:  A comparator, used to determine whether two Nodes have the same state
	 * =====================================================================================
	 */
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
    priority_queue<Node*, vector<Node*>, Compare_f> m_openNodes;		// open nodes
    set<Node*, Compare_state> m_allNodes;								// generated nodes

    AStarSearch(const State& goalState, Node& initNode): m_goalState(goalState)
    {
    	// init node doesn't have parent
    	initNode.m_state.m_parentAction = NA;

        m_openNodes.push(&initNode);
        m_allNodes.insert(&initNode);
    }

    ~AStarSearch()
    {
    	// no need to deallocate resources, this program is going to quit anyway...
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
        vector<State*> successors;		// temporary store successors
        set<Node*>::iterator existed;	// check to see if the new node is already existed
        Node* newNode;
        int newPathCost;

        while(!m_openNodes.empty())
        {
        	// delete all successors of existed
            for (int i = 0; i < successors.size(); i++)
                delete successors[i];
            successors.clear();

            // get the best node, i.e., the one with the lowest F() value
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

                // REPEATED STATE AVOIDANCE: is it existed in m_allNodes ?
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

                // it passed the test
                newNode->ComputeF(m_goalState);

                m_openNodes.push(newNode);
                m_allNodes.insert(newNode);
            }

            // limitation as described on BBC
            if ((g_n == 3 && g_states >= 250000) ||
            	(g_n == 4 && g_states >= 600000) ||
            	(g_n >= 5 && g_states >= 1000000))
            	return -1;
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
	int tile;
    int* initTiles;
    int* goalTiles;
    ifstream fin(filename);

    // read n, k
    fin >> g_countTiles;
    g_countTiles++;
    fin >> g_k;
    g_n = sqrt(g_countTiles);

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

    g_states = 1;


    delete[] initTiles;
    delete[] goalTiles;
    fin.close();
}   /* -----  end of function Input  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  Output
 *  Description:  Write to output file
 * =====================================================================================
 */
void Output(char* filename, int optimalCost, int generatedStates)
{
	ofstream fout(filename);
	fout << optimalCost << " " << generatedStates << endl;
	fout.close();
}	/* -----  end of function Output  ----- */

int main()
{
    State goalState;
    Node  initNode;
    int result;

    Input("nPuzzle.inp", goalState, initNode);
    AStarSearch as = AStarSearch(goalState, initNode);
    result = as.Solve();
    Output("nPuzzle.out", result, g_states);

    return 0;
}
