#ifndef CHESS_H
#define CHESS_H

#include "../core.h"

typedef enum{
    PAWN = 0,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING,
    NON
} PieceType;

typedef enum{
    WHITE_TEAM = 0,
    BLACK_TEAM = 1
} TeamType;

typedef enum{
    WPAWN_TEXTURE = 0,
    WROOK_TEXTURE,
    WKNIGHT_TEXTURE,
    WBISHOP_TEXTURE,
    WQUEEN_TEXTURE,
    WKING_TEXTURE,

    BPAWN_TEXTURE,
    BROOK_TEXTURE,
    BKNIGHT_TEXTURE,
    BBISHOP_TEXTURE,
    BQUEEN_TEXTURE,
    BKING_TEXTURE
} ChessPieceTextureIndex;


typedef struct{
    PieceType type;
    Vec2d pos;
    int team;
    Vec2d* steps;
    int countSteps;
    int moved;
} ChessPiece;


typedef struct{
    ChessPiece** pieces;
    int pcount;
    int id;
    int score;
} ChessTeam;


typedef struct
{
    int pSide;
    int rows;
    int cols;
    int partCount;
    ChessPiece** pMap;
    Window* win;
} ChessSimulator;

#define TILE_AT_USE(s, x, y) s->pMap[y][x].type != NON
#define PIECE_TEAM_IS(s, x, y, t) s->pMap[y][x].team == t


int InitChessSimulator(ChessSimulator** csim, Window* win, int w, int h, int ps);
void DestroyChessSimulator(ChessSimulator** csim);
void AssignTileColors(Color tileWhite, Color tileBlack, Color pieceStep);

void NewGameChessTable(ChessSimulator* csim, int forTeam);
void InitChessTable(ChessSimulator* csim);
void ClearChessTable(ChessSimulator* csim);
void CreateChessPiece(ChessSimulator* csim, int px, int py, PieceType type, int team);
void DeleteChessPiece(ChessSimulator* csim, int px, int py);

void DrawChessTable(ChessSimulator* csim);
void DrawChessPieceSteps(ChessSimulator* csim, int px, int py);
void DrawChessPieces(ChessSimulator* csim);

void CalcPawnMoves(ChessSimulator* csim, ChessPiece* p);
void CalcRookMoves(ChessSimulator* csim, ChessPiece* p);
void CalcKnightMoves(ChessSimulator* csim, ChessPiece* p);
void CalcBishopMoves(ChessSimulator* csim, ChessPiece* p);
void CalcQueenMoves(ChessSimulator* csim, ChessPiece* p);
void CalcKingMoves(ChessSimulator* csim, ChessPiece* p);
void CalcPieceMoves(ChessSimulator* csim, int px, int py);
void CalcAllMoves(ChessSimulator* csim);

void ProcessChessInputs(ChessSimulator* csim, SDL_Event event);
void ChessGame(ChessSimulator* csim);

#endif