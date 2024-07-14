/*
* Losing Chess using matrix
* 
* Author: Farhan Syed
* Year: 2024

  Pawn header file
*/

#ifndef PAWN_H
#define PAWN_H

#include "ChessPiece.h"

class Pawn : public ChessPiece {
    public:
        using ChessPiece::ChessPiece; 
        
        virtual int validMove(int to_x, int to_y) override;
        virtual char32_t utfRepresentation() override;     
        virtual char latin1Representation() override;
};

#endif //PAWN_H
