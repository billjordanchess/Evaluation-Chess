#include "globals.h"

#define HALFKENDMOB 4

#define BVAL 300
#define RVAL 500
#define QVAL 900
#define ROOKBEHIND 15
//rook_behind

#define QUEENING 650//680

int endgame_score[MAX_PLY][2];

int endmatrix[10][3][10][3];

int RookMoveCount(const int, const int);

int PassedScore(const int s,const int xs);

int PawnEndingScore(const int s,const int xs);

void Alg(int a,int b);
void ShowAll2();

int EndgameScore(const int,const int);

int OppositeBishops();

int MostAdvancedPawn(const int s,const int);
int LeastDifference(const int s,const int xs);

int EvalEndgame()
{
int score[2];
score[0] = 0;
score[1] = 0;

if(bit_units[0] == (bit_pieces[0][0] | bit_pieces[0][5]) && bit_units[1] == (bit_pieces[1][0] | bit_pieces[1][5]))
{
  return PawnEndingScore(side,xside) - PawnEndingScore(xside,side);
}

if(bit_pieces[0][0]==0 && bit_pieces[1][0]==0)
{
	/*
	if(game_list[hply-1].capture<6)
	{
		int result = endmatrix[piece_mat[side]/100][total[side][1]][piece_mat[xside]/100][total[xside][1]];
		if(result == 77)
		{
		drawn = 1;
		return 0;//21/3/13
		}
		if(result != 0 && SafeKingMoves(side,xside)>0 && (!bit_pieces[side][5] & bit_units[xside]))
		{   //add defended
			//z();

			return result;
		}
	}
	*/
	//score[0] += KingPawnLess[pieces[0][5][0]];
	//score[1] += KingPawnLess[pieces[1][5][0]];

  if(abs(piece_mat[0]-piece_mat[1])<500)
  {
	  score[0] -= (piece_mat[0]>>1);
	  score[1] -= (piece_mat[1]>>1);
  }
  return score[side] - score[xside];
}
	
if(pawn_mat[1]>0 && piece_mat[1]>0 && piece_mat[0]+pawn_mat[0]>piece_mat[1]+pawn_mat[1])
  {
    score[0] += (piece_mat[0]*10)/piece_mat[1];
	if(mask_abc & bit_pieces[0][0] && mask_def & bit_pieces[0][0])
	{
      score[0] += pawns_both_sides;
	}
  }
 else if(pawn_mat[0]>0 && piece_mat[0]>0 && piece_mat[0]+pawn_mat[0]<piece_mat[1]+pawn_mat[1])
  {
    score[1] += (piece_mat[1]*10)/piece_mat[0];
	if(mask_abc & bit_pieces[1][0] && mask_def & bit_pieces[1][0])
	{
      score[1] += pawns_both_sides;
	}
  }

if(pawn_mat[0]==0 && abs(piece_mat[0]-piece_mat[1])<500)
	score[0] -= (piece_mat[0]>>1);
if(pawn_mat[1]==0 && abs(piece_mat[0]-piece_mat[1])<500)
	score[1] -= (piece_mat[1]>>1);

if(bit_pieces[0][2] && bit_pieces[1][2])
{
if(OppositeBishops()>0)
{
	score[0] -= pawn_mat[0] * 100/opposite_coloured_bishops;
	score[1] -= pawn_mat[1] * 100/opposite_coloured_bishops;
}
}

if(passed_list[side])
	score[side] += PassedScore(side,xside);

if(passed_list[xside])
	score[xside] += PassedScore(xside,side);

int sq;

BITBOARD b1;

for(int s = 0;s<2;s++)
{
b1 = bit_pieces[s][3];
while(b1)
{
	sq = NextBit(b1);
	b1 &= not_mask[sq];
	score[s] += RookMoveCount(s,sq);
}
}
 return score[side] - score[xside];
}

int PassedScore(const int s,const int xs)
{
int score=0;
int x;
BITBOARD b1 = passed_list[s]  & (mask_squareking[xs][NextBit(bit_pieces[xs][5])] );

while(b1)
{
  x = NextBit(b1);
  b1 &= not_mask[x];

  score += passed[s][x]/2;
  if(piece_mat[xs]==0 && !(mask_path[s][x] & bit_all))
  {
    score += square_of_pawn;
  }
}

return score;
}

int OppositeBishops()
{
if(bit_color[1] & bit_pieces[0][2] && bit_color[0] & bit_pieces[1][2])
	return 1;
if(bit_color[0] & bit_pieces[0][2] && bit_color[1] & bit_pieces[1][2])
	return 1;
return 0;
}

int PawnEndingScore(const int s,const int xs)
{
int x;
int a;
int score=0;
BITBOARD b1 = passed_list[s];//to calculate

while(b1)
{
  x = NextBit(b1);
  b1 &= not_mask[x];

 if(!(mask_squarepawn[s][x] & bit_pieces[xs][5]))
 {
    score += square_of_pawn;
 }

 if(rank[s][x]>2)
 {
  if(bit_left[xs][x] & bit_pieces[s][0])
  {
  a=pawnleft[xs][x];
  if((mask_passed[s][a] & not_mask[x-1] & bit_pieces[xs][0])==0)
  {
  score += kp_supported;
  }
  else
  {
  if(bit_right[xs][x] & bit_pieces[s][0])
  {
	a=pawnright[xs][x];
   if((mask_passed[s][a] & not_mask[x+1] & bit_pieces[xs][0])==0)
  {
  score += kp_supported;
  }
  }
  }

  }
 }
     //outside passed pawn
     if(col[x]==0 || (col[x]==1 &&  (mask_files[0] & bit_pieces[xs][0])==0) &&
      (bit_pieces[s][0] & mask_def))
     {
  	  score+= kp_outside;    
      }
     if(col[x]==7 || (col[x]==6 &&  (mask_files[7] & bit_pieces[xs][0])==0) &&
     (bit_pieces[s][0] & mask_abc))
     {
 	  score+= kp_outside;    
     }
}
if(bit_kingattacks[s] & bit_pieces[xs][0] & ~(bit_pawnattacks[xs]))
	score+=kp_attack_pawn;
return score;
}



