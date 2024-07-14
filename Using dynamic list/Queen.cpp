/*
* Losing Chess using a list
* 
* Author: Farhan Syed
* Year: 2024

  Implementation of Queen piece
*/
#include "Queen.h"

#include <cmath>

using namespace std;

/**
 * Returns 0 if target square is unreachable.
 * Returns 1 if target square is reachable and empty.
 * Returns 2 if move captures a piece.
 */

// Queen constructor using virtual inheritance
Queen::Queen(int x, int y, bool is_white, ChessBoard* board) : ChessPiece(x, y, is_white, board), Rook(x, y, is_white, board), Bishop(x, y, is_white, board) {}

int Queen::validMove(int to_x, int to_y) {
   int rookValidMove = Rook::validMove(to_x, to_y);    
   int bishopValidMove = Bishop::validMove(to_x, to_y);    

   return ((bishopValidMove >= rookValidMove) ? bishopValidMove : rookValidMove);  // best possible valid move is from either bishop or rook moves 
}

char32_t Queen::utfRepresentation(){
   if(m_is_white) return U'\u2655'; 
   return U'\u265B';
}     
char Queen::latin1Representation(){
   if(m_is_white) return 'Q'; 
   return 'q';
}