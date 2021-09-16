#include <stdlib.h>
#include <assert.h>
#include "globals.h"

BITBOARD bit_pawncaptures[2][64];
BITBOARD bit_pawndefends[2][64];
BITBOARD bit_left[2][64];
BITBOARD bit_right[2][64];

BITBOARD bit_pawnmoves[2][64];
BITBOARD bit_moves[6][64];

BITBOARD bit_pieces[2][7];
BITBOARD bit_units[2];
BITBOARD bit_all;

BITBOARD bit_between[64][64];
BITBOARD bit_after[64][64];

BITBOARD mask_passed[2][64];
BITBOARD mask_path[2][64];

BITBOARD mask[64];
BITBOARD not_mask[64];
BITBOARD mask_cols[64];
BITBOARD mask_isolated[64];

BITBOARD mask_kingside;
BITBOARD mask_queenside;
BITBOARD not_a_file;
BITBOARD not_h_file;

int pawnplus[2][64];
int pawndouble[2][64];
int pawnleft[2][64];
int pawnright[2][64];

void SetLines();
void SetRowCol();
void SetBetweenVector();
int GetEdge(int sq,int plus);

void SetBit(BITBOARD& bb, int square);
void SetBitFalse(BITBOARD& bb, int square);
int NextBit(BITBOARD bb);
void PrintBitBoard(BITBOARD bb);
void PrintCell(int x,BITBOARD bb);

void SetPawnBits();

void SetRowCol();

void SetBit(BITBOARD& bb, int square)
{
bb |= (1ui64 << square);
}

void SetBitFalse(BITBOARD& bb, int square)
{
bb &= ~mask[square];
}

//x & ~(x-1)
//c & -c or c & (~c + 1) from bitwise tricks

void SetBits()
{
memset(bit_pawncaptures,0,sizeof(bit_pawncaptures));
memset(bit_pawnmoves,0,sizeof(bit_pawnmoves));
memset(bit_moves[N],0,sizeof(bit_moves[N]));
memset(bit_moves[B],0,sizeof(bit_moves[B]));
memset(bit_moves[R],0,sizeof(bit_moves[R]));
memset(bit_moves[Q],0,sizeof(bit_moves[Q]));
memset(bit_moves[K],0,sizeof(bit_moves[K]));

memset(bit_adjacent,0,sizeof(bit_adjacent));

SetLines();
SetRowCol();
SetPawnBits();
SetBetweenVector();
}

void SetPawnBits()
{
for(int x=0;x<64;x++)
{
	SetBit(mask[x],x);
	not_mask[x] = ~mask[x];

    pawnleft[0][x]=-1;
    pawnleft[1][x]=-1;
    pawnright[0][x]=-1;
    pawnright[1][x]=-1;
	if(col[x]>0)
	{
		if(row[x]<7){pawnleft[0][x]=x+7;}
		if(row[x]>0){pawnleft[1][x]=x-9;}
	}
	if(col[x]<7)
	{
		if(row[x]<7){pawnright[0][x]=x+9;}
		if(row[x]>0){pawnright[1][x]=x-7;}
	}
    if(col[x]>0)
	{
		if(row[x]<7)
		{
			SetBit(bit_pawncaptures[0][x],pawnleft[0][x]);
			SetBit(bit_left[0][x],pawnleft[0][x]);
		}
		if(row[x]>0)
		{
			SetBit(bit_pawncaptures[1][x],pawnleft[1][x]);
			SetBit(bit_left[1][x],pawnleft[1][x]);
		}
    }
    if(col[x]<7)
	{
		if(row[x]<7)
		{
			SetBit(bit_pawncaptures[0][x],pawnright[0][x]);
			SetBit(bit_right[0][x],pawnright[0][x]);
		}
		if(row[x]>0)
		{
			SetBit(bit_pawncaptures[1][x],pawnright[1][x]);
			SetBit(bit_right[1][x],pawnright[1][x]);
		}
    }
	bit_pawndefends[0][x] = bit_pawncaptures[1][x];
	bit_pawndefends[1][x] = bit_pawncaptures[0][x];

	if(row[x]<7)
		pawnplus[0][x]=x+8;
	if(row[x]<6)
		pawndouble[0][x]=x+16;
	if(row[x]>0)
		pawnplus[1][x]=x-8;
	if(row[x]>1)
		pawndouble[1][x]=x-16;
}
}

void SetBetweenVector()
{
int x,y;
int z;
for(x=0;x<64;x++)
for(y=0;y < 64;y++)
{
	if(row[x]==row[y])
	{
		if(y > x)
			for(z=x+1;z < y;z++)
				SetBit(bit_between[x][y],z);
		else
			for(z=y+1;z < x;z++)
				SetBit(bit_between[x][y],z);
	}

if(col[x]==col[y])
{
	if(y > x)
		for(z=x+8;z < y;z+=8)
			SetBit(bit_between[x][y],z);
	else
		for(z=y+8;z < x;z+=8)
			SetBit(bit_between[x][y],z);
}

if(nwdiag[x]==nwdiag[y])
{
	if(y > x)
		for(z=x+7;z < y;z+=7)
			SetBit(bit_between[x][y],z);
	else
		for(z=y+7;z < x;z+=7)
			SetBit(bit_between[x][y],z);
}

if(nediag[x]==nediag[y])
{
	if(y > x)
		for(z=x+9;z < y;z+=9)
			SetBit(bit_between[x][y],z);
	else
		for(z=y+9;z < x;z+=9)
			SetBit(bit_between[x][y],z);
}
}

for(x=0;x < 64;x++)
for(y=0;y < 64;y++)
{
	if(x==y)
		continue;
if(row[x]==row[y])
{
	if(y > x)
		for(z=y;z <= row[y]*8+7;z++)
			SetBit(bit_after[x][y],z);
	else
		for(z=y;z >= row[y]*8;z--)
			SetBit(bit_after[x][y],z);
}

if(col[x]==col[y])
{
	if(y > x)
		for(z=y;z <= 56+col[y];z+=8)
			SetBit(bit_after[x][y],z);
	else
		for(z=y;z >= col[y];z-=8)
			SetBit(bit_after[x][y],z);
}
if(nwdiag[x]==nwdiag[y])
{
	if(y > x)
		for(z=y;z <= GetEdge(x,7);z+=7)
			SetBit(bit_after[x][y],z);
	else
		for(z=y;z >= GetEdge(x,-7);z-=7)
			SetBit(bit_after[x][y],z);
}

if(nediag[x]==nediag[y])
{
	if(y > x)
		for(z=y;z <= GetEdge(x,9);z+=9)
		SetBit(bit_after[x][y],z);
	else
		for(z=y;z >= GetEdge(x,-9);z-=9)
			SetBit(bit_after[x][y],z);
}

}

for(x=0;x < 64;x++)
for(y=0;y < 64;y++)
{
	bit_after[x][y] = ~bit_after[x][y];
}

}

int GetEdge(int sq,int plus)
{
do
{
  sq += plus;
}
while(col[sq] > 0 && col[sq] < 7 && row[sq] > 0 && row[sq] < 7);

return sq;
}

void SetRowCol()
{
for(int x=0;x < 64;x++)
{
if(col[x] < 2)
  SetBit(mask_queenside,x);
if(col[x] > 5)
  SetBit(mask_kingside,x);
}

for(int x=0;x < 64;x++)
for(int y=0;y < 64;y++)
{
	if(abs(col[x]-col[y])<2)
	{
		if(row[x] < row[y] && row[y] < 7)
			SetBit(mask_passed[0][x],y);
		if(row[x] > row[y] && row[y] > 0)
			SetBit(mask_passed[1][x],y);
	}
	if(abs(col[x]-col[y])==1)
	{
		SetBit(mask_isolated[x],y);
	}
	if(col[x]==col[y])
	{
		if(row[x] < row[y])
			SetBit(mask_path[0][x],y);
		if(row[x] > row[y])
			SetBit(mask_path[1][x],y);
	}

if(row[x]!=0 && row[x]!=7)
{
    if(row[x]>1)
    {
    if(row[y]>=row[x] && abs(col[x]-col[y])<=7-row[x])
    SetBit(mask_squarepawn[0][x],y);
    }
    else
    {
    if(row[y]>row[x] && abs(col[x]-col[y])<=6-row[x])
    SetBit(mask_squarepawn[0][x],y);
    }
    if(row[x]<6)
    {
    if(row[y]<=row[x] && abs(col[x]-col[y])<=row[x])
    SetBit(mask_squarepawn[1][x],y);
    }
    {
    if(row[y]<row[x] && abs(col[x]-col[y])<row[x])
    SetBit(mask_squarepawn[1][x],y);
    }

}

if(abs(col[x]-col[y])==1 && row[x]!=0 && row[x]!=7)
{
if(row[x]>=row[y] || (row[x]==1 && row[y]==2))
  SetBit(mask_backward[0][x],y);
if(row[x]<=row[y] || (row[x]==6 && row[y]==5))
  SetBit(mask_backward[1][x],y);
}

if(col[x]-col[y]==1)
{
  SetBit(mask_left_col[x],y);
}
if(col[y]-col[x]==1)
{
  SetBit(mask_right_col[x],y);
}

if(abs(col[x]-col[y])<=7-row[x])
  SetBit(mask_square[0][x],y);
if(abs(col[x]-col[y])<=row[x])
  SetBit(mask_square[1][x],y);

}

for(int x=0;x<64;x++)
{
if(row[x]==1)
  mask_square[0][x]=mask_square[0][x+8];
if(row[x]==6)
  mask_square[1][x]=mask_square[1][x-8];
if(col[x]>0)
  SetBit(mask_adjacent[x],x-1);
if(col[x]<7)
  SetBit(mask_adjacent[x],x+1);
}
for(int x=0;x<64;x++)
{
	for(int y=0;y<64;y++)
	{
		if(row[y]<row[x] || abs(col[x]-col[y])>row[y])
			SetBit(mask_squareking[0][x],y);
		if(row[y]>row[x]  || abs(col[x]-col[y])>rank[1][y])
			SetBit(mask_squareking[1][x],y);
	}
}

memset(bishop_a7,0,sizeof(bishop_a7));
memset(bishop_h7,0,sizeof(bishop_h7));
memset(knight_a7,0,sizeof(knight_a7));
memset(knight_h7,0,sizeof(knight_h7));

SetBit(bishop_a7[0],C7);
SetBit(bishop_a7[0],B6);
SetBit(bishop_a7[1],C2);
SetBit(bishop_a7[1],B3);
SetBit(bishop_h7[0],F7);
SetBit(bishop_h7[0],G6);
SetBit(bishop_h7[1],F2);
SetBit(bishop_h7[1],G3);

SetBit(knight_a7[0],B7);
SetBit(knight_a7[0],C6);
SetBit(knight_a7[1],B2);
SetBit(knight_a7[1],C3);
SetBit(knight_h7[0],G7);
SetBit(knight_h7[0],F6);
SetBit(knight_h7[1],F3);
SetBit(knight_h7[1],G2);

for(int x=0;x<64;x+=2)
	SetBit(bit_colors,x);

for(int x=0;x<64;x+=2)
{
	if(col[x]<7)
		SetBit(bit_adjacent[x],x-1);
}

}

void SetLines()
{
for(int x=0;x<64;x++)
{
	rank[0][x] = row[x];
	rank[1][x] = 7-row[x];
}
for(int x=0;x < 64;x++)
for(int y=0;y < 64;y++)
{
	if(col[x]==col[y])
		SetBit(mask_cols[x],y);
}
not_a_file = ~mask_cols[0];
not_h_file = ~mask_cols[7];
}

const int lsb_64_table[64] =
{
   63, 30,  3, 32, 59, 14, 11, 33,
   60, 24, 50,  9, 55, 19, 21, 34,
   61, 29,  2, 53, 51, 23, 41, 18,
   56, 28,  1, 43, 46, 27,  0, 35,
   62, 31, 58,  4,  5, 49, 54,  6,
   15, 52, 12, 40,  7, 42, 45, 16,
   25, 57, 48, 13, 10, 39,  8, 44,
   20, 47, 38, 22, 17, 37, 36, 26
};

int NextBit(BITBOARD bb)
{
   unsigned int folded;
   bb ^= bb - 1;
   folded = (int) bb ^ (bb >> 32);
   return lsb_64_table[folded * 0x78291ACF >> 26];
}

void PrintBitBoard(BITBOARD bb)
{
printf("\n");
int x;
for(x=56;x<64;x++)
  PrintCell(x,bb);
for(x=48;x<56;x++)
  PrintCell(x,bb);
for(x=40;x<48;x++)
  PrintCell(x,bb);
for(x=32;x<40;x++)
  PrintCell(x,bb);
for(x=24;x<32;x++)
  PrintCell(x,bb);
for(x=16;x<24;x++)
  PrintCell(x,bb);
for(x=8;x<16;x++)
  PrintCell(x,bb);
for(x=0;x<8;x++)
  PrintCell(x,bb);
}

void PrintCell(int x,BITBOARD bb)
{
if(mask[x] & bb)
printf(" X");
else
printf(" -");
if((x+1)%8==0)printf("\n");
}

BITBOARD bishop_a7[2];
BITBOARD bishop_h7[2];
BITBOARD knight_a7[2];
BITBOARD knight_h7[2];

BITBOARD passed_list[2];
BITBOARD mask_left_col[64];
BITBOARD mask_right_col[64];

BITBOARD bit_top[2];
BITBOARD bit_adjacent[64];

BITBOARD mask_zone;
BITBOARD mask_bishop_zone[2];
BITBOARD mask_centre;
BITBOARD mask_d3e4[2];
BITBOARD mask_d5e4[2];
BITBOARD mask_long[64];
BITBOARD mask_short[64];
BITBOARD mask_short_zone[2][64];
BITBOARD mask_long_zone[2][64];
BITBOARD mask_short_3[2][64];
BITBOARD mask_long_3[2][64];
BITBOARD mask_bishop3[2];

BITBOARD mask_moves[64][56];
BITBOARD mask_nwdiag[64];
BITBOARD mask_nediag[64];

BITBOARD bit_pinned;
BITBOARD bit_partial;
BITBOARD bit_captured;

BITBOARD mask_abc2;
BITBOARD mask_abc3;
BITBOARD mask_abc4;
BITBOARD mask_fgh2;
BITBOARD mask_fgh3;
BITBOARD mask_fgh4;

//legal moves from each square
BITBOARD bit_knightmoves[64];
BITBOARD bit_bishopmoves[64];
BITBOARD bit_rookmoves[64];
BITBOARD bit_queenmoves[64];
BITBOARD bit_kingmoves[64];
BITBOARD bit_rook_one[64];
BITBOARD bit_bishop_one[64];

//current position
BITBOARD bit_color[2];

//current attacks
BITBOARD bit_leftcaptures[2];
BITBOARD bit_rightcaptures[2];
BITBOARD bit_pawnattacks[2];
BITBOARD bit_knightattacks[2];
BITBOARD bit_bishopattacks[2];
BITBOARD bit_rookattacks[2];
BITBOARD bit_queenattacks[2];
BITBOARD bit_kingattacks[2];
BITBOARD bit_attacks[2];

BITBOARD bit_colors;

BITBOARD mask_square[2][64];
BITBOARD mask_backward[2][64];
BITBOARD mask_ranks[2][8];

BITBOARD mask_files[8];
BITBOARD mask_rows[8];

BITBOARD mask_adjacent[64];
BITBOARD mask_squarepawn[2][64];
BITBOARD mask_squareking[2][64];
BITBOARD mask_kingpawns[2];
BITBOARD mask_queenpawns[2];

BITBOARD mask_rookfiles;
BITBOARD mask_edge;
BITBOARD mask_corner;

BITBOARD not_mask_rookfiles;
BITBOARD not_mask_edge;
BITBOARD not_mask_corner;
BITBOARD not_mask_files[8];
BITBOARD not_mask_rows[8];

BITBOARD mask_abc;
BITBOARD mask_def;

int CountBits(BITBOARD x) 
{
   int count = 0;
   while (x) {
       count++;
       x &= x - 1; 
   }
   return count;
}