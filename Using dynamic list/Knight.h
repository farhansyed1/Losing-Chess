/*
* Losing Chess using a list
* 
* Author: Farhan Syed
* Year: 2024

  Knight header file
*/

#ifndef KNIGHT_H
#define KNIGHT_H

#include "ChessPiece.h"

class Knight : public ChessPiece {
    // Override virtual methods from ChessPiece here
    public:
        using ChessPiece::ChessPiece; 
        
        virtual int validMove(int to_x, int to_y) override;
        virtual char32_t utfRepresentation() override;     
        virtual char latin1Representation() override;
};


#endif //KNIGHT_H
