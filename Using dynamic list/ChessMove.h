/*
* Losing Chess using a list
* 
* Author: Farhan Syed
* Year: 2024

  Chessmove header file
*/

#ifndef CHESSMOVE_H
#define CHESSMOVE_H

using namespace std;

class ChessPiece;

struct ChessMove {
    int from_x;
    int from_y;
    int to_x;
    int to_y;

    ChessPiece * piece;   // change the position of the chess piece with this pointer.
};

#endif //CHESSMOVE_H
