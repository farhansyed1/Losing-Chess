/*
* Losing Chess using a list
* 
* Author: Farhan Syed
* Year: 2024

  King header file
*/

#ifndef KING_H
#define KING_H

#include "ChessPiece.h"

class King : public ChessPiece {
    // Override virtual methods from ChessPiece here
    
    public:
        using ChessPiece::ChessPiece; 
        
        virtual int validMove(int to_x, int to_y) override;
        virtual char32_t utfRepresentation() override;     
        virtual char latin1Representation() override;

};


#endif //KING_H
