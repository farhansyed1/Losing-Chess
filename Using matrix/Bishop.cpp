/*
* Losing Chess using matrix
* 
* Author: Farhan Syed
* Year: 2024

  Implementation of Bishop
*/

#include "Bishop.h"
#include <cmath>

using namespace std;

/**
 * Returns 0 if target square is unreachable.
 * Returns 1 if target square is reachable and empty.
 * Returns 2 if move captures a piece.
 */
int Bishop::validMove(int to_x, int to_y) {
    if (m_x == to_x && m_y == to_y) { // Moving to current square
        return 0; 
    }
    
    int dx = abs(m_x - to_x);
    int dy = abs(m_y - to_y);

    if (dx != dy) { // To be valid diagonal move, dx must equal dy
        return 0; 
    }

    // Check for blocking pieces along the diagonal path
    int directionX = (to_x - m_x) / dx;         // Direction on x-axis is -1 or 1
    int directionY = (to_y - m_y) / dy;         // Direction on y-axis is -1 or 1
    for (int i = 1; i < dx; i++) { // Check each square along the path
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

char32_t Bishop::utfRepresentation(){
   if(m_is_white) return U'\u2657'; 
   return U'\u265D';
}     
char Bishop::latin1Representation(){
   if(m_is_white) return 'B'; 
   return 'b';
}