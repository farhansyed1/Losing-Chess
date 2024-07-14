/*
* Losing Chess using matrix
* 
* Author: Farhan Syed
* Year: 2024

  Queen header file
*/

#ifndef QUEEN_H
#define QUEEN_H

#include "ChessPiece.h"
#include "Bishop.h"
#include "Rook.h"

class Queen :  public Bishop, public Rook { // Diamond inheritance
    public:      
        virtual int validMove(int to_x, int to_y) override;
        virtual char32_t utfRepresentation() override;     
        virtual char latin1Representation() override;

        Queen(int x, int y, bool is_white, ChessBoard * board);
};

#endif //QUEEN_H
