/*
* Losing Chess using a list
* 
* Author: Farhan Syed
* Year: 2024

  Rook header file
*/

#ifndef ROOK_H
#define ROOK_H

#include "ChessPiece.h"

class Rook : virtual public ChessPiece {
    public:
        using ChessPiece::ChessPiece; 
        
        virtual int validMove(int to_x, int to_y) override;
        virtual char32_t utfRepresentation() override;     
        virtual char latin1Representation() override;
};


#endif //ROOK_H
