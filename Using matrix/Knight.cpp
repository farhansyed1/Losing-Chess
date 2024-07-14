/*
* Losing Chess using matrix
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
   if ((dx * dx + dy * dy) == 5) { // 2 squares in any vertical/horisontal direction and 1 square in perpendicular direction
      shared_ptr<ChessPiece> newSquare = m_board->getChessBoard()(to_x, to_y);

      // Move captures an opponent's piece or moves to an empty square
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

char32_t Knight::utfRepresentation(){
   if(m_is_white) return U'\u2658'; 
   return U'\u265E';

}     
char Knight::latin1Representation(){
   if(m_is_white) return 'N'; 
   return 'n';
}
