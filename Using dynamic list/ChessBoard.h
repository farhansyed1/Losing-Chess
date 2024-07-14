/*
* Losing Chess using a list
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
    vector<shared_ptr<ChessPiece>> m_pieces; // List containing all pieces
    vector<ChessPiece *> m_white_pieces;
    vector<ChessPiece *> m_black_pieces;

public:
    // Public getters
    vector<shared_ptr<ChessPiece>> & getPieces(){ 
        return m_pieces; 
    }

    const vector<shared_ptr<ChessPiece>>& getPieces() const { 
        return m_pieces; 
    }

    vector<ChessPiece *>& getWhitePieces(){
        return m_white_pieces;
    };
    vector<ChessPiece *>& getBlackPieces(){
        return m_black_pieces;
    };

    void addPiece(const shared_ptr<ChessPiece>& piece) {
        m_pieces.push_back(piece);
    }

    // Member functions
    bool isOccupied(int x, int y) const;
    ChessPiece* getPieceAt(int x, int y) ;
    
    void movePiece(ChessMove chess_move);
    vector<ChessMove> capturingMoves(bool is_white);
    vector<ChessMove> nonCapturingMoves(bool is_white);
    void createBoard(int x, int y, char pieceAsChar,ChessBoard &cb);

    bool randomAI(bool is_white);
    bool smartAI(bool is_white);
    bool checkPawnPromotion(ChessMove move, bool is_white, bool is_smart);
    void replacePieceInList(ChessPiece* oldPiece, shared_ptr<ChessPiece> newPiece);
    bool promotePawn(int x, int y, ChessMove move, bool is_white, bool is_smart);
    
    bool noCapturingMovesForSquare(shared_ptr<ChessPiece> piece);
    bool forcesOpponentCapturingMove(ChessMove move, bool colorOfOpponent);
};

ChessBoard & operator>>(istream & is, ChessBoard & cb);
ChessBoard & operator<<(ostream & os, ChessBoard & cb);

#endif //CHESSBOARD_H
