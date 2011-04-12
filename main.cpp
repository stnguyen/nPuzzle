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

using namespace std;

class State {
  private:
    vector<int> m_Tiles;

  public:
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  IsSameState
     *  Description:  Check to see if the passed in state is the same as this state
     * =====================================================================================
     */
    bool
      IsSameState ( State* state )
      {
        // std::vector can be deeply compare by using ==
        return this->m_Tiles == state->m_Tiles;
      }		/* -----  end of function IsSameState  ----- */
};

int main()
{
    return 0;
}
