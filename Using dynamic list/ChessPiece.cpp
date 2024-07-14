/*
* Losing Chess using a list
* 
* Author: Farhan Syed
* Year: 2024

  Implementation of Chess piece
*/

#include "ChessPiece.h"
#include "ChessBoard.h"

// Constructor for a chess piece
ChessPiece::ChessPiece(int x, int y, bool is_white, ChessBoard *board) : m_x(x), m_y(y), m_is_white(is_white), m_board(board) {}

// Virtual functions that are implemented by specific chess pieces
int ChessPiece::validMove(int to_x, int to_y) {
    return 0;
}

char32_t ChessPiece::utfRepresentation() {
    return U' ';
}

char ChessPiece::latin1Representation() {
    return ' ';
}

// Check if a move is a capturing move. Non virtual (same for all implementations)
bool ChessPiece::capturingMove(int to_x, int to_y) {
    return (validMove(to_x, to_y) == 2); // validMove returns 2 if move captures a piece.
}

// Check if a move is a non capturing move. Non virtual (same for all implementations)
bool ChessPiece::nonCapturingMove(int to_x, int to_y) {
    return (validMove(to_x, to_y) == 1); // validMove returns 1 if target square is reachable and empty.
}

// Get vector of all capturing moves. Non virtual brute force method that is used for all chess pieces
vector<ChessMove> ChessPiece::capturingMoves() {
    vector<ChessMove> vecCapMoves;
    for (int x = 0; x < 8; x++) { // Loop over all squares on the chessboard 
        for (int y = 0; y < 8; y++) {
            if (capturingMove(x, y)) { // Check for a capturing move to each square
                ChessMove capMove{m_x, m_y, x, y, this}; // Create as ChessMove and store in vector
                vecCapMoves.push_back(capMove);
            }
        }
    }
    return vecCapMoves;
}

// Get vector of all non capturing moves. Non virtual brute force method that is used for all chess pieces
vector<ChessMove> ChessPiece::nonCapturingMoves() {
    vector<ChessMove> vecNonCapMoves;
    for (int x = 0; x < 8; x++) { // Loop over all squares on the chessboard 
        for (int y = 0; y < 8; y++) {
            if (nonCapturingMove(x, y)) { // Check for a non capturing move to each square
                ChessMove nonCapMove{m_x, m_y, x, y, this}; // Create as ChessMove and store in vector
                vecNonCapMoves.push_back(nonCapMove);
            }
        }
    }
    return vecNonCapMoves;
}
