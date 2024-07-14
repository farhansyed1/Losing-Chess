/*
* Losing Chess using a list
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

// Check if square is occupied by piece
bool ChessBoard::isOccupied(int x, int y) const {
    for (const auto& piece : m_pieces) {  
        if (piece->getX() == x && piece->getY() == y) {
            return true;
        }
    }
    return false;
}

// Get the piece that is occupying a specific square
ChessPiece* ChessBoard::getPieceAt(int x, int y) {
    for (const auto& piece : m_pieces) {
        if (piece->getX() == x && piece->getY() == y) {
            return piece.get(); 
        }
    }
    return nullptr;
}

// Given a valid move, the piece is moved from one square to another 
void ChessBoard::movePiece(ChessMove chess_move) {
    ChessPiece* movingPiece = getPieceAt(chess_move.from_x, chess_move.from_y);
    ChessPiece* targetPiece = getPieceAt(chess_move.to_x, chess_move.to_y);

    // If piece exists on new square, remove the captured piece from its vector
    if (targetPiece != nullptr) {
        // Remove from white/black piece vector
        vector<ChessPiece*> &whiteOrBlackPieces = targetPiece->pieceIsWhite() ? m_white_pieces : m_black_pieces;
        whiteOrBlackPieces.erase(remove(whiteOrBlackPieces.begin(), whiteOrBlackPieces.end(), targetPiece), whiteOrBlackPieces.end());

        // Remove from  main pieces vector
        m_pieces.erase(remove_if(m_pieces.begin(), m_pieces.end(), [&](const shared_ptr<ChessPiece>& piece) { return piece.get() == targetPiece; }), m_pieces.end());
    }

    if (movingPiece) { 
        movingPiece->setPosition(chess_move.to_x, chess_move.to_y); // move the piece from original to new square
    }
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

// Helper method for input stream. Used for creating shared pointers based on given character and its position.  
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

    cb.m_pieces.push_back(newPiece);

    if (isWhite) { // Add piece to vector of given colour 
        cb.m_white_pieces.push_back(newPiece.get());
    } else {
        cb.m_black_pieces.push_back(newPiece.get());
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
    vector<vector<char>> board(8, vector<char>(8, '.')); // Empty 8x8 board

    // Place pieces on the board
    for (const auto& piece : cb.getPieces()) { 
        if (piece) { 
            board[piece->getX()][piece->getY()] = piece->getLatin1Representation(); 
        }
    }

    // Iterata through rows and send to output stream 
    for (const auto& row : board) {
        for (char piece : row) {
            os << piece; // Output each piece
        }
        os << "\n"; // New line after each row
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
bool ChessBoard::noCapturingMovesForSquare(shared_ptr<ChessPiece> piece){
    return piece && piece->capturingMoves().empty();
}

// Replaces an old piece with a new piece in white/black list and general list. Used for pawn promotion 
void ChessBoard::replacePieceInList(ChessPiece* oldPiece, shared_ptr<ChessPiece> newPiece) {
    // Color-specific list
    auto& whiteOrBlackPieces = oldPiece->pieceIsWhite() ? m_white_pieces : m_black_pieces;
    for (auto& piece : whiteOrBlackPieces) {
        if (piece == oldPiece) {    
            piece = newPiece.get();  // Piece is replaced with new piece
            break;  
        }
    }

    // General list
    for (auto& piece : m_pieces) {
        if (piece.get() == oldPiece) {
            piece = newPiece;  // Piece is replaced with the shared pointer
            break;  
        }
    }
}

// Helper method for pawn promotion. Selects a piece to promote to based on the AI's smartness and then promotes the pawn.    
bool ChessBoard::promotePawn(int x, int y, ChessMove move, bool is_white, bool is_smart) {
    // The smart AI promotes to a piece that has no capturing moves 
    if (is_smart) {
        vector<int> pieceTypes = {0, 1, 2, 3}; // Knight, Bishop, Rook, Queen

        // Try to find a piece type that has no capturing moves
        for (int type : pieceTypes) {
            shared_ptr<ChessPiece> newPiece = createPiece(type, x, y, is_white, this);
            if (newPiece && noCapturingMovesForSquare(newPiece)) {
                replacePieceInList(move.piece, newPiece);
                move.piece = newPiece.get();
                cout << "Pawn can promote to " << newPiece->getLatin1Representation() << endl;
                return true;
            }
        }
    }

    // Random AI randomly promotes. The smart AI also randomly promotes if all the above pieces had capturing moves.
    int randomType = rand() % 4; 
    auto newPiece = createPiece(randomType, x, y, is_white, this);
    if (newPiece) {
        replacePieceInList(move.piece, newPiece);
        move.piece = newPiece.get();
        cout << "Pawn can promote to " << newPiece->getLatin1Representation() << endl;
        return true;
    }

    return false;
}

// Checks if pawn promotion is available and then uses helper methods to find best piece to promote to
bool ChessBoard::checkPawnPromotion(ChessMove move, bool is_white, bool is_smart){
    int lastRow = is_white ? 0 : 7;
    char pieceType = tolower(move.piece->getLatin1Representation());

    if (pieceType == 'p' && move.to_x == lastRow) { // The piece is a pawn that has reached the last row
        return promotePawn(move.to_x, move.to_y, move, is_white, is_smart);
    }
    return false; // No promotion available
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