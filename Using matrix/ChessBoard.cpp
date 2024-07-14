/*
* Losing Chess using matrix
* 
* Author: Farhan Syed
* Year: 2024

  Implementation of Chessboard
*/
#include <random>
#include <sstream>
#include <memory>

#include "ChessBoard.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"

using namespace std;

// Given a valid move, the piece is moved from one square to another 
void ChessBoard::movePiece(ChessMove chess_move) {
    shared_ptr<ChessPiece> &originalSquare = m_state(chess_move.from_x, chess_move.from_y);
    shared_ptr<ChessPiece> &newSquare = m_state(chess_move.to_x, chess_move.to_y);

    // If piece exists on new square, remove the captured piece from its vector
    if (newSquare != nullptr) {
        vector<ChessPiece*> &whiteOrBlackPieces = newSquare->m_is_white ? m_white_pieces : m_black_pieces;
        whiteOrBlackPieces.erase(remove(whiteOrBlackPieces.begin(), whiteOrBlackPieces.end(), newSquare.get()), whiteOrBlackPieces.end());
    }

    newSquare = move(originalSquare); // transfer ownership of the shared pointer pointing to piece. 
    newSquare->m_x = chess_move.to_x; // move the piece from original to new square
    newSquare->m_y = chess_move.to_y;
}

// Get vector of all capturing moves for a given colour
vector<ChessMove> ChessBoard::capturingMoves(bool is_white) {
    vector<ChessMove> vecCapMoves;
    vector<ChessPiece*> whiteOrBlackPieces = is_white ? m_white_pieces : m_black_pieces;

    for (auto& piece : whiteOrBlackPieces) {        // Go through all pieces of given colour and get their capturing moves
        vector<ChessMove> pieceMoves = piece->capturingMoves();         // capturingMoves() is from the ChessPiece class
        vecCapMoves.insert(vecCapMoves.end(), pieceMoves.begin(), pieceMoves.end()); // insert the vector for the piece into vector for whole board
    }
    
    return vecCapMoves;
}

// Get vector of all non capturing moves for a given colour
vector<ChessMove> ChessBoard::nonCapturingMoves(bool is_white) {
    vector<ChessMove> vecNonCapMoves;
    vector<ChessPiece*> whiteOrBlackPieces = is_white ? m_white_pieces : m_black_pieces;

    for (auto& piece : whiteOrBlackPieces) {            // Go through all pieces of given colour and get their non capturing moves
        vector<ChessMove> pieceMoves = piece->nonCapturingMoves();         // nonCapturingMoves() is from the ChessPiece class
        vecNonCapMoves.insert(vecNonCapMoves.end(), pieceMoves.begin(), pieceMoves.end()); // insert the vector for the piece into vector for whole board
    }

    return vecNonCapMoves;
}

// Helper method input stream. Used for creating shared pointers based on given character and its position.  
void ChessBoard::createBoard(int x, int y, char pieceAsChar, ChessBoard &cb) {
    bool isWhite = isupper(pieceAsChar); // True if the piece is white, false if black

    shared_ptr<ChessPiece> newPiece;
    switch (tolower(pieceAsChar)) {
        case 'k':
            newPiece = make_shared<King>(x, y, isWhite, &cb);
            break;
        case 'q':
            newPiece = make_shared<Queen>(x, y, isWhite, &cb);
            break;
        case 'r':
            newPiece = make_shared<Rook>(x, y, isWhite, &cb);
            break;
        case 'n':
            newPiece = make_shared<Knight>(x, y, isWhite, &cb);
            break;
        case 'b':
            newPiece = make_shared<Bishop>(x, y, isWhite, &cb);
            break;
        case 'p':
            newPiece = make_shared<Pawn>(x, y, isWhite, &cb);
            break;
        default:
            throw invalid_argument("Unidentified character!");
    }

    cb.getChessBoard()(x, y) = newPiece; // Pointer given a posiiton on the board
    
    if (isWhite) { // Add piece to vector of given colour 
        cb.getWhitePieces().push_back(newPiece.get());  
    } else {
        cb.getBlackPieces().push_back(newPiece.get()); 
    }
}

// Input operator 
ChessBoard & operator>>(istream &is, ChessBoard &cb) {
    string line;

    // Remove a newline at start in order to integrate with output operator
    char nextChar = is.peek();
    if (nextChar == '\n') {
        getline(is, line); 
    }

    for (int x = 0; x < 8; x++) { // going through each row of lines
        getline(is, line); // read whole line
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        line.erase(remove(line.begin(), line.end(), '\n'), line.end());

        if (line.length() < 8) {
            cerr << "Invalid board!" << endl;
            break;
        }
        for (int y = 0; y < 8; y++) { // going through a column of a line
            if (line[y] != '.') {
                cb.createBoard(x, y, line[y], cb); 
            }
        }
    }
    return cb;
}

// Output operator
ChessBoard & operator<<(ostream &os, ChessBoard &cb) {
    os << "\n";
    Matrix<shared_ptr<ChessPiece>> boardAsMatrix = cb.getChessBoard(); 
    for (int row = 0; row < 8; row++) { // going through each row of pieces
        for (int col = 0; col < 8; col++) { // going through each column of a row
            if (boardAsMatrix(row, col) != nullptr) { // if piece exists, get its character representation
                os << boardAsMatrix(row, col)->getLatin1Representation();
            } else {
                os << ".";
            }
        }
        os << "\n";
    }
    os << "\n";
    return cb;
}

// Helper method used for pawn promotion. Creates a shared pointer based on given piece and colour
shared_ptr<ChessPiece> createPiece(int pieceType, int x, int y, bool is_white, ChessBoard* cb) {
    if (pieceType == 0) return make_shared<Knight>(x, y, is_white, cb);
    if (pieceType == 1) return make_shared<Bishop>(x, y, is_white, cb);
    if (pieceType == 2) return make_shared<Rook>(x, y, is_white, cb);
    if (pieceType == 3) return make_shared<Queen>(x, y, is_white, cb);
    return nullptr;
}

// Helper method for smart AI's pawn promotion. Checks if the piece on a given square has no capturing moves
bool ChessBoard::noCapturingMovesForSquare(int x, int y, bool is_white){
    if((this -> getChessBoard()(x,y)->capturingMoves()).empty()){ 
        return true;
    }
    return false;
}

// Helper method for pawn promotion. Selects a piece to promote to based on the AI's smartness and then promotes the pawn.    
bool ChessBoard::promotePawn(int x, int y, ChessMove move, bool is_white, bool is_smart){
    
    // The smart AI promotes to a piece that has no capturing moves 
    if(is_smart){
        this -> getChessBoard()(x,y) = createPiece(0,x,y,is_white,this);
        if(noCapturingMovesForSquare(x,y,is_white)){
            return true; 
        }

        this -> getChessBoard()(x,y) = createPiece(1,x,y,is_white,this);
        if(noCapturingMovesForSquare(x,y,is_white)){
            return true; 
        }

        this -> getChessBoard()(x,y) = createPiece(2,x,y,is_white,this);
        if(noCapturingMovesForSquare(x,y,is_white)){
            return true; 
        }
    }
    // Random AI randomly promotes. The smart AI also randomly promotes if all the above pieces had capturing moves. 
    int newPiece = rand() % 4; 
    this -> getChessBoard()(x,y) = createPiece(newPiece, x, y, is_white, this);
    return true;  
}

// Checks if pawn promotion is available and then uses helper methods to find best piece to promote to
bool ChessBoard::checkPawnPromotion(ChessMove move, bool is_white, bool is_smart){
    int lastRow = is_white ? 0 : 7;
    char pieceType = tolower(move.piece->getLatin1Representation());

    if(pieceType == 'p' && move.to_x == lastRow){ // The piece is a pawn that has reached the last row
        promotePawn(move.to_x,move.to_y,move,is_white, is_smart);   // Promote the pawn
        
        // Replace the pawn with the new piece
        vector<ChessPiece*> &pieces = is_white ? this->m_white_pieces : this->m_black_pieces;
        
        auto iter = find(pieces.begin(), pieces.end(), move.piece); // Iterate through pieces vector of given colour
        if (iter != pieces.end()) {
            *iter = this->getChessBoard()(move.to_x, move.to_y).get(); // Find the correct square and replace the pawn with the new piece
        }
        return true;
    } 
    else {
        return false;
    }
}

// Helper method for checking for forced capturing moves. Creates a copy of board by utilising the input and output streams. 
unique_ptr<ChessBoard> copyChessBoard(ChessBoard& originalBoard) {
    
    stringstream ss;
    ss << originalBoard; // Send board to stream

    unique_ptr<ChessBoard> copiedBoard = make_unique<ChessBoard>(); 
    ss >> *copiedBoard; // Create new board (createBoard() is called)

    return copiedBoard;
}

// Checks if the given move forces the opponent to capture the piece. Used by smart AI. 
bool ChessBoard::forcesOpponentCapturingMove(ChessMove move, bool colorOfOpponent) {
    unique_ptr<ChessBoard> copyOfBoard = copyChessBoard(*this); // Create a copy of the current board
    copyOfBoard->movePiece(move);  // Make the move on the copied board

    auto& opponentPieces = colorOfOpponent ? copyOfBoard -> m_white_pieces : copyOfBoard -> m_black_pieces;

    for(int i = 0; i < opponentPieces.size(); i++){
        if(opponentPieces[i] -> capturingMove(move.to_x,move.to_y)){ // The opponent's piece can capture the player's piece if the player makes the given move   
            return true;   
        }
    }

    return false;
}

// AI that solely uses randomness when making its moves. 
bool ChessBoard::randomAI(bool is_white){
    vector<ChessMove> vecNonCapMoves = nonCapturingMoves(is_white);
    vector<ChessMove> vecCapMoves = capturingMoves(is_white);

	srand((unsigned) time(NULL));

    if(!vecCapMoves.empty()){   // There are capturing moves
        int randomIndex = rand() % (vecCapMoves.size());
        movePiece(vecCapMoves[randomIndex]);
        checkPawnPromotion(vecCapMoves[randomIndex], is_white, false);
        return true;
    }
    else if(!vecNonCapMoves.empty()) { // There are only non capturing moves
        int randomIndex = rand() % (vecNonCapMoves.size()); 
        movePiece(vecNonCapMoves[randomIndex]);
        checkPawnPromotion(vecNonCapMoves[randomIndex], is_white, false);
        return true;
    } 

    return false; // No moves possible -> Lose the game
}

// The smart AI. It primarily makes moves that will force the opponent to capture their piece 
bool ChessBoard::smartAI(bool is_white){
    vector<ChessMove> vecNonCapMoves = nonCapturingMoves(is_white);
    vector<ChessMove> vecCapMoves = capturingMoves(is_white);
	srand((unsigned) time(NULL));

    // Find and a make a smart move that forces the opponent to capture. 
    // If capturing moves available, go through them. Otherwise use non capturing moves
    vector<ChessMove>& chosenMoves = !vecCapMoves.empty() ? vecCapMoves : vecNonCapMoves;
    for (auto& move : chosenMoves) {
        if (forcesOpponentCapturingMove(move, !is_white)) {
            movePiece(move);
            checkPawnPromotion(move, is_white, true);
            return true;
        }
    }

    // If no smart moves available, choose a random move
    if (!chosenMoves.empty()) {
        int randomIndex = rand() % chosenMoves.size();
        movePiece(chosenMoves[randomIndex]);
        checkPawnPromotion(chosenMoves[randomIndex], is_white, true);
        return true;
    }

    return false; // No moves possible -> Lose the game
}