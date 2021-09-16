#include <iostream>

#include "globals.h"

int piece_score[2][7][64];

int mobility[6];
int outpost[6];

int attacks[6][6];
int defend[6][6];
int pins[6][6];
int trapped[6];

int blocked[2][6][64];
int blocked2[2][6][64];

int isolated[2][64];
int backward[2][64];
int doubled[2][64];

int passed_pawn[6][64];
int supported[2][64];
int connected[2][64];
int adjacent[2][64];

int kingside_score[2][64];
int queenside_score[2][64];

int rook_support;
int queen_support;

int isolated_open;
int backward_open;

int separated_passed;

int square_of_pawn;

int doubled_majority;
int backward_majority;

int pawns_both_sides;
int centre_pawns;
int adjacent_centre_pawns;

int opposite_coloured_bishops;
int open_file;
int half_open_file;
int rook_behind;

int bishop_pair;//done

int kp_attack_pawn;
int kp_outside;
int kp_supported;
int kp_winning;

int no_castle;
int no_k_castle;
int no_q_castle;

int weak_back_rank;

int drawn;
int lone_king;
int mate;

int ReadData();

void SetBlockedFiles(int p,int start,int end,int start2,int end2,int n);
void SetPassedFiles(int p,int start,int end,int start2,int end2,int n);
void SetPieceFiles(int p,int start,int end,int start2,int end2,int n);
void ClearData();

void pb();

FILE *data_file;

int ReadData()
{
char s[80];
char line[80];
char a[80],b[80];

int x = 0;
int y = 0;
int n = 0;

data_file = fopen("data.txt", "r");
if (!data_file)
{
	printf("Diagram missing.\n");
	return -1;
}
ClearData();

for(int c = 0; c<1000;c++)
{
   if(fgets (line , 80 , data_file) == NULL )
	 break;
   if(line[0] == '=')
	   continue;

x = 0;
strcpy(a,"");
strcpy(s,"");
while(line[x] != ' ')
{
	b[0] = line[x]; b[1] = 0;
	strcat(a,b);
	x++;
}
n = atoi(a);
if(n==0)
	continue;
y = 0;
x += 2;

while(line[x++])
	s[y++] = line[x];
if(y>1 && s[y-2] == 10)
	s[y-2] = 0;

piece_value[K] = 10000;

//material = done
if(!strcmp(s,"P_VAL"))
	piece_value[P] = n;
if(!strcmp(s,"N_VAL"))
	piece_value[N] = n;
if(!strcmp(s,"B_VAL"))
	piece_value[B] = n;
if(!strcmp(s,"R_VAL"))
	piece_value[R] = n;
if(!strcmp(s,"Q_VAL"))
	piece_value[Q] = n;

//pawn position
if (!strcmp(s,"pawn on A2 or H2")) 
{piece_score[0][0][A2] = n; piece_score[0][0][H2] = n;}

if (!strcmp(s,"pawn on B2 or G2")) 
{piece_score[0][0][B2] = n; piece_score[0][0][G2] = n;} 

if (!strcmp(s,"pawn on C2 or F2")) 
{piece_score[0][0][C2] = n; piece_score[0][0][F2] = n;}  

if (!strcmp(s,"pawn on D2 or E2")) 
{piece_score[0][0][D2] = n; piece_score[0][0][E2] = n;}  

if (!strcmp(s,"pawn on A3 or H3")) 
{piece_score[0][0][A3] = n; piece_score[0][0][H3] = n;}

if (!strcmp(s,"pawn on B3 or G3")) 
{piece_score[0][0][B3] = n; piece_score[0][0][G3] = n;}

if (!strcmp(s,"pawn on C3 or F3")) 
{piece_score[0][0][C3] = n; piece_score[0][0][F3] = n;}

if (!strcmp(s,"pawn on D3 or E3")) 
{piece_score[0][0][D3] = n; piece_score[0][0][E3] = n;}  

if (!strcmp(s,"pawn on A4 or H4")) 
{piece_score[0][0][A4] = n; piece_score[0][0][H4] = n;}

if (!strcmp(s,"pawn on B4 or G4")) 
{piece_score[0][0][B4] = n; piece_score[0][0][G4] = n;}

if (!strcmp(s,"pawn on C4 or F4"))
{piece_score[0][0][C4] = n; piece_score[0][0][F4] = n;}

if (!strcmp(s,"pawn on D4 or E4")) 
{piece_score[0][0][D4] = n; piece_score[0][0][E4] = n;} 

if (!strcmp(s,"pawn on A5 or H5")) 
{piece_score[0][0][A5] = n; piece_score[0][0][H5] = n;}

if (!strcmp(s,"pawn on B5 or G5")) 
{piece_score[0][0][B5] = n; piece_score[0][0][G5] = n;}

if (!strcmp(s,"pawn on C5 or F5"))
{piece_score[0][0][C5] = n; piece_score[0][0][F5] = n;}

if (!strcmp(s,"pawn on D5 or E5")) 
{piece_score[0][0][D5] = n; piece_score[0][0][E5] = n;} 

if (!strcmp(s,"pawn on A6 or H6")) 
{piece_score[0][0][A6] = n; piece_score[0][0][H6] = n;}

if (!strcmp(s,"pawn on B6 or G6")) 
{piece_score[0][0][B6] = n; piece_score[0][0][G6] = n;}

if (!strcmp(s,"pawn on C6 or F6"))
{piece_score[0][0][C6] = n; piece_score[0][0][F6] = n;}

if (!strcmp(s,"pawn on D6 or E6")) 
{piece_score[0][0][D6] = n; piece_score[0][0][E6] = n;} 

//ranks + files = done
if (!strcmp(s,"knight on rank 1"))
	for(x=A1;x<=H1;x++) piece_score[0][1][x] += n;

if (!strcmp(s,"knight on rank 2"))
	for(x=A2;x<=H2;x++) piece_score[0][1][x] += n;

if (!strcmp(s,"knight on rank 3"))
	for(x=A3;x<=H3;x++) piece_score[0][1][x] += n;

if (!strcmp(s,"knight on rank 4"))
	for(x=A4;x<=H4;x++) piece_score[0][1][x] += n;

if (!strcmp(s,"knight on rank 5"))
	for(x=A5;x<=H5;x++) piece_score[0][1][x] += n;

if (!strcmp(s,"knight on rank 6"))
	for(x=A6;x<=H6;x++) piece_score[0][1][x] += n;

if (!strcmp(s,"knight on rank 7"))
	for(x=A7;x<=H7;x++) piece_score[0][1][x] += n;

if (!strcmp(s,"knight on rank 8"))
	for(x=A8;x<=H8;x++) piece_score[0][1][x] += n;

if (!strcmp(s,"knight on A or H file"))
	SetPieceFiles(1,A1,A8,H1,H8,n);
if (!strcmp(s,"knight on B or G file"))
	SetPieceFiles(1,B1,B8,G1,G8,n);
if (!strcmp(s,"knight on C or F file"))
	SetPieceFiles(1,C1,C8,F1,F8,n);
if (!strcmp(s,"knight on D or E file"))
	SetPieceFiles(1,D1,D8,E1,E8,n);

if (!strcmp(s,"bishop on rank 1"))
	for(x=A1;x<=H1;x++) piece_score[0][2][x] += n;

if (!strcmp(s,"bishop on rank 2"))
	for(x=A2;x<=H2;x++) piece_score[0][2][x] += n;

if (!strcmp(s,"bishop on rank 3"))
	for(x=A3;x<=H3;x++) piece_score[0][2][x] += n;

if (!strcmp(s,"bishop on rank 4"))
	for(x=A4;x<=H4;x++) piece_score[0][2][x] += n;

if (!strcmp(s,"bishop on rank 5"))
	for(x=A5;x<=H5;x++) piece_score[0][2][x] += n;

if (!strcmp(s,"bishop on rank 6"))
	for(x=A6;x<=H6;x++) piece_score[0][2][x] += n;

if (!strcmp(s,"bishop on rank 7"))
	for(x=A7;x<=H7;x++) piece_score[0][2][x] += n;

if (!strcmp(s,"bishop on rank 8"))
	for(x=A8;x<=H8;x++) piece_score[0][2][x] += n;

if (!strcmp(s,"king on A or H file"))
	  SetPieceFiles(2,A1,A8,H1,H8,n);
if (!strcmp(s,"king on B or G file"))
	SetPieceFiles(2,B1,B8,G1,G8,n);
if (!strcmp(s,"king on C or F file"))
	SetPieceFiles(2,C1,C8,F1,F8,n);
if (!strcmp(s,"king on D or E file"))
	SetPieceFiles(2,D1,D8,E1,E8,n);

if (!strcmp(s,"rook on rank 1"))
	for(x=A1;x<=H1;x++) piece_score[0][3][x] += n;

if (!strcmp(s,"rook on rank 2"))
	for(x=A2;x<=H2;x++) piece_score[0][3][x] += n;

if (!strcmp(s,"rook on rank 3"))
	for(x=A3;x<=H3;x++) piece_score[0][3][x] += n;

if (!strcmp(s,"rook on rank 4"))
	for(x=A4;x<=H4;x++) piece_score[0][3][x] += n;

if (!strcmp(s,"rook on rank 5"))
	for(x=A5;x<=H5;x++) piece_score[0][3][x] += n;

if (!strcmp(s,"rook on rank 6"))
	for(x=A6;x<=H6;x++) piece_score[0][3][x] += n;

if (!strcmp(s,"rook on rank 7"))
	for(x=A7;x<=H7;x++) piece_score[0][3][x] += n;

if (!strcmp(s,"rook on rank 8"))
	for(x=A8;x<=H8;x++) piece_score[0][3][x] += n;

if (!strcmp(s,"rook on A or H file"))
	SetPieceFiles(3,A1,A8,H1,H8,n);
if (!strcmp(s,"rook on B or G file"))
	SetPieceFiles(3,B1,B8,G1,G8,n);
if (!strcmp(s,"rook on C or F file"))
	SetPieceFiles(3,C1,C8,F1,F8,n);
if (!strcmp(s,"rook on D or E file"))
	SetPieceFiles(3,D1,D8,E1,E8,n);

if (!strcmp(s,"queen on rank 1"))
	for(x=A1;x<=H1;x++) piece_score[0][4][x] += n;

if (!strcmp(s,"queen on rank 2"))
	for(x=A2;x<=H2;x++) piece_score[0][4][x] += n;

if (!strcmp(s,"queen on rank 3"))
	for(x=A3;x<=H3;x++) piece_score[0][4][x] += n;

if (!strcmp(s,"queen on rank 4"))
	for(x=A4;x<=H4;x++) piece_score[0][4][x] += n;

if (!strcmp(s,"queen on rank 5"))
	for(x=A5;x<=H5;x++) piece_score[0][4][x] += n;

if (!strcmp(s,"queen on rank 6"))
	for(x=A6;x<=H6;x++) piece_score[0][4][x] += n;

if (!strcmp(s,"queen on rank 7"))
	for(x=A7;x<=H7;x++) piece_score[0][4][x] += n;

if (!strcmp(s,"queen on rank 8"))
	for(x=A8;x<=H8;x++) piece_score[0][4][x] += n;

if (!strcmp(s,"queen on A or H file"))
	SetPieceFiles(4,A1,A8,H1,H8,n);
if (!strcmp(s,"queen on B or G file"))
	SetPieceFiles(4,B1,B8,G1,G8,n);
if (!strcmp(s,"queen on C or F file"))
	SetPieceFiles(4,C1,C8,F1,F8,n);
if (!strcmp(s,"queen on D or E file"))
	SetPieceFiles(4,D1,D8,E1,E8,n);

if (!strcmp(s,"king on rank 1"))
	for(x=A1;x<=H1;x++) piece_score[0][5][x] += n;

if (!strcmp(s,"king on rank 2"))
	for(x=A2;x<=H2;x++) piece_score[0][5][x] += n;

if (!strcmp(s,"king on rank 3"))
	for(x=A3;x<=H3;x++) piece_score[0][5][x] += n;

if (!strcmp(s,"king on rank 4"))
	for(x=A4;x<=H4;x++) piece_score[0][5][x] += n;

if (!strcmp(s,"king on rank 5"))
	for(x=A5;x<=H5;x++) piece_score[0][5][x] += n;

if (!strcmp(s,"king on rank 6"))
	for(x=A6;x<=H6;x++) piece_score[0][5][x] += n;

if (!strcmp(s,"king on rank 7"))
	for(x=A7;x<=H7;x++) piece_score[0][5][x] += n;

if (!strcmp(s,"king on rank 8"))
	for(x=A8;x<=H8;x++) piece_score[0][5][x] += n;

if (!strcmp(s,"king on A or H file"))
	SetPieceFiles(5,A1,A8,H1,H8,n);
if (!strcmp(s,"king on B or G file"))
	SetPieceFiles(5,B1,B8,G1,G8,n);
if (!strcmp(s,"king on C or F file"))
	SetPieceFiles(5,C1,C8,F1,F8,n);
if (!strcmp(s,"king on D or E file"))
	SetPieceFiles(5,D1,D8,E1,E8,n);

if (!strcmp(s,"king endgame on rank 1"))
	for(x=A1;x<=H1;x++) piece_score[0][6][x] += n;

if (!strcmp(s,"king endgame on rank 2"))
	for(x=A2;x<=H2;x++) piece_score[0][6][x] += n;

if (!strcmp(s,"king endgame on rank 3"))
	for(x=A3;x<=H3;x++) piece_score[0][6][x] += n;

if (!strcmp(s,"king endgame on rank 4"))
	for(x=A4;x<=H4;x++) piece_score[0][6][x] += n;

if (!strcmp(s,"king endgame on rank 5"))
	for(x=A5;x<=H5;x++) piece_score[0][6][x] += n;

if (!strcmp(s,"king endgame on rank 6"))
	for(x=A6;x<=H6;x++) piece_score[0][6][x] += n;

if (!strcmp(s,"king endgame on rank 7"))
	for(x=A7;x<=H7;x++) piece_score[0][6][x] += n;

if (!strcmp(s,"king endgame on rank 8"))
	for(x=A8;x<=H8;x++) piece_score[0][6][x] += n;

if (!strcmp(s,"king endgame on A or H file"))
	SetPieceFiles(6,A1,A8,H1,H8,n);
if (!strcmp(s,"king endgame on B or G file"))
	SetPieceFiles(6,B1,B8,G1,G8,n);
if (!strcmp(s,"king endgame on C or F file"))
	SetPieceFiles(6,C1,C8,F1,F8,n);
if (!strcmp(s,"king endgame on D or E file"))
	SetPieceFiles(6,D1,D8,E1,E8,n);

//pawn blocked = done
if (!strcmp(s,"by pawn on rank 2"))
	for(x=A2;x<=H2;x++) blocked[0][0][x] += n;
if (!strcmp(s,"by pawn on rank 3"))
	for(x=A3;x<=H3;x++) blocked[0][0][x] += n;
if (!strcmp(s,"by pawn on rank 4"))
	for(x=A4;x<=H4;x++) blocked[0][0][x] += n;
if (!strcmp(s,"by pawn on rank 5"))
	for(x=A5;x<=H5;x++) blocked[0][0][x] += n;
if (!strcmp(s,"by pawn on rank 6"))
	for(x=A6;x<=H6;x++) blocked[0][0][x] += n;
if (!strcmp(s,"by pawn on rank 7"))
	for(x=A7;x<=H7;x++) blocked[0][0][x] += n;

if (!strcmp(s,"by knight on rank 2"))
	for(x=A2;x<=H2;x++) blocked[0][1][x] += n;
if (!strcmp(s,"by knight on rank 3"))
	for(x=A3;x<=H3;x++) blocked[0][1][x] += n;
if (!strcmp(s,"by knight on rank 4"))
	for(x=A4;x<=H4;x++) blocked[0][1][x] += n;
if (!strcmp(s,"by knight on rank 5"))
	for(x=A5;x<=H5;x++) blocked[0][1][x] += n;
if (!strcmp(s,"by knight on rank 6"))
	for(x=A6;x<=H6;x++) blocked[0][1][x] += n;
if (!strcmp(s,"by knight on rank 7"))
	for(x=A7;x<=H7;x++) blocked[0][1][x] += n;

if (!strcmp(s,"by bishop on rank 2"))
	for(x=A2;x<=H2;x++) blocked[0][2][x] += n;
if (!strcmp(s,"by bishop on rank 3"))
	for(x=A3;x<=H3;x++) blocked[0][2][x] += n;
if (!strcmp(s,"by bishop on rank 4"))
	for(x=A4;x<=H4;x++) blocked[0][2][x] += n;
if (!strcmp(s,"by bishop on rank 5"))
	for(x=A5;x<=H5;x++) blocked[0][2][x] += n;
if (!strcmp(s,"by bishop on rank 6")) 
	for(x=A6;x<=H6;x++) blocked[0][2][x] += n;
if (!strcmp(s,"by bishop on rank 7"))
	for(x=A7;x<=H7;x++) blocked[0][2][x] += n;

if (!strcmp(s,"by rook on rank 2"))
	for(x=A2;x<=H2;x++) blocked[0][3][x] += n;
if (!strcmp(s,"by rook on rank 3"))
	for(x=A3;x<=H3;x++) blocked[0][3][x] += n;
if (!strcmp(s,"by rook on rank 4"))
	for(x=A4;x<=H4;x++) blocked[0][3][x] += n;
if (!strcmp(s,"by rook on rank 5"))
	for(x=A5;x<=H5;x++) blocked[0][3][x] += n;
if (!strcmp(s,"by rook on rank 6"))
	for(x=A6;x<=H6;x++) blocked[0][3][x] += n;
if (!strcmp(s,"by rook on rank 7"))
	for(x=A7;x<=H7;x++) blocked[0][3][x] += n;

if (!strcmp(s,"by queen on rank 2"))
	for(x=A2;x<=H2;x++) blocked[0][4][x] += n;
if (!strcmp(s,"by queen on rank 3"))
	for(x=A3;x<=H3;x++) blocked[0][4][x] += n;
if (!strcmp(s,"by queen on rank 4"))
	for(x=A4;x<=H4;x++) blocked[0][4][x] += n;
if (!strcmp(s,"by queen on rank 5"))
	for(x=A5;x<=H5;x++) blocked[0][4][x] += n;
if (!strcmp(s,"by queen on rank 6"))
	for(x=A6;x<=H6;x++) blocked[0][4][x] += n;
if (!strcmp(s,"by queen on rank 7"))
	for(x=A7;x<=H7;x++) blocked[0][4][x] += n;

if (!strcmp(s,"by king on rank 2"))
	for(x=A2;x<=H2;x++) blocked[0][5][x] += n;
if (!strcmp(s,"by king on rank 3"))
	for(x=A3;x<=H3;x++) blocked[0][5][x] += n;
if (!strcmp(s,"by king on rank 4"))
	for(x=A4;x<=H4;x++) blocked[0][5][x] += n;
if (!strcmp(s,"by king on rank 5"))
	for(x=A5;x<=H5;x++) blocked[0][5][x] += n;
if (!strcmp(s,"by king on rank 6"))
	for(x=A6;x<=H6;x++) blocked[0][5][x] += n;
if (!strcmp(s,"by king on rank 7"))
	for(x=A7;x<=H7;x++) blocked[0][5][x] += n;

if (!strcmp(s,"by pawn on A or H file")) SetBlockedFiles(0,A1,A8,H1,H8,n);
if (!strcmp(s,"by pawn on B or G file")) SetBlockedFiles(0,B1,B8,G1,G8,n);
if (!strcmp(s,"by pawn on C or F file")) SetBlockedFiles(0,C1,C8,F1,F8,n);
if (!strcmp(s,"by pawn on D or E file")) SetBlockedFiles(0,D1,D8,E1,E8,n);

if (!strcmp(s,"by knight on A or H file")) SetBlockedFiles(1,A1,A8,H1,H8,n);
if (!strcmp(s,"by knight on B or G file")) SetBlockedFiles(1,B1,B8,G1,G8,n);
if (!strcmp(s,"by knight on C or F file")) SetBlockedFiles(1,C1,C8,F1,F8,n);
if (!strcmp(s,"by knight on D or E file")) SetBlockedFiles(1,D1,D8,E1,E8,n);

if (!strcmp(s,"by bishop on A or H file")) SetBlockedFiles(2,A1,A8,H1,H8,n);
if (!strcmp(s,"by bishop on B or G file")) SetBlockedFiles(2,B1,B8,G1,G8,n);
if (!strcmp(s,"by bishop on C or F file")) SetBlockedFiles(2,C1,C8,F1,F8,n);
if (!strcmp(s,"by bishop on D or E file")) SetBlockedFiles(2,D1,D8,E1,E8,n);

if (!strcmp(s,"by rook on A or H file")) SetBlockedFiles(3,A1,A8,H1,H8,n);
if (!strcmp(s,"by rook on B or G file")) SetBlockedFiles(3,B1,B8,G1,G8,n);
if (!strcmp(s,"by rook on C or F file")) SetBlockedFiles(3,C1,C8,F1,F8,n);
if (!strcmp(s,"by rook on D or E file")) SetBlockedFiles(3,D1,D8,E1,E8,n);

if (!strcmp(s,"by queen on A or H file")) SetBlockedFiles(4,A1,A8,H1,H8,n);
if (!strcmp(s,"by queen on B or G file")) SetBlockedFiles(4,B1,B8,G1,G8,n);
if (!strcmp(s,"by queen on C or F file")) SetBlockedFiles(4,C1,C8,F1,F8,n);
if (!strcmp(s,"by queen on D or E file")) SetBlockedFiles(4,D1,D8,E1,E8,n);

if (!strcmp(s,"by king on A or H file")) SetBlockedFiles(5,A1,A8,H1,H8,n);
if (!strcmp(s,"by king on B or G file")) SetBlockedFiles(5,B1,B8,G1,G8,n);
if (!strcmp(s,"by king on C or F file")) SetBlockedFiles(5,C1,C8,F1,F8,n);
if (!strcmp(s,"by king on D or E file")) SetBlockedFiles(5,D1,D8,E1,E8,n);

//blocking double pawn move
if (!strcmp(s,"by knight on A or H file")) {blocked2[0][1][A2] = n; blocked2[0][1][H2] = n;}
if (!strcmp(s,"by knight on B or G file")) {blocked2[0][1][B2] = n; blocked2[0][1][G2] = n;}
if (!strcmp(s,"by knight on C or F file")) {blocked2[0][1][C2] = n; blocked2[0][1][F2] = n;}
if (!strcmp(s,"by knight on D or E file")) {blocked2[0][1][D2] = n; blocked2[0][1][E2] = n;}

if (!strcmp(s,"by bishop on A or H file")) {blocked2[0][1][A2] = n; blocked2[0][2][H2] = n;}
if (!strcmp(s,"by bishop on B or G file")) {blocked2[0][1][B2] = n; blocked2[0][2][G2] = n;}
if (!strcmp(s,"by bishop on C or F file")) {blocked2[0][1][C2] = n; blocked2[0][2][F2] = n;}
if (!strcmp(s,"by bishop on D or E file")) {blocked2[0][1][D2] = n; blocked2[0][2][E2] = n;}

if (!strcmp(s,"by rook on A or H file")) {blocked2[0][1][A2] = n; blocked2[0][3][H2] = n;}
if (!strcmp(s,"by rook on B or G file")) {blocked2[0][1][B2] = n; blocked2[0][3][G2] = n;}
if (!strcmp(s,"by rook on C or F file")) {blocked2[0][1][C2] = n; blocked2[0][3][F2] = n;}
if (!strcmp(s,"by rook on D or E file")) {blocked2[0][1][D2] = n; blocked2[0][3][E2] = n;}

if (!strcmp(s,"by queen on A or H file")) {blocked2[0][1][A2] = n; blocked2[0][4][H2] = n;}
if (!strcmp(s,"by queen on B or G file")) {blocked2[0][1][B2] = n; blocked2[0][4][G2] = n;}
if (!strcmp(s,"by queen on C or F file")) {blocked2[0][1][C2] = n; blocked2[0][4][F2] = n;}
if (!strcmp(s,"by queen on D or E file")) {blocked2[0][1][D2] = n; blocked2[0][4][E2] = n;}

if (!strcmp(s,"by king on A or H file")) {blocked2[0][1][A2] = n; blocked2[0][5][H2] = n;}
if (!strcmp(s,"by king on B or G file")) {blocked2[0][1][B2] = n; blocked2[0][5][G2] = n;}
if (!strcmp(s,"by king on C or F file")) {blocked2[0][1][C2] = n; blocked2[0][5][F2] = n;}
if (!strcmp(s,"by king on D or E file")) {blocked2[0][1][D2] = n; blocked2[0][5][E2] = n;}

//mobility = done
//empty squares not attacked by pawn
if (!strcmp(s,"by knight")) mobility[2] = n;
if (!strcmp(s,"by bishop")) mobility[3] = n;
if (!strcmp(s,"by rook")) mobility[4] = n; 
if (!strcmp(s,"by queen")) mobility[5] = n; 

//squares attacked by king = done
if (!strcmp(s,"by pawn")) attacks[0][5] = n;
if (!strcmp(s,"by knight")) attacks[1][5] = n; 
if (!strcmp(s,"by bishop")) attacks[2][5] = n;
if (!strcmp(s,"by rook")) attacks[3][5] = n; 
if (!strcmp(s,"by queen")) attacks[4][5] = n; 

if (!strcmp(s,"rook with support")) rook_support = n;
if (!strcmp(s,"queen with support")) queen_support = n;

//attacks on pawns not defended by pawns
if (!strcmp(s,"by knight")) attacks[1][0] = n;
if (!strcmp(s,"by bishop")) attacks[2][0] = n;
if (!strcmp(s,"by rook")) attacks[3][0] = n;
if (!strcmp(s,"by queen")) attacks[4][0] = n;
if (!strcmp(s,"by king")) attacks[5][0] = n;

//pawn attacks = done
if (!strcmp(s,"pawn attacks knight")) attacks[0][1] = n;
if (!strcmp(s,"pawn attacks bishop")) attacks[0][2] = n;
if (!strcmp(s,"pawn attacks rook")) attacks[0][3] = n;
if (!strcmp(s,"pawn attacks queen")) attacks[0][4] = n;

//knight attacks = done
if (!strcmp(s,"knight attacks bishop")) attacks[1][2] = n;
if (!strcmp(s,"knight attacks rook")) attacks[1][3] = n;
if (!strcmp(s,"knight attacks queen")) attacks[1][4] = n;

//bishop attacks = done
if (!strcmp(s,"bishop attacks knight")) attacks[2][1] = n;
if (!strcmp(s,"bishop attacks rook")) attacks[2][3] = n;
if (!strcmp(s,"bishop attacks queen")) attacks[2][4] = n;

//rook attacks = done
if (!strcmp(s,"rook attacks knight")) attacks[3][1] = n;
if (!strcmp(s,"rook attacks bishop")) attacks[3][2] = n;
if (!strcmp(s,"rook attacks queen")) attacks[3][4] = n;

//queen attacks = done
if (!strcmp(s,"queen attacks knight")) attacks[4][1] = n;
if (!strcmp(s,"queen attacks bishop")) attacks[4][2] = n;
if (!strcmp(s,"queen attacks rook")) attacks[4][3] = n;

//king attacks?

//absolute pins
if (!strcmp(s,"bishop pins pawn")) pins[2][0] = n;
if (!strcmp(s,"bishop pins knight")) pins[2][1] = n;
if (!strcmp(s,"bishop pins rook")) pins[2][3] = n;
if (!strcmp(s,"bishop pins queen")) pins[2][4] = n;
if (!strcmp(s,"rook pins pawn")) pins[3][0] = n;
if (!strcmp(s,"rook pins knight")) pins[3][1] = n;
if (!strcmp(s,"rook pins bishop")) pins[3][2] = n;
if (!strcmp(s,"rook pins queen")) pins[3][4] = n;
if (!strcmp(s,"queen pins pawn")) pins[4][0] = n;
if (!strcmp(s,"queen pins knight")) pins[4][1] = n;
if (!strcmp(s,"queen pins bishop")) pins[4][2] = n;
if (!strcmp(s,"queen pins rook")) pins[4][3] = n;

//defends
//pawns = done
if (!strcmp(s,"pawn defends pawn")) defend[0][0] = n;
if (!strcmp(s,"pawn defends knight")) defend[0][1] = n;
if (!strcmp(s,"pawn defends bishop")) defend[0][2] = n;
if (!strcmp(s,"pawn defends rook")) defend[0][3] = n;
if (!strcmp(s,"pawn defends queen")) defend[0][4] = n;

if (!strcmp(s,"knight defends pawn")) defend[1][0] = n;
if (!strcmp(s,"knight defends knight")) defend[1][1] = n;
if (!strcmp(s,"knight defends bishop")) defend[1][2] = n;
if (!strcmp(s,"knight defends rook")) defend[1][3] = n;
if (!strcmp(s,"knight defends queen")) defend[1][4] = n;

if (!strcmp(s,"bishop defends pawn")) defend[2][0] = n;
if (!strcmp(s,"bishop defends knight")) defend[2][1] = n;
if (!strcmp(s,"bishop defends bishop")) defend[2][2] = n;
if (!strcmp(s,"bishop defends rook")) defend[2][3] = n;
if (!strcmp(s,"bishop defends queen")) defend[2][4] = n;

if (!strcmp(s,"rook defends pawn")) defend[3][0] = n;
if (!strcmp(s,"rook defends knight")) defend[3][1] = n;
if (!strcmp(s,"rook defends bishop")) defend[3][2] = n;
if (!strcmp(s,"rook defends rook")) defend[3][3] = n;
if (!strcmp(s,"rook defends queen")) defend[3][4] = n;

if (!strcmp(s,"queen defends pawn")) defend[4][0] = n;
if (!strcmp(s,"queen defends knight")) defend[4][1] = n;
if (!strcmp(s,"queen defends bishop")) defend[4][2] = n;
if (!strcmp(s,"queen defends rook")) defend[4][3] = n;
if (!strcmp(s,"queen defends queen")) defend[4][4] = n;

if (!strcmp(s,"king defends pawn")) defend[5][0] = n;
if (!strcmp(s,"king defends knight")) defend[5][1] = n;
if (!strcmp(s,"king defends bishop")) defend[5][2] = n;
if (!strcmp(s,"king defends rook")) defend[5][3] = n;
if (!strcmp(s,"king defends queen")) defend[5][4] = n;

//pawns
//pawn majorities
if (!strcmp(s,"doubled majority")) doubled_majority = n;//done
if (!strcmp(s,"backward majority")) backward_majority = n;

//weak pawns = done
if (!strcmp(s,"isolated pawn on rank 2")) for(x=A2;x<=H2;x++) isolated[0][x] += n;
if (!strcmp(s,"isolated pawn on rank 3")) for(x=A3;x<=H3;x++) isolated[0][x] += n;
if (!strcmp(s,"isolated pawn on rank 4")) for(x=A4;x<=H4;x++) isolated[0][x] += n;
if (!strcmp(s,"isolated pawn on rank 5")) for(x=A5;x<=H5;x++) isolated[0][x] += n;
if (!strcmp(s,"isolated pawn on rank 6")) for(x=A6;x<=H6;x++) isolated[0][x] += n;
if (!strcmp(s,"isolated pawn on rank 7")) for(x=A7;x<=H7;x++) isolated[0][x] += n;

if (!strcmp(s,"backward pawn on rank 2")) for(x=A2;x<=H2;x++) backward[0][x] += n;
if (!strcmp(s,"backward pawn on rank 3")) for(x=A3;x<=H3;x++) backward[0][x] += n;
if (!strcmp(s,"backward pawn on rank 4")) for(x=A4;x<=H4;x++) backward[0][x] += n;
if (!strcmp(s,"backward pawn on rank 5")) for(x=A5;x<=H5;x++) backward[0][x] += n;

if (!strcmp(s,"doubled pawn on rank 3")) 
	for(x=A3;x<=H3;x++) doubled[0][x] += n;
if (!strcmp(s,"doubled pawn on rank 4")) 
	for(x=A4;x<=H4;x++) doubled[0][x] += n;
if (!strcmp(s,"doubled pawn on rank 5")) 
	for(x=A5;x<=H5;x++) doubled[0][x] += n;
			
if (!strcmp(s,"isolated open file bonus")) isolated_open = n;//d
if (!strcmp(s,"backward open file bonus")) backward_open = n;//d

//outpost = done for knight
if (!strcmp(s,"knight on outpost")) outpost[1] = n;
if (!strcmp(s,"bishop on outpost")) outpost[2] = n;
if (!strcmp(s,"rook on outpost")) outpost[3] = n;
if (!strcmp(s,"queen on outpost")) outpost[4] = n;
if (!strcmp(s,"king on outpost")) outpost[5] = n;

//passed pawns = done
if (!strcmp(s,"passed pawn on rank 2")) for(x=A2;x<=H2;x++) passed[0][x] += n;
if (!strcmp(s,"passed pawn on rank 3")) for(x=A3;x<=H3;x++) passed[0][x] += n;
if (!strcmp(s,"passed pawn on rank 4")) for(x=A4;x<=H4;x++) passed[0][x] += n;
if (!strcmp(s,"passed pawn on rank 5")) for(x=A5;x<=H5;x++) passed[0][x] += n;
if (!strcmp(s,"passed pawn on rank 6")) for(x=A6;x<=H6;x++) passed[0][x] += n;
if (!strcmp(s,"passed pawn on rank 7")) for(x=A7;x<=H7;x++) passed[0][x] += n;

if (!strcmp(s,"supported passed pawn on rank 3")) for(x=A3;x<=H3;x++) supported[0][x] += n;
if (!strcmp(s,"supported passed pawn on rank 4")) for(x=A4;x<=H4;x++) supported[0][x] += n;
if (!strcmp(s,"supported passed pawn on rank 5")) for(x=A5;x<=H5;x++) supported[0][x] += n;
if (!strcmp(s,"supported passed pawn on rank 6")) for(x=A6;x<=H6;x++) supported[0][x] += n;
if (!strcmp(s,"supported passed pawn on rank 7")) for(x=A7;x<=H7;x++) supported[0][x] += n;

if (!strcmp(s,"connected passed pawn on rank 3")) for(x=A3;x<=H3;x++) connected[0][x] += n;
if (!strcmp(s,"connected passed pawn on rank 4")) for(x=A4;x<=H4;x++) connected[0][x] += n;
if (!strcmp(s,"connected passed pawn on rank 5")) for(x=A5;x<=H5;x++) connected[0][x] += n;
if (!strcmp(s,"connected passed pawn on rank 6")) for(x=A6;x<=H6;x++) connected[0][x] += n;
if (!strcmp(s,"connected passed pawn on rank 7")) for(x=A7;x<=H7;x++) connected[0][x] += n;

if (!strcmp(s,"adjacent pawn on rank 3")) for(x=A3;x<=H3;x++) adjacent[0][x] += n;
if (!strcmp(s,"adjacent pawn on rank 4")) for(x=A4;x<=H4;x++) adjacent[0][x] += n;
if (!strcmp(s,"adjacent pawn on rank 5")) for(x=A5;x<=H5;x++) adjacent[0][x] += n;
if (!strcmp(s,"adjacent pawn on rank 6")) for(x=A6;x<=H6;x++) adjacent[0][x] += n;
if (!strcmp(s,"adjacent pawn on rank 7")) for(x=A7;x<=H7;x++) adjacent[0][x] += n;

//piece combinations
if (!strcmp(s,"bishop pair")) bishop_pair = n;//
if (!strcmp(s,"opposite colored bishops percentage")) opposite_coloured_bishops = n;

//rooks
if (!strcmp(s,"open file")) open_file = n;
if (!strcmp(s,"half-open file"))  half_open_file = n;
if (!strcmp(s,"rook behind passed pawn"))  rook_behind = n;

//trapped pieces
if (!strcmp(s,"trapped knight"))  trapped[1] = n;
if (!strcmp(s,"trapped bishop"))  trapped[2] = n;
if (!strcmp(s,"trapped rook"))  trapped[3] = n;
if (!strcmp(s,"trapped queen"))  trapped[4] = n;
if (!strcmp(s,"trapped king"))  trapped[5] = n;

//king defence
if (!strcmp(s,"defence on A2 or H2")) 
{queenside_score[0][A2] = n; kingside_score[0][H2] = n;}
if (!strcmp(s,"defence on B2 or G2")) 
{queenside_score[0][B2] = n; kingside_score[0][G2] = n;} 
if (!strcmp(s,"defence on C2 or F2")) 
{queenside_score[0][C2] = n; kingside_score[0][F2] = n;}  
if (!strcmp(s,"defence on A3 or H3")) 
{queenside_score[0][A3] = n; kingside_score[0][H3] = n;}
if (!strcmp(s,"defence on B3 or G3")) 
{queenside_score[0][B3] = n; kingside_score[0][G3] = n;}
if (!strcmp(s,"defence on C3 or F3")) 
{queenside_score[0][C3] = n; kingside_score[0][F3] = n;}
if (!strcmp(s,"defence on A4 or H4")) 
{queenside_score[0][A4] = n; kingside_score[0][H4] = n;}
if (!strcmp(s,"defence on B4 or G4")) 
{queenside_score[0][B4] = n; kingside_score[0][G4] = n;}
if (!strcmp(s,"defence on C4 or F4"))
{queenside_score[0][C4] = n; kingside_score[0][F4] = n;}

//uncastled
if (!strcmp(s,"moved king"))  no_castle = n;
if (!strcmp(s,"moved queen rook"))  no_q_castle = n;
if (!strcmp(s,"moved king rook"))  no_q_castle = n;
}
a[0] = 0;
pb();
return 0;
}
//new scores after castling

void SetBlockedFiles(int p,int start,int end,int start2,int end2,int n)
{
	for(int x=start;x<=end;x+=8)
		blocked[0][p][x] += n;
	for(int x=start2;x<=end2;x+=8)
		blocked[0][p][x] += n;
}

void SetPassedFiles(int p,int start,int end,int start2,int end2,int n)
{
	for(int x=start;x<=end;x+=8)
		passed_pawn[p][x] += n;
	for(int x=start2;x<=end2;x+=8)
		passed_pawn[p][x] += n;
}

void SetPieceFiles(int p,int start,int end,int start2,int end2,int n)
{
	for(int x=start;x<=end;x+=8)
		piece_score[0][p][x] += n;
	for(int x=start2;x<=end2;x+=8)
		piece_score[0][p][x] += n;
}

void ClearData()
{
memset(piece_value,0,sizeof(piece_value));
memset(piece_score,0,sizeof(piece_score));

memset(mobility,0,sizeof(mobility));
memset(outpost,0,sizeof(outpost));

memset(attacks,0,sizeof(attacks));
memset(pins,0,sizeof(pins));
memset(trapped,0,sizeof(trapped));

memset(blocked,0,sizeof(blocked));
memset(blocked2,0,sizeof(blocked2));

memset(isolated,0,sizeof(isolated));
memset(backward,0,sizeof(backward));
memset(doubled,0,sizeof(doubled));

memset(passed_pawn,0,sizeof(passed_pawn));
memset(supported,0,sizeof(supported));
memset(connected,0,sizeof(connected));

memset(kingside_score,0,sizeof(kingside_score));
memset(queenside_score,0,sizeof(queenside_score));
}

void pb()
{
//	int blocked[2][6][64];
//int blocked2[2][6][64];
/*
for(int p=1;p<7;p++)
	{
		
	for(int x=0;x<64;x++)
	{
		printf(" %d ",piece_score[0][p][x]);
		if(x>0 && (x+1)%8==0) printf("\n");
	}
	printf("\n\n");
	}*/
/*
	for(int x=0;x<64;x++)
	{
		printf(" %d ",connected[0] [x]);
		if(x>0 && (x+1)%8==0) printf("\n");
	}
	printf("connected\n\n");
*/

}