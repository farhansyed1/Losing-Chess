/*
* Losing Chess using matrix
* 
* Author: Farhan Syed
* Year: 2024

  Implementation of Rook
*/

#include "Rook.h"

#include <cmath>

using namespace std;


/**
 * Returns 0 if target square is unreachable.
 * Returns 1 if target square is reachable and empty.
 * Returns 2 if move captures a piece.
 */
int Rook::validMove(int to_x, int to_y) {
    if (m_x == to_x && m_y == to_y) { // Moving to current square is invalid
        return 0; 
    }
    
    int dx = abs(m_x - to_x);
    int dy = abs(m_y - to_y);

    if (dx != 0 && dy != 0) { // Move must be either in x or y direction, not both 
        return 0; 
    }

    int directionX = 0;
    int directionY = 0;

    if (dx == 0) { // Vertical move
        directionY = (to_y > m_y) ? 1 : -1; 
    } else { // Horizontal move
        directionX = (to_x > m_x) ? 1 : -1;
    }

    int steps = max(dx, dy); 

    // Check for blocking pieces along the path
    for (int i = 1; i < steps; i++) {       // Check each square along the path except the destination square
        shared_ptr<ChessPiece> pathSquare = m_board->getChessBoard()(m_x + directionX * i, m_y + directionY * i);
        if (pathSquare != nullptr) {
            return 0; // Blocked by another piece
        }
    }

    // Check the final destination square
    shared_ptr<ChessPiece> newSquare = m_board->getChessBoard()(to_x, to_y);
    if (newSquare == nullptr) { // Non capturing. Empty square
        return 1; 
    } else if (newSquare->pieceIsWhite() != m_is_white) { // Capturing opponent's piece
        return 2; 
    }
    return 0; // Trying to capture piece of same colour
}

char32_t Rook::utfRepresentation(){
   if(m_is_white) return U'\u2656'; 
   return U'\u265C';
}     
char Rook::latin1Representation(){
   if(m_is_white) return 'R'; 
   return 'r';
}