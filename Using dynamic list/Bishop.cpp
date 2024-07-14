/*
* Losing Chess using a list
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
        int squareOnPathX = m_x + directionX * i;
        int squareOnPathY = m_y + directionY * i;
        
        // Check each piece to see if it blocks the path
        if (m_board->isOccupied(squareOnPathX, squareOnPathY)) { 
            return 0;
        }
    }

    // Check the final destination square
    if (m_board->isOccupied(to_x, to_y)) { // Piece on the target square
        auto piece = m_board->getPieceAt(to_x, to_y);
        if (piece->pieceIsWhite() == this->m_is_white) { 
            return 0; // Trying to capture piece of same colour
        } else {
            return 2; // Move captures an opposing piece
        }
    }
    return 1; // Move is to empty square
}

char32_t Bishop::utfRepresentation(){
   if(m_is_white) return U'\u2657'; 
   return U'\u265D';
}     
char Bishop::latin1Representation(){
   if(m_is_white) return 'B'; 
   return 'b';
}