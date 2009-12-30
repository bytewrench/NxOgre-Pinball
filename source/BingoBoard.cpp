//
// BingoBoard.cpp
//
// Copyright 2007 Tournament Games Inc
//

static const char * s_versionControlString = "$Id: LocationID.cpp 856 2007-04-12 19:24:07Z mikeb $";

#include "stdafx.h"

BingoBoard::BingoBoard () { }

void BingoBoard::InitBoard ()
{
	PostageStampHit = false;
	srand(static_cast<unsigned int>(time(NULL)));
	int i;

	for ( int x = 0; x < BoardDimension; ++ x )
	{
		std::vector<int> balls;

		for ( i = 1; i < 16; ++ i )
		{
			balls.push_back(i);
		}

		for ( int y = 0; y < BoardDimension; ++ y )
		{
			i = rand() % static_cast<int>(balls.size());
			_board[x][y] = (x * 15) + balls[i];
			std::vector<int>::iterator it = balls.begin();
			it += i;
			balls.erase(it);
		}
	}

	for ( int i = 0; i < 12; i ++ )
	{
		BingoMarkers[i] = false;
	}

	Reset();
}

BingoBoard::BingoBoard ( int board[5][5] )
{
	(void)memcpy(_board, board, sizeof(_board));
}

void
BingoBoard::GetBoard(Board board/*OUTPUT*/) const throw()
{
	(void)memcpy(board, _board, sizeof(_board));
}

void
BingoBoard::GetDauber(Dauber dauber/*OUTPUT*/) const throw()
{
	(void)memcpy(dauber, _dauber, sizeof(_dauber));
}

void
BingoBoard::Reset() throw()
{
	(void)memset(_dauber, 0, sizeof(_dauber));
}


int
BingoBoard::NewBall(int value) throw()
{
	for ( int x = 0; x < BoardDimension; ++ x )
	{
		for ( int y = 0; y < BoardDimension; ++ y )
		{
			if ( value == _board[x][y] )
			{
				if ( ! _dauber[x][y] )
				{
					_dauber[x][y] = true;

					int newbingos = (CountNewHorizontalBingos(x, y)
						+ CountNewVerticalBingos(x, y)
						+ CountNewDiagonalBingos(x, y));

					if ( newbingos > 0 )
					{
						return newbingos;
					}
					else
					{
						return - 1; /*daub*/
					}
				}
			}
		}
	}

	return 0;
}


int
BingoBoard::CountNewHorizontalBingos(int x, int y) throw()
{
	for ( x = 0; x < BoardDimension; ++ x )
	{
		if ( ! _dauber[x][y] )
		{
			return 0;
		}
	}

	switch ( y )
	{
	case 0:
		BingoMarkers[HORIZ_0] = true;
		break;

	case 1:
		BingoMarkers[HORIZ_1] = true;
		break;

	case 2:
		BingoMarkers[HORIZ_2] = true;
		break;

	case 3:
		BingoMarkers[HORIZ_3] = true;
		break;

	case 4:
		BingoMarkers[HORIZ_4] = true;
		break;
	}

	return 1;
}


int
BingoBoard::CountNewVerticalBingos(int x, int y) throw()
{
	for ( y = 0; y < BoardDimension; ++ y )
	{
		if ( ! _dauber[x][y] )
		{
			return 0;
		}
	}

	switch ( x )
	{
	case 0:
		BingoMarkers[VERT_0] = true;
		break;

	case 1:
		BingoMarkers[VERT_1] = true;
		break;

	case 2:
		BingoMarkers[VERT_2] = true;
		break;

	case 3:
		BingoMarkers[VERT_3] = true;
		break;

	case 4:
		BingoMarkers[VERT_4] = true;
		break;
	}

	return 1;
}


int
BingoBoard::CountNewDiagonalBingos(int x, int y) throw()
{
	int bingoCount = 0;
	bool bingo;

	if ( (x != y) && ((x + y) != (BoardDimension - 1)) )
	{
		return 0; // Not on a diagonal.
	}

	// The \ diagonal.
	if ( x == y )
	{
		bingo = true;

		for ( int i = 0; bingo && (i < BoardDimension); ++ i )
		{
			if ( ! _dauber[i][i] )
			{
				bingo = false;
			}
		}

		if ( bingo )
		{
			bingoCount ++;
			BingoMarkers[DIAG_0] = true;
		}
	}

	// The / diagonal.
	if ( (x + y) == (BoardDimension - 1) )
	{
		bingo = true;

		for ( int i = 0; bingo && (i < BoardDimension); ++ i )
		{
			if ( ! _dauber[i][BoardDimension - 1 - i] )
			{
				bingo = false;
			}
		}

		if ( bingo )
		{
			bingoCount ++;
			BingoMarkers[DIAG_1] = true;
		}
	}

	return bingoCount;
}


int
BingoBoard::CountNewTBingos(int x, int y) throw()
{
	for ( int x = 0; x < 5; x ++ )
	{
		if ( ! _dauber[x][0] )
		{
			return 0;
		}
	}

	for ( int y = 0; y < 5; y ++ )
	{
		if ( ! _dauber[2][y] )
		{
			return 0;
		}
	}

	return 1;
}


int
BingoBoard::CountNewPostageStampBingos(int x, int y) throw()
{
	if ( PostageStampHit )
	{
		return 0;
	}

	if ( (_dauber[0][0]) && (_dauber[0][4]) && (_dauber[4][0]) && (_dauber[4][4]) )
	{
		PostageStampHit = true;
		return 1;
	}

	else
	{
		return 0;
	}
}
