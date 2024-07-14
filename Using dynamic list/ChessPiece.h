/*
* Losing Chess using a list
* 
* Author: Farhan Syed
* Year: 2024

  Chess piece header file
*/


#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <vector>
#include "ChessMove.h"
#include "ChessBoard.h"

using namespace std;

class ChessPiece {
    friend void ChessBoard::movePiece(ChessMove p);
protected:                               // protected will cause problems
    int m_x, m_y;
    bool m_is_white;
    ChessBoard* m_board;
    /**
     * Returns 0 if target square is unreachable.
     * Returns 1 if target square is reachable and empty.
     * Returns 2 if move captures a piece.
     */
    virtual int validMove(int to_x, int to_y);
    virtual char32_t utfRepresentation();     // may be implemented as string
    virtual char latin1Representation();
public:
    // Constructor
    ChessPiece(int x, int y, bool is_white, ChessBoard * board);
    /**
     * Checks if this move is valid for this piece and captures
     * a piece of the opposite color.
     */
    bool capturingMove(int to_x, int to_y);
    /**
     * Checks if this move is valid but does not capture a piece.
     */
    bool nonCapturingMove(int to_x, int to_y);
    vector<ChessMove> capturingMoves();
    vector<ChessMove> nonCapturingMoves();

    /**
    * For testing multiple inheritance
    */
    int unnecessary_int;

    int getX() const { return m_x; }

    int getY() const { return m_y; }

    void setPosition(int x, int y) {
        m_x = x;
        m_y = y;
    }

    bool pieceIsWhite() { return m_is_white; }
    char getLatin1Representation() {
        return latin1Representation();
    }
};


#endif //CHESSPIECE_H
