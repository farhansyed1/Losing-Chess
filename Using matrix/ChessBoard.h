/*
* Losing Chess using matrix
* 
* Author: Farhan Syed
* Year: 2024

  Chessboard header file
*/
#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <istream>
#include <memory>
#include "ChessMove.h"
#include "Matrix.h"   

using namespace std;

class ChessPiece;

class ChessBoard {

private:
    // The matrix owns the chess pieces:
    Matrix<shared_ptr<ChessPiece>> m_state{8}; // Create 8 x 8 chessboard using matrix class
    vector<ChessPiece *> m_white_pieces;
    vector<ChessPiece *> m_black_pieces;

    // Alternative 2 (the vectors own the chess pieces):
    // Matrix<ChessPiece *> m_state; 
    // vector<shared_ptr<ChessPiece>> m_white_pieces;
    // vector<shared_ptr<ChessPiece>> m_black_pieces;

public:
    // Public getters
    Matrix<shared_ptr<ChessPiece>> & getChessBoard(){ 
        return m_state; 
    }
    vector<ChessPiece *>& getWhitePieces(){
        return m_white_pieces;
    };
    vector<ChessPiece *>& getBlackPieces(){
        return m_black_pieces;
    };

    
    void movePiece(ChessMove chess_move);
    vector<ChessMove> capturingMoves(bool is_white);
    vector<ChessMove> nonCapturingMoves(bool is_white);
    void createBoard(int x, int y, char pieceAsChar,ChessBoard &cb);

    bool randomAI(bool is_white);
    bool smartAI(bool is_white);
    bool checkPawnPromotion(ChessMove move, bool is_white, bool is_smart);
    bool promotePawn(int x, int y, ChessMove move, bool is_white, bool is_smart);
    
    bool noCapturingMovesForSquare(int x,int y, bool is_white);
    bool forcesOpponentCapturingMove(ChessMove move, bool colorOfOpponent);
};

ChessBoard & operator>>(istream & is, ChessBoard & cb);
ChessBoard & operator<<(ostream & os, ChessBoard & cb);

#endif //CHESSBOARD_H
