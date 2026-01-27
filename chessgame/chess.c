#include "chess.h"

ChessTeam teams[16];
int teamCount = 0;

Color tileColors[2];
Color stepColor;
Vec2d selectedTile = {-1, -1};

SDL_Texture* pieceTextures[256];
const char* pieceTexturePaths[] = {
    "./chessgame/images/wpawn.bmp",
    "./chessgame/images/wrook.bmp",
    "./chessgame/images/wknight.bmp",
    "./chessgame/images/wbishop.bmp",
    "./chessgame/images/wqueen.bmp",
    "./chessgame/images/wking.bmp",

    "./chessgame/images/bpawn.bmp",
    "./chessgame/images/brook.bmp",
    "./chessgame/images/bknight.bmp",
    "./chessgame/images/bbishop.bmp",
    "./chessgame/images/bqueen.bmp",
    "./chessgame/images/bking.bmp"
};

int pieceCount = 12;


int InitChessSimulator(ChessSimulator** csim, Window* win, int w, int h, int ps){
    *csim = (ChessSimulator*)malloc(sizeof(ChessSimulator));
    (*csim)->pSide = ps;
    (*csim)->rows = h/ps;
    (*csim)->cols = w/ps;
    (*csim)->partCount = 0;
    (*csim)->win = win;
    (*csim)->pMap = (ChessPiece**)Malloc2D((*csim)->rows, (*csim)->cols, sizeof(ChessPiece));
    if((*csim)->pMap == NULL) return 1;
    InitChessTable((*csim));
    return 0;
}

void DestroyChessSimulator(ChessSimulator** csim){
    ClearChessTable(*csim);
    for(int i = 0; i < (*csim)->rows; i++){
        free((*csim)->pMap[i]);
    }
    free((*csim)->pMap);

    free(*csim);
}

void AssignTileColors(Color tileWhite, Color tileBlack, Color pieceStep){
    tileColors[0] = tileWhite;
    tileColors[1] = tileBlack;
    stepColor = pieceStep;
}

void LoadPieceTextures(ChessSimulator* csim){
    for(int i = 0; i < pieceCount; i++){
        SDL_Surface* surf = SDL_LoadBMP(pieceTexturePaths[i]);
        if(surf == NULL) {printf("failed to load the texture\n"); return;}
        SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 255, 0, 255));

        SDL_Texture* text = SDL_CreateTextureFromSurface(csim->win->renderer, surf);
        pieceTextures[i] = text;
        SDL_FreeSurface(surf);
    }
}

void DestroyPieceTextures(ChessSimulator* csim){
    for(int i = 0; i < pieceCount; i++){
        SDL_DestroyTexture(pieceTextures[i]);
    }
}


void NewGameChessTable(ChessSimulator* csim, int forTeam){
    // This team
    CreateChessPiece(csim, 0, 7, ROOK, forTeam);
    CreateChessPiece(csim, 1, 7, KNIGHT, forTeam);
    CreateChessPiece(csim, 2, 7, BISHOP, forTeam);
    CreateChessPiece(csim, 3, 7, QUEEN, forTeam);
    CreateChessPiece(csim, 4, 7, KING, forTeam);
    CreateChessPiece(csim, 5, 7, BISHOP, forTeam);
    CreateChessPiece(csim, 6, 7, KNIGHT, forTeam);
    CreateChessPiece(csim, 7, 7, ROOK, forTeam);
    CreateChessPiece(csim, 0, 6, PAWN, forTeam);
    CreateChessPiece(csim, 1, 6, PAWN, forTeam);
    CreateChessPiece(csim, 2, 6, PAWN, forTeam);
    CreateChessPiece(csim, 3, 6, PAWN, forTeam);
    CreateChessPiece(csim, 4, 6, PAWN, forTeam);
    CreateChessPiece(csim, 5, 6, PAWN, forTeam);
    CreateChessPiece(csim, 6, 6, PAWN, forTeam);
    CreateChessPiece(csim, 7, 6, PAWN, forTeam);

    // Other team
    forTeam++;
    CreateChessPiece(csim, 0, 0, ROOK, forTeam);
    CreateChessPiece(csim, 1, 0, KNIGHT, forTeam);
    CreateChessPiece(csim, 2, 0, BISHOP, forTeam);
    CreateChessPiece(csim, 3, 0, QUEEN, forTeam);
    CreateChessPiece(csim, 4, 0, KING, forTeam);
    CreateChessPiece(csim, 5, 0, BISHOP, forTeam);
    CreateChessPiece(csim, 6, 0, KNIGHT, forTeam);
    CreateChessPiece(csim, 7, 0, ROOK, forTeam);
    CreateChessPiece(csim, 0, 1, PAWN, forTeam);
    CreateChessPiece(csim, 1, 1, PAWN, forTeam);
    CreateChessPiece(csim, 2, 1, PAWN, forTeam);
    CreateChessPiece(csim, 3, 1, PAWN, forTeam);
    CreateChessPiece(csim, 4, 1, PAWN, forTeam);
    CreateChessPiece(csim, 5, 1, PAWN, forTeam);
    CreateChessPiece(csim, 6, 1, PAWN, forTeam);
    CreateChessPiece(csim, 7, 1, PAWN, forTeam);
}

void InitChessTable(ChessSimulator* csim){
    for(int i = 0; i < csim->rows; i++){
        for(int j = 0; j < csim->rows; j++){
            ChessPiece* p = &csim->pMap[i][j];
            p->steps = NULL;
            p->countSteps = 0;
            p->type = NON;
        }
    }
}


void ClearChessTable(ChessSimulator* csim){
    for(int i = 0; i < csim->rows; i++){
        for(int j = 0; j < csim->rows; j++){
            DeleteChessPiece(csim, j, i);
        }
    }
}

void CreateChessPiece(ChessSimulator* csim, int px, int py, PieceType type, int team){

    ChessPiece p;
    p.type = type;
    p.team = team;
    p.moved = 0;
    p.steps = (Vec2d*)malloc(sizeof(Vec2d) * csim->cols * csim->rows);
    p.countSteps = 0;
    p.pos.x = px;
    p.pos.y = py;

    csim->pMap[py][px] = p;
}

void DeleteChessPiece(ChessSimulator* csim, int px, int py){
    ChessPiece* p = &csim->pMap[py][px];
    if(p->steps == NULL) return;
    free(p->steps);
    p->steps = NULL;
    p->countSteps = 0;
    p->type = NON;
}

void CreateChessTeam(ChessSimulator* csim, int teamId){
    ChessTeam team;
    team.score = 0;
    team.id = teamId;

    team.pieces = (ChessPiece**)malloc(sizeof(ChessPiece*) * pieceCount);
    team.pcount = 0;
    for(int i = 0; i < csim->rows; i++){
        for(int j = 0; j < csim->cols; j++){
            if(csim->pMap[i][j].team == teamId){
                team.pieces[team.pcount++] = &csim->pMap[i][j];
            }
        }
    }

    teams[teamCount++] = team;
}

void DestroyChessTeam(ChessSimulator* csim, int teamId){
    free(teams[teamId].pieces);
    teams[teamId].id = -1;
}


void DrawChessTable(ChessSimulator* csim){
    SDL_Rect tile;
    tile.h = csim->pSide;
    tile.w = csim->pSide;

    int tileNum = 0;
    for(int i = 0; i < csim->rows; i++){
        for(int j = 0; j < csim->cols; j++){
            tile.x = j * csim->pSide;
            tile.y = i * csim->pSide;
            Color c = tileColors[(tileNum++) % 2];
            SDL_SetRenderDrawColor(csim->win->renderer, c.r, c.g, c.b, c.a);
            SDL_RenderFillRect(csim->win->renderer, &tile);
        }
        tileNum++;
    }
}

void DrawChessPieceSteps(ChessSimulator* csim, int px, int py){
    ChessPiece* p = &csim->pMap[py][px];

    SDL_Rect rect;
    rect.w = csim->pSide;
    rect.h = csim->pSide;

    SDL_SetRenderDrawColor(csim->win->renderer, stepColor.r, stepColor.g, stepColor.b, stepColor.a);
    for(int i = 0; i < p->countSteps; i++){
        Vec2d step = p->steps[i];
        
        rect.x = step.x * csim->pSide;
        rect.y = step.y * csim->pSide;
        SDL_RenderFillRect(csim->win->renderer, &rect);
    }
}

void DrawChessPieces(ChessSimulator* csim){
    SDL_Rect piece;
    piece.h = csim->pSide - 20;
    piece.w = csim->pSide - 30;
    for(int i = 0; i < csim->rows; i++){
        for(int j = 0; j < csim->cols; j++){
            ChessPiece* p = &csim->pMap[i][j];
            if(p->type == NON) continue;
            
            int sx = (csim->pSide - piece.w) / 2;
            int sy = (csim->pSide - piece.h) / 2;
            switch (p->type)
            {
            case PAWN:
                piece.x = csim->pSide * j + sx;
                piece.y = csim->pSide * i + sy;
                SDL_RenderCopy(csim->win->renderer, 
                    p->team == WHITE_TEAM ? pieceTextures[WPAWN_TEXTURE] : pieceTextures[BPAWN_TEXTURE],
                    NULL,
                    &piece
                );
                break;
            case ROOK:
                piece.x = csim->pSide * j + sx;
                piece.y = csim->pSide * i + sy;
                SDL_RenderCopy(csim->win->renderer, 
                    p->team == WHITE_TEAM ? pieceTextures[WROOK_TEXTURE] : pieceTextures[BROOK_TEXTURE],
                    NULL,
                    &piece
                );
                break;
            case KNIGHT:
                piece.x = csim->pSide * j + sx;
                piece.y = csim->pSide * i + sy;
                SDL_RenderCopy(csim->win->renderer, 
                    p->team == WHITE_TEAM ? pieceTextures[WKNIGHT_TEXTURE] : pieceTextures[BKNIGHT_TEXTURE],
                    NULL,
                    &piece
                );
                break;
            case BISHOP:
                piece.x = csim->pSide * j + sx;
                piece.y = csim->pSide * i + sy;
                SDL_RenderCopy(csim->win->renderer, 
                    p->team == WHITE_TEAM ? pieceTextures[WBISHOP_TEXTURE] : pieceTextures[BBISHOP_TEXTURE],
                    NULL,
                    &piece
                );
                break;
            case QUEEN:
                piece.x = csim->pSide * j + sx;
                piece.y = csim->pSide * i + sy;
                SDL_RenderCopy(csim->win->renderer, 
                    p->team == WHITE_TEAM ? pieceTextures[WQUEEN_TEXTURE] : pieceTextures[BQUEEN_TEXTURE],
                    NULL,
                    &piece
                );
                break;
            case KING:
                piece.x = csim->pSide * j + sx;
                piece.y = csim->pSide * i + sy;
                SDL_RenderCopy(csim->win->renderer, 
                    p->team == WHITE_TEAM ? pieceTextures[WKING_TEXTURE] : pieceTextures[BKING_TEXTURE],
                    NULL,
                    &piece
                );
                break;
            }
        }
    }    
}



int GetAvailableStepsCount(ChessSimulator* csim, int team){
    int totalSteps = 0;
    for(int i = 0; i < teams[team].pcount; i++){
        totalSteps += teams[team].pieces[i]->countSteps;
    }
    return totalSteps;
}

int CheckPieceSteps(ChessSimulator* csim, ChessPiece* p, int px, int py){
    for(int i = 0; i < p->countSteps; i++){
        if(p->steps[i].x == px && p->steps[i].y == py) return 1;
    }
    return 0;
}

int CheckIfUnderAttack(ChessSimulator* csim, int team, int px, int py){
    for(int i = 0; i < teams[team].pcount; i++){
        return CheckPieceSteps(csim, teams[team].pieces[i], px, py);
    }
    return 0;
}


int TileIsAtUse(ChessSimulator* csim, int px, int py){
    if(!(px >= 0 && px < csim->cols && py >= 0 && py < csim->rows)) return 1;
    return (csim->pMap[py][px].type != NON ? 1 : 0);
}

int PieceTeamIs(ChessSimulator* csim, int team, int px, int py){
    if(!(px >= 0 && px < csim->cols && py >= 0 && py < csim->rows)) return 1;
    return csim->pMap[py][px].team == team;
}

void AddStep(ChessSimulator* csim, ChessPiece* p, int px, int py){
    if(!(px >= 0 && px < csim->cols && py >= 0 && py < csim->rows)) return;
    if(CheckPieceSteps(csim, p, px, py)) return;
    p->steps[p->countSteps].x = px;
    p->steps[p->countSteps].y = py;
    p->countSteps++;
}

void AddStepByChecking(ChessSimulator* csim, ChessPiece* p, int px, int py){
    if(!TileIsAtUse(csim, px, py)) AddStep(csim, p, px, py);
    else {
        if(!PieceTeamIs(csim, px, py, p->team)) AddStep(csim, p, px, py);
    }
}

void CalcPawnMoves(ChessSimulator* csim, ChessPiece* p){
    printf("%d %d\n", p->pos.x, p->pos.y);
    if(p->team == WHITE_TEAM){
        if(!TileIsAtUse(csim, p->pos.x, p->pos.y - 1)){
            AddStep(csim, p, p->pos.x, p->pos.y - 1);
            if(p->pos.y == 6 && !TileIsAtUse(csim, p->pos.x, p->pos.y - 2)){
                AddStep(csim, p, p->pos.x, p->pos.y - 2);
            }
        }
        if(TileIsAtUse(csim, p->pos.x + 1, p->pos.y - 1)){
            if(!PieceTeamIs(csim, p->pos.x + 1, p->pos.y - 1, WHITE_TEAM))
                AddStep(csim, p, p->pos.x + 1, p->pos.y - 1);
        }
        if(TileIsAtUse(csim, p->pos.x - 1, p->pos.y - 1)){
            if(!PieceTeamIs(csim, p->pos.x - 1, p->pos.y - 1, WHITE_TEAM))
                AddStep(csim, p, p->pos.x - 1, p->pos.y - 1);
        }
    }

    if(p->team == BLACK_TEAM){
        if(!TileIsAtUse(csim, p->pos.x, p->pos.y + 1)){
            AddStep(csim, p, p->pos.x, p->pos.y + 1);
            if(p->pos.y == 1 && !TileIsAtUse(csim, p->pos.x, p->pos.y + 2)){
                AddStep(csim, p, p->pos.x, p->pos.y + 2);
            }
        }
        if(TileIsAtUse(csim, p->pos.x + 1, p->pos.y + 1)){
            if(!PieceTeamIs(csim, p->pos.x + 1, p->pos.y + 1, WHITE_TEAM))
                AddStep(csim, p, p->pos.x + 1, p->pos.y + 1);
        }
        if(TileIsAtUse(csim, p->pos.x - 1, p->pos.y + 1)){
            if(!PieceTeamIs(csim, p->pos.x - 1, p->pos.y + 1, WHITE_TEAM))
                AddStep(csim, p, p->pos.x - 1, p->pos.y + 1);
        }
    }
}

void CalcRookMoves(ChessSimulator* csim, ChessPiece* p){
    int x = p->pos.x;
    int y = p->pos.y;

    // Y steps
    for(int i = y; i >= 0; i--){
        AddStepByChecking(csim, p, x, i);
    }

    for(int i = y; i < csim->rows; i++){
        AddStepByChecking(csim, p, x, i);
    }


    // X steps
    for(int j = x; j >= 0; j--){
        AddStepByChecking(csim, p, j, y);
    }

    for(int j = y; j < csim->cols; j++){
        AddStepByChecking(csim, p, j, y);
    }
}

void CalcKnightMoves(ChessSimulator* csim, ChessPiece* p){
    int x = p->pos.x;
    int y = p->pos.y;

    if(!TileIsAtUse(csim, x + 1, y + 2)) AddStep(csim, p, x + 1, y + 2);
    else {
        if(!PieceTeamIs(csim, x + 1, y + 2, p->team)) AddStep(csim, p, x + 1, y + 2);
    }
    if(!TileIsAtUse(csim, x - 1, y + 2)) AddStep(csim, p, x - 1, y + 2);
    else {
        if(!PieceTeamIs(csim, x - 1, y + 2, p->team)) AddStep(csim, p, x - 1, y + 2);
    }

    if(!TileIsAtUse(csim, x + 1, y - 2)) AddStep(csim, p, x + 1, y - 2);
    else {
        if(!PieceTeamIs(csim, x + 1, y - 2, p->team)) AddStep(csim, p, x + 1, y - 2);
    }
    if(!TileIsAtUse(csim, x - 1, y - 2)) AddStep(csim, p, x - 1, y - 2);
    else {
        if(!PieceTeamIs(csim, x - 1, y - 2, p->team)) AddStep(csim, p, x - 1, y - 2);
    }
    

    if(!TileIsAtUse(csim, x + 2, y + 1)) AddStep(csim, p, x + 2, y + 1);
    else {
        if(!PieceTeamIs(csim, x + 2, y + 1, p->team)) AddStep(csim, p, x + 2, y + 1);
    }
    if(!TileIsAtUse(csim, x + 2, y - 1)) AddStep(csim, p, x + 2, y - 1);
    else {
        if(!PieceTeamIs(csim, x + 2, y - 1, p->team)) AddStep(csim, p, x + 2, y - 1);
    }

    if(!TileIsAtUse(csim, x - 2, y + 1)) AddStep(csim, p, x - 2, y + 1);
    else {
        if(!PieceTeamIs(csim, x - 2, y + 1, p->team)) AddStep(csim, p, x - 2, y + 1);
    }
    if(!TileIsAtUse(csim, x - 2, y - 1)) AddStep(csim, p, x - 2, y - 1);
    else {
        if(!PieceTeamIs(csim, x - 2, y - 1, p->team)) AddStep(csim, p, x - 2, y - 1);
    }
}

void CalcBishopMoves(ChessSimulator* csim, ChessPiece* p){
    int x = p->pos.x;
    int y = p->pos.y;

    for(int i = 0, nx = x - i, ny = y - i; nx >= 0 && ny >= 0; i++){
        AddStepByChecking(csim, p, nx, ny);
    }

    for(int i = 0, nx = x - i, ny = y + i; nx >= 0 && ny < csim->rows; i++){
        AddStepByChecking(csim, p, nx, ny);
    }

    for(int i = 0, nx = x + i, ny = y - i; nx < csim->cols && ny >= 0; i++){
        AddStepByChecking(csim, p, nx, ny);
    }

    for(int i = 0, nx = x + i, ny = y + i; nx < csim->cols && ny < csim->rows; i++){
        AddStepByChecking(csim, p, nx, ny);
    }
}

void CalcQueenMoves(ChessSimulator* csim, ChessPiece* p){
    CalcRookMoves(csim, p);
    CalcBishopMoves(csim, p);
}

void CalcKingMoves(ChessSimulator* csim, ChessPiece* p){

}

void CalcPieceMoves(ChessSimulator* csim, int px, int py){
    ChessPiece* p = &csim->pMap[py][px];

    switch (p->type)
    {
    case PAWN:
        CalcPawnMoves(csim, p);
        break;
    case ROOK:
        CalcRookMoves(csim, p);
        break;
    case KNIGHT:
        CalcKnightMoves(csim, p);
        break;
    case BISHOP:
        CalcBishopMoves(csim, p);
        break;
    case QUEEN:
        CalcQueenMoves(csim, p);
        break;
    case KING:
        CalcKingMoves(csim, p);
        break;
    default:
        break;
    }
}

void CalcAllMoves(ChessSimulator* csim){
    for(int i = 0; i < csim->rows; i++){
        for(int j = 0; j < csim->cols; j++){
            if(csim->pMap[i][j].type != NON){
                CalcPieceMoves(csim, j, i);
            }
        }
    }
}

void ProcessChessInputs(ChessSimulator* csim, SDL_Event event){

    while(SDL_PollEvent(&event)){
        
        if(event.key.keysym.sym == SDLK_ESCAPE) csim->win->isrunning = 0;

    }
}

void ChessGame(ChessSimulator* csim){
    SDL_Event event;

    LoadPieceTextures(csim);
    NewGameChessTable(csim, WHITE_TEAM);

    while(csim->win->isrunning){
        ProcessChessInputs(csim, event);
        CalcPieceMoves(csim, 1, 1);
        Clear(csim->win);
        DrawChessTable(csim);
        DrawChessPieceSteps(csim, 1, 1);
        DrawChessPieces(csim);
        SDL_RenderPresent(csim->win->renderer);
        SDL_RenderClear(csim->win->renderer);
        SDL_Delay(100);
    }

    DestroyPieceTextures(csim);
}