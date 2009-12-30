//
// BingoBoard.hpp
//
// Copyright 2007 Tournament Games, Inc.
// $Id: $
//
#ifndef __BINGO_BOARD_HPP__

#define __BINGO_BOARD_HPP__
#include "stdafx.h"

class BingoBoard
    {
        public:

            typedef int Board[5][5];
            typedef bool Dauber[5][5];
            static const int BoardDimension = 5; // B-I-N-G-O.
            bool PostageStampHit;
            bool BingoMarkers[12];

            BingoBoard();                // Random.
            void InitBoard();
            BingoBoard(int board[5][5]); // 5x5, no value can be used more than once.
            void
            Reset()

            throw ();

            void
            GetBoard(Board board/*OUTPUT*/)const throw();

            void
            GetDauber(Dauber dauber/*OUTPUT*/)const throw();
    //
    // Returns the number of bingos this new ball creates.
    //
    int NewBall(int value) throw();

        private:
    // Called only after (x, y) has been initially set.
    //  Does not validate (x, y), for performance reasons.
    int CountNewHorizontalBingos(int x, int y) throw();
    int CountNewVerticalBingos(int x, int y) throw();
    int CountNewDiagonalBingos(int x, int y) throw();
    int CountNewTBingos(int x, int y) throw();
    int CountNewPostageStampBingos(int x, int y) throw();

            // False means the number hasn't been hit yet.
            bool _dauber[BoardDimension][BoardDimension];
            int _board[BoardDimension][BoardDimension];
    };

#endif // !defined(__PINBALL_BINGO_BOARD_HPP__)
