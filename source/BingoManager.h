#ifndef _BINGOMANAGER

#define _BINGOMANAGER

#include "stdafx.h"

class BingoManager
    {
        public:

			BingoManager();
			~BingoManager();
            static BingoManager * getSingleton ();
			int BINGOCOUNT;
            void initBoard();
            bool resetCard(int lastHit);
			void initBingoMarkers();
			BingoMarker * bingo_marker[12];
            BingoBoard b;
            BingoBoard::Board board;
            BingoBoard::Dauber dauber;

        protected:
        private:
    };

#endif
