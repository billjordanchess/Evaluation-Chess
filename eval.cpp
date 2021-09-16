#include "globals.h"

int kingside[2];
int queenside[2];
int kingattack[2];
int queenattack[2];

int piece_value[6];

int EvalPawn(const int s,const int xs, const int sq);
int EvalKnight(const int s,const int xs, const int sq);
int EvalLinePiece(const int s, const int xs,const int p,const int sq);
int EvalKing(const int s, const int xs, const int sq);

void SetPawnAttacks();

int queenside_pawns[2],kingside_pawns[2];

int Eval()
{
int score[2] = {0,0};
int bishops[2] = {0,0};

queenside_pawns[0] = 0;
queenside_pawns[1] = 0;
kingside_pawns[0] = 0;
kingside_pawns[1] = 0;

piece_mat[0] = 0;
piece_mat[1] = 0;

pawn_mat[0] = 0;
pawn_mat[1] = 0;

BITBOARD b1;
int sq;

SetPawnAttacks();
for(int s=1;s<2;s++)
for(int p=1;p<5;p++)
{
	if(bit_pawnattacks[s] & bit_pieces[!s][p])
		score[s] += attacks[0][p];
	if(bit_pawnattacks[s] & bit_pieces[s][p])
		score[s] += defend[0][p];
}

for(int s=0;s<2;s++)
{
	b1 = bit_pieces[s][P];
	while(b1)
	{
		sq = NextBit(b1);
		b1 &= not_mask[sq];
		score[s] += piece_score[s][P][sq];
		score[s] += EvalPawn(s,!s,sq) + piece_value[P];
	}
	b1 = bit_pieces[s][N];
	while(b1)
	{
		sq = NextBit(b1);
		b1 &= not_mask[sq];
		score[s] += piece_score[s][N][sq];
		piece_mat[s] += piece_value[N];
		score[s] += EvalKnight(s,!s,sq);
	}
	b1 = bit_pieces[s][B];
	while(b1)
	{
		sq = NextBit(b1);
		b1 &= not_mask[sq];
		score[s] += piece_score[s][B][sq];
		piece_mat[s] += piece_value[B];
		score[s] += EvalLinePiece(s,!s,B,sq);
		bishops[s]++;
		if(bishops[s]==2)
			score[s] += bishop_pair;
	}
	b1 = bit_pieces[s][R];
	while(b1)
	{
		sq = NextBit(b1);
		b1 &= not_mask[sq];
		score[s] += piece_score[s][R][sq];
		if(!(mask_cols[sq] & bit_pieces[s][0]))
		{
			score[s] += half_open_file;
			if(!(mask_cols[sq] & bit_pieces[!s][0]))
				score[s] += half_open_file;
		}
		score[s] += EvalLinePiece(s,!s,R,sq);
		piece_mat[s] += piece_value[R];
	}
	b1 = bit_pieces[s][Q];
	while(b1)
	{
		sq = NextBit(b1);
		b1 &= not_mask[sq];
		score[s] += piece_score[s][Q][sq];
		piece_mat[s] += piece_value[Q];
		score[s] += EvalLinePiece(s,!s,Q,sq);
	}
	score[s] += piece_mat[s];
	score[s] += EvalKing(s,!s,kingloc[s]);
}

if (pawn_mat[0] == 0 && pawn_mat[1] == 0)
{
  if(piece_mat[0]<=300 && piece_mat[1]<=300)
	  return 0;
}

return score[side] - score[xside];
}

int EvalPawn(const int s,const int xs,const int sq)
{
	int r = piece_score[s][0][sq];

     if((mask_isolated[sq] & bit_pieces[s][0])==0)
     {
		r -= isolated[s][sq];
        if((mask_cols[sq] & bit_pieces[xs][0]) == 0)
          r -= isolated[s][sq];
		if(mask[pawnplus[s][sq]] & bit_pieces[N][xs])
		 r += outpost[N];
     }
	else
	{
	if(bit_pieces[s][0] & mask_path[s][sq])
    {
	  r -= doubled[0][sq];
      if(!((bit_pieces[0][0] | bit_pieces[1][0]) & mask_left_col[sq] ) ||
       !((bit_pieces[0][0] | bit_pieces[1][0]) & mask_right_col[sq]))
      {
	   r -= doubled_majority;
      }
    }
    if((mask_backward[s][sq] & bit_pieces[s][0])==0)
    {
      r -= backward[s][sq];
	  if(bit_pawncaptures[s][pawnplus[s][sq]] & bit_pieces[xs][0])
	  {
          r -= backward[s][sq];
	  }
      if(bit_pieces[s][0] & mask_path[xs][sq])
      {
        r -= doubled[s][sq];
      }
      if((mask_cols[sq] & bit_pieces[xs][0]) == 0)
        r -= backward_open;
	  if(mask[pawnplus[s][sq]] & bit_pieces[N][xs])
		 r += outpost[N];
	    if(bit_pieces[s][0] & bit_adjacent[sq])
		{
		  r += connected[0][sq];
		}
    }
	}

    if(!(mask_passed[s][sq] & bit_pieces[xs][0]) && !(mask_path[s][sq] & bit_pieces[s][0]))
    {
	  if(bit_pawncaptures[xs][sq] & bit_pieces[s][0])
     {
      r += supported[0][sq];
	 }	
	    r += passed[s][sq];
    }
	
	if(mask[sq] && bit_adjacent[sq] & bit_pieces[s][0])
	{
		r += adjacent[s][sq];
	}
    kingside[s] += kingside_score[s][sq];
    queenside[s] += queenside_score[s][sq];

	BITBOARD b1;
	b1 = mask[pawnplus[s][sq]] & bit_units[s];
	if(b1)
	{
		if(b1 & bit_pieces[s][0]) r += blocked[s][0][sq];
		else if(b1 & bit_pieces[s][1]) r += blocked[s][1][sq];
		else if(b1 & bit_pieces[s][2]) r += blocked[s][2][sq];
		else if(b1 & bit_pieces[s][3]) r += blocked[s][3][sq];
		else if(b1 & bit_pieces[s][4]) r += blocked[s][4][sq];
		else if(b1 & bit_pieces[s][5]) r += blocked[s][5][sq];
	}
	return r;
}

int EvalKnight(const int s, const int xs, const int sq)
{
int score = 0;
int mob = 0, att = 0;
BITBOARD b1,b2;
int sq2;

b1 = bit_moves[N][sq] & bit_units[xs] & ~(bit_pawnattacks[xs] & bit_pieces[xs][0]);
while(b1)
{
	sq2 = NextBit(b1);
	b1 &= not_mask[sq2];
	score += attacks[1][board[sq2]];
}
    b1 = bit_moves[N][sq] & ~bit_pawnattacks[xs] & ~bit_units[s];
	b2 = b1;
	while(b1) 
	{
       mob++;
       b1 &= b1 - 1; 
   }
	mob *= mobility[N];
	if(mob==0)
		mob -= trapped[N];
	att = CountBits(b2 & bit_kingmoves[kingloc[xs]]) * attacks[N][K];
	return mob + att + score;
}

int EvalLinePiece(const int s, const int xs, const int p, const int sq)
{
int mob = 0, att = 0;
int score = 0;
int sq2;
BITBOARD b1,b2;

b1 = bit_moves[p][sq] & bit_units[xs];

if(b1)
{
	if(CountBits(bit_between[sq][kingloc[xs]] & bit_units[xs])==1) 
	{
		sq2 = NextBit(bit_between[sq][kingloc[xs]] & bit_units[xs]);
		score += pins[p][board[sq2]];
	}
}

b1 = bit_moves[p][sq];
	b2 = bit_units[s];
	while(b1 & b2)
	{
		sq2 = NextBit(b1 & b2);
		b1 &= bit_after[sq][sq2];
	}
	b2 = bit_units[xs];
	while(b1 & b2)
	{
		sq2 = NextBit(b1 & b2);
		if(!(bit_between[sq][sq2] & bit_all))
		{
			score += attacks[p][board[sq2]];
		}
		b1 &= bit_after[sq][sq2];
	}
	att = CountBits(b1 & bit_kingmoves[kingloc[xs]]) * attacks[p][K];
	b1 &= ~bit_pawnattacks[xs] & ~bit_units[s];
	
	mob = CountBits(b1);
	mob *= mobility[p];
	if(mob==0)
		mob -= trapped[p];
	return mob + att + score;
}

int EvalKing(const int s, const int xs, const int sq)
{
	int score = 0;
 if(bit_pieces[xs][Q]==0)
    score += piece_score[s][KING_ENDGAME][kingloc[s]];
  else
  {
	score += piece_score[s][K][kingloc[s]];
	if(bit_pieces[s][K] & mask_kingside)
		score += kingside_pawns[s];
	else if(bit_pieces[s][K] & mask_queenside)
		score += queenside_pawns[s];
  }

for(int x=0;x<4;x++)
{
	if(bit_moves[K][sq] & bit_pieces[s][x])
		score += defend[K][x];
	if(bit_moves[K][sq] & bit_pieces[xs][x])
		score += attacks[K][x];
}
	return score;
}

void SetPawnAttacks()
{
bit_pawnattacks[0] = (bit_pieces[0][0] & not_a_file)<<7;
bit_pawnattacks[0] |=(bit_pieces[0][0] & not_h_file)<<9;
bit_pawnattacks[1] = (bit_pieces[1][0] & not_a_file)>>9;
bit_pawnattacks[1] |=(bit_pieces[1][0] & not_h_file)>>7;
}