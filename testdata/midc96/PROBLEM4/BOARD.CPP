#include <iostream.h>
#include <fstream.h>
#include <math.h>


#define INBOARD(r,c) ((r>=0)&&(r<=7)&&(c>=0)&&(c<=7))
struct Move
    {
    int frR, frC, toR, toC;
    };
char board[8][8];
Move moves[96];
int numMoves;
char player;

void Input (ifstream&);
void Output (ofstream&);
int IsLegalMove ();
void GetLegalMoves (int, int);
void GetLegalMoves ();



void main ()
    {
    ifstream infile ("board.in");
    ofstream outfile ("board.out");
    while (!infile.eof())
        {
        Input (infile);
        if (!infile.eof())
            {
            numMoves = 0;
            GetLegalMoves ();
            Output (outfile);
            }
        }
    }


void Input ( ifstream& infile )
    {
    infile.flags (ios::skipws);
    for (int i = 0; i < 8; i++)
        {
        for (int j = 0; j < 8; j++)
            {
            infile >> board[i][j];
            if (infile.eof()) return;
            }
        }
    infile >> player;
    }


void Output ( ofstream& outfile )
    {
    if (numMoves)
        {
        for (int i = 0; i < numMoves; i++)
            {
            outfile << (char)(moves[i].frR + (int)'A') << (char)(moves[i].frC + (int)'1') << '-'
                    << (char)(moves[i].toR + (int)'A') << (char)(moves[i].toC + (int)'1') << endl;
            }
        }
    else
        outfile << "No moves are possible" << endl;
    outfile << endl;
    }



int IsLegalMove (Move move)
    {
    char thisPiece, otherPiece;
    thisPiece = player;
    if (thisPiece == 'X')
        otherPiece = 'O';
    else
        otherPiece = 'X';

    // Test for valid coordinates
    if (!INBOARD(move.frR,move.frC) || !INBOARD(move.toR,move.toC))
        return 0;
    
    // Test for blocks
    int rowChg, colChg;
    int rowCur, colCur;
    rowChg = (move.toR - move.frR);
    if (rowChg) rowChg /= abs (rowChg);
    colChg = (move.toC - move.frC);
    if (colChg) colChg /= abs (colChg);
    for (rowCur = move.frR, colCur = move.frC;
         (rowCur != move.toR) || (colCur != move.toC);
         rowCur+=rowChg, colCur+=colChg)
        if ((board[rowCur][colCur] == otherPiece) && ((rowCur != move.toR) || (colCur != move.toC)))
            return 0;

    // Test for right distance
    int distance, piececount = 0;
    int rowDiff, colDiff;
    for (rowCur = move.frR, colCur = move.frC;
         INBOARD(rowCur-rowChg,colCur-colChg);
         rowCur -= rowChg, colCur -= colChg);
    for (;
         INBOARD(rowCur,colCur);
         rowCur += rowChg, colCur += colChg)
        if (board[rowCur][colCur] != '.')
            piececount++;
    rowDiff = abs(move.toR - move.frR);
    colDiff = abs(move.toC - move.frC);
    if (rowDiff)
        distance = rowDiff;
    else
        distance = colDiff;
    if (distance != piececount)
        return 0;

    return 1;
    }




void GetLegalMoves (int row, int col)
    {
    int pieces_n_s, pieces_e_w, pieces_ne_sw, pieces_nw_se;
    int rowCur, colCur;

    pieces_n_s = 0;
    for (rowCur = 0; rowCur < 8; rowCur++)
        if (board[rowCur][col] != '.')
            pieces_n_s++;

    pieces_e_w = 0;
    for (colCur = 0; colCur < 8; colCur++)
        if (board[row][colCur] != '.')
            pieces_e_w++;

    pieces_nw_se = 0;
    for ((row < col) ? (rowCur = 0, colCur = col-row) : (rowCur = row-col, colCur = 0);
         INBOARD(rowCur,colCur);
         rowCur++,colCur++)
        if (board[rowCur][colCur] != '.')
            pieces_nw_se++;

    pieces_ne_sw = 0;
    for ((7-row < col) ? (rowCur = 7, colCur = col-(7-row)) : (rowCur = row+col, colCur = 0);
         INBOARD(rowCur,colCur);
         rowCur--,colCur++)
        if (board[rowCur][colCur] != '.')
            pieces_ne_sw++;
    
    Move move;
    move.frR = row; move.frC = col;

    move.toR = row - pieces_n_s;   move.toC = col;                  // N
    if (IsLegalMove (move)) moves[numMoves++] = move;
    move.toR = row - pieces_ne_sw; move.toC = col + pieces_ne_sw;   // NE
    if (IsLegalMove (move)) moves[numMoves++] = move;
    move.toR = row;                move.toC = col + pieces_e_w;     // E
    if (IsLegalMove (move)) moves[numMoves++] = move;
    move.toR = row + pieces_nw_se; move.toC = col + pieces_nw_se;   // SE
    if (IsLegalMove (move)) moves[numMoves++] = move;
    move.toR = row + pieces_n_s;   move.toC = col;                  // S
    if (IsLegalMove (move)) moves[numMoves++] = move;
    move.toR = row + pieces_ne_sw; move.toC = col - pieces_ne_sw;   // SW
    if (IsLegalMove (move)) moves[numMoves++] = move;
    move.toR = row;                move.toC = col - pieces_e_w;     // W
    if (IsLegalMove (move)) moves[numMoves++] = move;
    move.toR = row - pieces_nw_se; move.toC = col - pieces_nw_se;   // NW
    if (IsLegalMove (move)) moves[numMoves++] = move;
    }



void GetLegalMoves ()
    {
    int rowCur, colCur;
    for (rowCur = 0; rowCur < 8; rowCur++)
        for (colCur = 0; colCur < 8; colCur++)
            if (board[rowCur][colCur] == player)
                GetLegalMoves (rowCur, colCur);
    }


