/*
* Losing Chess using matrix
* 
* Author: Farhan Syed
* Year: 2024

  Main file
*/
#include "ChessBoard.h"
#include <iostream> 
#include <sstream>  
#include <vector>   

using namespace std;

// Compiling:         g++ -o main.exe main.cpp ChessBoard.cpp ChessPiece.cpp King.cpp Queen.cpp Rook.cpp Bishop.cpp Knight.cpp Pawn.cpp
// Testing for leaks: valgrind --leak-check=full --show-leak-kinds=all ./main.exe

// Checks if the line consists of exactly 8 correct characters
bool checkValidLine(string& line) {
    if (line.length() != 8) return false; 
    vector<char> validChars{'r', 'n', 'b', 'q', 'k', 'p', '.'}; 
    
    for (char c : line) {
        if (find(validChars.begin(), validChars.end(), tolower(c)) == validChars.end()) { // check if character is not valid
            return false; 
        }
    }
    return true; // All characters are valid
}

int main() {
    cout << "Welcome to losing chess with AI! \n";
    cout << "Select board option:  \n";
    cout << "1: Use default chess board \n";
    cout << "2: Input own starting board \n";
    cout << "Select 1 or 2: \n";

    int defaultBoard;
    
    // 1. Input type of board. 1 = default, 2 = own input
    while(true){
        if (!(cin >> defaultBoard) || (defaultBoard != 1 && defaultBoard != 2)) { // reading number fails 
            cout << "Incorrect input. Please enter 1 or 2. \n";
            cin.clear();            
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue; 
        }
        else{   // 1 or 2 is correctly inputted
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            break;
        } 
    }

    // 2. Create the chessboard
    ChessBoard cb;

    if(defaultBoard == 1){
        stringstream s;
        s << "rnbqkbnr" << endl;
        s << "pppppppp" << endl;
        s << "........" << endl;
        s << "........" << endl;
        s << "........" << endl;
        s << "........" << endl;
        s << "PPPPPPPP" << endl;
        s << "RNBQKBNR";
        s >> cb; 
    } else {
        // If own input, repeat loop until correct board is given
        bool validBoard = false;
        while (!validBoard) {
            vector<string> boardLines;
            cout << "Input the entire board with a new line after each row:" << endl;

            for (int i = 0; i < 8; i++) {
                string line;
                getline(cin, line);
                if (!checkValidLine(line)) { // if line is invalid, restart input of board
                    cout << " Line " << i+1 << " is invalid. Please start over.\n";
                    boardLines.clear(); 
                    break; 
                }
                boardLines.push_back(line);
            }
            // All 8 lines have been read correctly
            if (boardLines.size() == 8) {
                validBoard = true; 
            }
            // Send to input stream
            if (validBoard) {
                stringstream s;
                for(auto line : boardLines) {
                    s << line << "\n";
                }
                s >> cb;
            }
        }
    }
    
    cout << "Board has been read! \n";
    
    // 3. Select starting colour
    cout << "Select colour for player 1: w or b? \n";

    char startingColour; 
    while(true){
        cin >> startingColour;
        if(tolower(startingColour) == 'w' || tolower(startingColour) == 'b') { 
            break;
        } else {
            cout << "Incorrect input. Please enter 'w' for white or 'b' for black. \n";
        }
    }
    cout << "\n";
    
    // 4. Select types of AI. 0 = random, 1 = smart
    cout << "Time to select the AI players. There are two types of AI: \n";
    cout << "- AI 0: Random thinker \n";
    cout << "- AI 1: Thinks one step ahead \n";

    bool playerOneType;
    while(true){
        cout << "Select AI type for player 1: 0 or 1? \n";
        cin >> playerOneType;
        if(playerOneType == 0 || playerOneType == 1){
            break; 
        }
        else {
            cout << "Incorrect input. Please enter 0 or 1. \n ";
        }
    }

    bool playerTwoType;
    while(true){
        cout << "Select AI type for player 2: 0 or 1? \n";
        cin >> playerTwoType;
        if(playerTwoType == 0 || playerTwoType == 1){
            break; 
        }
        else {
            cout << "Incorrect input. Please enter 0 or 1. \n ";
        }
    }
    // 5. Play the game
    bool player1Colour = (startingColour == 'w') ? true : false;
    bool player2Colour = !player1Colour;

    bool player1Turn = player1Colour;

    cout << "Time to play! \n";

    while(true){
        if(player1Turn){
            bool player1Lose = (playerOneType == 1) ?  cb.smartAI(player1Colour) : cb.randomAI(player1Colour);

            if(!player1Lose){
                cout << "\n Player 1 won!\n";
                break;
            } else{
                cout << "Player 1's turn: \n";
            }
            cout << cb;
            player1Turn = !player1Turn;
        } else{
            bool player2Lose = (playerTwoType == 1) ?  cb.smartAI(player2Colour) : cb.randomAI(player2Colour);
            if(!player2Lose){
                cout << "\n Player 2 won!\n";
                break;
            } else{
              cout << "Player 2's turn: \n";

            }
            cout << cb;
            player1Turn = !player1Turn;
        }
    }    
}