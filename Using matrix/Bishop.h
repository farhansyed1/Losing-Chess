/*
* Losing Chess using matrix
* 
* Author: Farhan Syed
* Year: 2024

  Bishop header file
*/
#ifndef BISHOP_H
#define BISHOP_H

#include "ChessPiece.h"

class Bishop : virtual public ChessPiece {
    // Override virtual methods from ChessPiece here
    public:
        using ChessPiece::ChessPiece; 
        
        virtual int validMove(int to_x, int to_y) override;
        virtual char32_t utfRepresentation() override;     
        virtual char latin1Representation() override;
};


#endif //BISHOP_H
