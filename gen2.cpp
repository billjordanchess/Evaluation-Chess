/*
void GenPawn(const int x)
{
	if(board[x+ForwardSquare[side]] == EMPTY)
	{
		AddMove(x,x + ForwardSquare[side]);
		if(rank[side][x]==1 && board[x + Double[side]] == EMPTY)
		{
			AddMove(x,x + Double[side]);
		}
	}
	if(col[x] > 0 && color[x + Left[side]] == OtherSide[side])
	{
		AddCapture(x,x + Left[side],px[board[x + Left[side]]]);
	}
	if(col[x] < 7 && color[x + Right[side]] == OtherSide[side])
	{
		AddCapture(x,x + Right[side],px[board[x + Right[side]]]);
	}
}

void GenKnight(const int sq)
{
int k = 0;
int sq2 = knight_moves[sq][k++];
while(sq2 > -1)
{
	if(color[sq2] == EMPTY)
		AddMove(sq,sq2);
	else if(color[sq2] == xside)
		AddCapture(sq,sq2,nx[board[sq2]]);
	sq2 = knight_moves[sq][k++];
}
}

void GenBishop(const int x,const int dir)
{
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
			AddCapture(x,sq,bx[board[sq]]);
		break;
	}
	AddMove(x,sq);
	sq = qrb_moves[sq][dir];
}

}

void GenRook(const int x,const int dir)
{
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
		{
			AddCapture(x,sq,rx[board[sq]]);
		}
		break;
	}
	AddMove(x,sq);
	sq = qrb_moves[sq][dir];
}

}

void GenQueen(const int x,const int dir)
{
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
		{
			AddCapture(x,sq,qx[board[sq]]);
		}
		break;
	}
	AddMove(x,sq);
	sq = qrb_moves[sq][dir];
}

}

void GenKing(const int x)
{
int k = 0;
int sq = king_moves[x][k++];

while(sq > -1)
{
//	if(sq==F7 && color[F7]==1)
//		DisplayBoard();
	if(color[sq] == EMPTY)
		AddMove(x,sq);
	else if(color[sq] == xside)
		AddCapture(x,sq,kx[board[sq]]);
	sq = king_moves[x][k++];
}

}

void AddMove(const int x,const int sq)
{
	move_list[mc].start = x;
	move_list[mc].dest = sq;
	move_list[mc].score = history[x][sq];
	mc++;
}

void AddCapture(const int x,const int sq,const int score)
{
	move_list[mc].start = x;
	move_list[mc].dest = sq;
	move_list[mc].score = score + CAPTURE_SCORE;
	mc++;
}

void CapPawn(const int x)
{	

if(col[x] > 0 && color[x + Left[side]] == OtherSide[side])
{
	AddCapture(x,x + Left[side],px[board[x + Left[side]]]);
}
if(col[x] < 7 && color[x + Right[side]] == OtherSide[side])
{
	AddCapture(x,x + Right[side],px[board[x + Right[side]]]);
}

}

void CapKnight(const int x)
{
int k = 0;
int sq = knight_moves[x][k++];
while(sq > -1)
{
	if(color[sq] == xside)
		AddCapture(x,sq,nx[board[sq]]);
	sq = knight_moves[x][k++];
}
}

void CapBishop(const int x,const int dir)
{
		
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
			AddCapture(x,sq,bx[board[sq]]);
		break;
	}
	sq = qrb_moves[sq][dir];
}

}

void CapRook(const int x,const int dir)
{
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
		{
			AddCapture(x,sq,rx[board[sq]]);
		}
		break;
	}
	sq = qrb_moves[sq][dir];
}

}

void CapQueen(const int x,const int dir)
{
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
		{
			AddCapture(x,sq,qx[board[sq]]);
		}
		break;
	}
	sq = qrb_moves[sq][dir];
}

}

void CapKing(const int x)
{
	int k = 0;
	int sq = king_moves[x][k++];

	while(sq > -1)
	{
		if(color[sq] == xside)
			AddCapture(x,sq,kx[board[sq]]);
		sq = king_moves[x][k++];
	}
}
*/