#include "globals.h"

int side,xside;
int fifty;
int ply,hply;
int nodes;

int board[64];

int history[64][64];
int king_endgame[2][64];

int pawn_mat[2];
int piece_mat[2];
int passed[2][64];

move move_list[MOVE_STACK];
int first_move[MAX_PLY];

game game_list[GAME_STACK];

const char piece_char[6] = 
{
	'P', 'N', 'B', 'R', 'Q', 'K'
};

const int init_color[64] = 
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1
};

const int init_board[64] = 
{
	3, 1, 2, 4, 5, 2, 1, 3,
	0, 0, 0, 0, 0, 0, 0, 0,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	0, 0, 0, 0, 0, 0, 0, 0,
	3, 1, 2, 4, 5, 2, 1, 3
};

const int col[64]=
{
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7
};

const int row[64]=
{
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7
};

const int nwdiag[64]={
	 14,13,12,11,10, 9, 8, 7,
	 13,12,11,10, 9, 8, 7, 6,
	 12,11,10, 9, 8, 7, 6, 5,
	 11,10, 9, 8, 7, 6, 5, 4,
	 10, 9, 8, 7, 6, 5, 4, 3,
	  9, 8, 7, 6, 5, 4, 3, 2,
	  8, 7, 6, 5, 4, 3, 2, 1,
	  7, 6, 5, 4, 3, 2, 1, 0
};

const int nediag[64]={
	 7, 8,9,10,11,12,13,14,
	 6, 7,8, 9,10,11,12,13,
	 5, 6,7, 8, 9,10,11,12,
	 4, 5,6, 7, 8, 9,10,11,
	 3, 4,5, 6, 7, 8, 9,10,
	 2, 3,4, 5, 6, 7, 8, 9,
	 1, 2,3, 4, 5, 6, 7, 8,
	 0, 1,2, 3, 4, 5, 6, 7
};

const int Flip[64] = 
{
	 56,  57,  58,  59,  60,  61,  62,  63,
	 48,  49,  50,  51,  52,  53,  54,  55,
	 40,  41,  42,  43,  44,  45,  46,  47,
	 32,  33,  34,  35,  36,  37,  38,  39,
	 24,  25,  26,  27,  28,  29,  30,  31,
	 16,  17,  18,  19,  20,  21,  22,  23,
	  8,   9,  10,  11,  12,  13,  14,  15,
	  0,   1,   2,   3,   4,   5,   6,   7
};

/* This is the castle_mask array. We can use it to determine
   the castling permissions after a move. What we do is
   logical-AND the castle bits with the castle_mask bits for
   both of the move's squares. Let's say castle is 1, meaning
   that white can still castle kingside. Now we play a move
   where the rook on h1 gets captured. We AND castle with
   castle_mask[63], so we have 1&14, and castle becomes 0 and
   white can't castle kingside anymore. */  //??

int castle_mask[64] = {
	13, 15, 15, 15, 12, 15, 15, 14,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	7, 15, 15, 15,  3, 15, 15, 11
};

int rank[2][64];
/*

SetTables fills the piece_score tables, king_endgame tables and passed tables with the individual piece tables.
The value of each piece is added to the score for each square.
The board is flipped for the Black scores.

*/
void SetTables()
{

for(int x=0;x<64;x++)
{
	for(int y = 0; y < 6;y++)
	{
		piece_score[1][y][x] = piece_score[0][y][Flip[x]];
		blocked[1][y][x] = blocked[0][y][Flip[x]];
		blocked2[1][y][x] = blocked2[0][y][Flip[x]];
	}
	passed[0][x] = passed[0][Flip[x]];
	passed[1][x] = passed[0][Flip[x]];
	supported[1][x] = supported[0][Flip[x]];
	connected[1][x] = connected[0][Flip[x]];

	isolated[1][x] = isolated[0][Flip[x]];
	backward[1][x] = backward[0][Flip[x]];
	doubled[1][x] = doubled[0][Flip[x]];

	kingside_score[1][x] = kingside_score[0][Flip[x]];
	queenside_score[1][x] = queenside_score[0][Flip[x]];

}

}

void InitBoard()
{
int x;
for(x=0;x<64;x++)
{
	board[x]=EMPTY;
}
memset(bit_pieces,0,sizeof(bit_pieces));
memset(bit_units,0,sizeof(bit_units));
bit_all = 0;

for (x = 0; x < 64; ++x) 
{
	if(init_board[x]<6)
		AddPiece(init_color[x],init_board[x],x);
}
side = 0;
xside = 1;
fifty = 0;
castle = 15;
ply = 0;
hply = 0;
first_move[0] = 0;
turn = 0;
kingloc[0] = E1;
kingloc[1] = E8;
}
/*

NewPosition gets the board ready before the computer starts to think.

*/
void NewPosition()
{
	int c=0;
	piece_mat[0] = pawn_mat[0] = 0;
	piece_mat[1] = pawn_mat[1] = 0;

	for(int i=0;i<64;i++)
	{
		if(board[i] < 6)
		{   
			if(bit_units[0] & mask[i]) c = 0;
			if(bit_units[1] & mask[i]) c = 1;
			AddPiece(c,board[i],i);
		}
	}
	currentkey = GetKey();
    currentlock = GetLock();
}
/*

Alg displays a move.

*/
void Alg(int a,int b)
{
Algebraic(a);
Algebraic(b);
}
/*

Algebraic displays a square.
e.g. 3 becomes col[3] + 96 which is ascii character 'd' and row[3]+1 which is '1'.
Passing 3 returns 'd1'.

*/
void Algebraic(int a)
{
	if(a<0 || a>63) return; 
	char c[2]="a";
	c[0] =  96+1+col[a];
	printf("%s%d",c,row[a]+1);
}
/*

SetMoves creates the lookup tables for Knights, line-pieces and Kings.
These will later be used to generate moves, captures and attacks.

*/
void SetMoves()
{
for(int x=0;x<64;x++)
{
	bit_moves[N][x] = 0;
	if(row[x]<6 && col[x]<7) 
		bit_moves[N][x] |= mask[x+17];
	if(row[x]<7 && col[x]<6) 
		bit_moves[N][x] |= mask[x+10];
	if(row[x]<6 && col[x]>0) 
		bit_moves[N][x] |= mask[x+15];
	if(row[x]<7 && col[x]>1) 
		bit_moves[N][x] |= mask[x+6];
	if(row[x]>1 && col[x]<7) 
		bit_moves[N][x] |= mask[x-15];
	if(row[x]>0 && col[x]<6) 
		bit_moves[N][x] |= mask[x-6];
	if(row[x]>1 && col[x]>0) 
		bit_moves[N][x] |= mask[x-17];
	if(row[x]>0 && col[x]>1) 
		bit_moves[N][x] |= mask[x-10];
}

for(int x=0;x<64;x++)
{
	if(col[x]>0) 
		bit_moves[K][x] |= mask[x-1];
	if(col[x]<7) 
		bit_moves[K][x] |= mask[x+1];
	if(row[x]>0) 
		bit_moves[K][x] |= mask[x-8];
	if(row[x]<7) 
		bit_moves[K][x] |= mask[x+8];
	if(col[x]<7 && row[x]<7) 
		bit_moves[K][x] |= mask[x+9];
	if(col[x]>0 && row[x]<7) 
		bit_moves[K][x] |= mask[x+7];
	if(col[x]>0 && row[x]>0) 
		bit_moves[K][x] |= mask[x-9];
	if(col[x]<7 && row[x]>0) 
		bit_moves[K][x] |= mask[x-7];
}
for(int x=0;x<64;x++)
{
	bit_moves[B][x] = 0;
	bit_moves[R][x] = 0;
	bit_moves[Q][x] = 0;

	for(int y=0;y < 64;y++)
	{
		if(x==y)
			continue;
		if(nwdiag[x]==nwdiag[y] || nediag[x]==nediag[y])
			bit_moves[B][x] |= mask[y];
		if(row[x]==row[y] || col[x]==col[y])
			bit_moves[R][x] |= mask[y];
		if(nwdiag[x]==nwdiag[y] || nediag[x]==nediag[y] || row[x]==row[y] || col[x]==col[y])
			bit_moves[Q][x] |= mask[y];
	}
}

}
/*dont need below here*/
int done[100];
int GetBest(int ply);//
void ShowAll(int ply)
{

move *g;
DisplayBoard();
memset(done, 0, sizeof(done));

          printf(" ply ");
        printf("%d",ply);
        printf(" nodes ");
        printf("%d",nodes);
        printf(" side ");
        printf("%d",side);
        printf(" xside ");
        printf("%d",xside);
        printf("\n");
  
	 printf(" one %d ",first_move[ply]);
	  printf(" two %d ",first_move[ply+1]);
	  Alg(move_list[first_move[0]].start,move_list[first_move[0]].dest);
			
        printf("\n");
 
     int j;
  for(int i=first_move[ply];i<first_move[ply+1];i++)
  //     for(int i=first_move[ply+1];i<first_move[ply + 2];i++)
    {
        j = GetBest(ply);
        {
        //how dest display current line?
        g = &move_list[j];
        printf("%s",MoveString(move_list[j].start,move_list[j].dest,move_list[j].promote));
        printf(" ");
        printf(" score ");
        printf("%d",g->score);
        printf("\n");
        }
    }
 printf("\n");
 
  _getch();
}

int GetBest(int ply)
{
move *g;
int bestscore = -100000000;
int best = 0;
  for(int i=0;i<first_move[ply+1]-first_move[ply];i++)
  {
    if(done[i] == 1) continue;
    g = &move_list[first_move[ply] + i];
    if(g->start == 0 && g->dest == 0)
      continue;//
    if(g->score > bestscore)
    {
        bestscore= g->score;
        best = i;
    }
  }
  if(best<1000) done[best]=1;//1000?
  return first_move[ply]+best;
}