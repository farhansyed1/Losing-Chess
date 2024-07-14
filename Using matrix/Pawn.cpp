/*
* Losing Chess using matrix
* 
* Author: Farhan Syed
* Year: 2024

  Implementation of Pawn
*/

#include "Pawn.h"

#include <cmath>

using namespace std;

/**
 * Returns 0 if target square is unreachable.
 * Returns 1 if target square is reachable and empty.
 * Returns 2 if move captures a piece.
 */
// NOTE: x represents row, y represents column. 
int Pawn::validMove(int to_x, int to_y){
    shared_ptr<ChessPiece> newSquare = m_board->getChessBoard()(to_x, to_y);

    int direction = m_is_white ? -1 : 1; // white moves up (-), black moves down (+)
    
    //NON CAPTURING
    // Pawn moving forward 1 step to an empty square
    if (m_x + direction == to_x && to_y == m_y && newSquare == nullptr) {  // Checks: One step forward, no movement sideways, empty square
        return 1;
    }
    // Pawn moving forward 2 steps from starting position, to an empty square
    else if ((!m_is_white && m_x == 1 && to_x == 3) || (m_is_white && m_x == 6 && to_x == 4)) { // Checks: 2 steps forward from starting position
        // Check both squares are empty for the double step
        if (to_y == m_y && m_board->getChessBoard()(m_x + direction, m_y) == nullptr && newSquare == nullptr) { // Checks: no movement sideways, 1st square empty, 2nd square empty
            return 1;
        }
    }
    // CAPTURING
    // Pawn capturing diagonally
    else if (abs(m_y - to_y) == 1 && m_x + direction == to_x) { // Checks: sideways 1 step (left or right in column), forward 1 step (up or down in row). This becomes diagonal capture
        if (newSquare != nullptr && newSquare->pieceIsWhite() != m_is_white) { // Checks: capture a piece of opposing colour
            return 2;
        }
        return 0; // Trying to capture piece of same colour
    }
    return 0;
}

char32_t Pawn::utfRepresentation(){
   if(m_is_white) return U'\u2659'; 
   return U'\u265F';
}     
char Pawn::latin1Representation(){
   if(m_is_white) return 'P'; 
   return 'p';
}