/*
* Losing Chess using a list
* 
* Author: Farhan Syed
* Year: 2024

  Implementation of Knight piece
*/

#include "Knight.h"
#include <cmath>

using namespace std;


/**
 * Returns 0 if target square is unreachable.
 * Returns 1 if target square is reachable and empty.
 * Returns 2 if move captures a piece.
 */
int Knight::validMove(int to_x, int to_y){
   int dx = abs(m_x - to_x);
   int dy = abs(m_y - to_y); 

   // Verifications to see whether Knight move is valid
   if ((dx * dx + dy * dy) == 5) { // 2 squares in any vertical/horizontal direction and 1 square in perpendicular direction
       if (m_board->isOccupied(to_x, to_y)) { // Check if the destination square is occupied
           auto piece = m_board->getPieceAt(to_x, to_y);
           if (piece->pieceIsWhite() == this->m_is_white) {
               return 0; // Trying to capture piece of same colour
           } else {
               return 2; // Move captures an opposing piece
           }
       }
       return 1; // Move is to empty square if not occupied
   }
   return 0;
}

char32_t Knight::utfRepresentation(){
   if(m_is_white) return U'\u2658'; 
   return U'\u265E';

}     
char Knight::latin1Representation(){
   if(m_is_white) return 'N'; 
   return 'n';
}
