/*
* Losing Chess using matrix
* 
* Author: Farhan Syed
* Year: 2024

  Implementation of King piece
*/

#include "King.h"
#include <cmath>

using namespace std;

/**
 * Returns 0 if target square is unreachable.
 * Returns 1 if target square is reachable and empty.
 * Returns 2 if move captures a piece.
 */
int King::validMove(int to_x, int to_y){
    int dx = abs(m_x - to_x);
    int dy = abs(m_y - to_y); 

    // Verifications to see whether King move is valid
    if ((dx * dy == 1) || (dx + dy == 1)) {         // one step in any direction
        shared_ptr<ChessPiece> newSquare = m_board->getChessBoard()(to_x, to_y);

        if (newSquare == nullptr) { // Move is to empty square
            return 1; 
        }
        else if (newSquare->pieceIsWhite() != m_is_white) { // Move captures an opposing piece
            return 2; 
        }
        return 0; // Trying to capture piece of same colour
    }
    return 0; 
}

char32_t King::utfRepresentation(){
   if(m_is_white) return U'\u2654'; 
   return U'\u265A';

}     
char King::latin1Representation(){
   if(m_is_white) return 'K'; 
   return 'k';
}
