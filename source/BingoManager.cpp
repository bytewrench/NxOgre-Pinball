#include "stdafx.h"

BingoManager::BingoManager () { 
BINGOCOUNT=0;
};

BingoManager::~BingoManager () { 
	for ( int i = 0; i < 12; i ++ )
	{
		delete bingo_marker[i];
	}
};
BingoManager * BingoManager::getSingleton()
{
	static BingoManager * bingoMan;

	if ( bingoMan == 0 )
	{
		bingoMan = new BingoManager();
	}

	return bingoMan;
}

void BingoManager::initBoard(){
	b.InitBoard();
	b.GetBoard(board);
	String myBoard;
	int thisNumber;
	for ( int x = 0; x < 5; x ++ )
	{
		for ( int y = 0; y < 5; y ++ )
		{
			thisNumber = board[x][y];
			myBoard += "[" + StringConverter::toString(thisNumber) + "]";
		}
	}

	gamedata::getSingleton()->logMessage(myBoard);
}

bool BingoManager::resetCard(int lastHit)
	{
		
		bool gotDauber = false;
		int positionCounter = 1;
		Vector3 pos;
		float initZ = 2.82f;
		pos = Vector3(- 4.05f, .03f, initZ);
		bool endReached = false;

		int thisNumber = - 1;
		int CurrRow = 0;
		int i = 1;

		
		int newBingoCount = b.NewBall(lastHit);

		if ( newBingoCount > 0 )
		{
			BINGOCOUNT += newBingoCount;
			gamedata::getSingleton()->logMessage("BINGO=" + StringConverter::toString(BINGOCOUNT));
			int addscore;

			switch ( BINGOCOUNT )
			{
			case 1:
				addscore = SCORE_BINGO_MULTIPLIER;
				break;

			case 2:
				addscore = SCORE_BINGO_MULTIPLIER * 2;
				break;

			case 3:
				addscore = SCORE_BINGO_MULTIPLIER * 4;
				break;

			case 4:
				addscore = SCORE_BINGO_MULTIPLIER * 8;
				break;

			case 5:
				addscore = SCORE_BINGO_MULTIPLIER * 16;
				break;

			case 6:
				addscore = SCORE_BINGO_MULTIPLIER * 32;
				break;

			case 7:
				addscore = SCORE_BINGO_MULTIPLIER * 64;
				break;

			case 8:
				addscore = SCORE_BINGO_MULTIPLIER * 120;
				break;

			case 9:
				addscore = SCORE_BINGO_MULTIPLIER * 240;
				break;

			case 10:
				addscore = SCORE_BINGO_MULTIPLIER * 480;
				break;

			case 11:
				addscore = SCORE_BINGO_MULTIPLIER * 960;
				break;

			case 12:
				addscore = SCORE_BINGO_MULTIPLIER * 1920;
				break;
			}

			
			SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_BINGO);
			ScoreManager::getScoreManager()->addToScore(addscore, ScoreType::_BINGO_MULTIPLIER);
			BubbleNotify::getSingleton()->setMessage(StringConverter::toString(addscore) + "!", 25, 10);

			for ( int i = 0; i < 12; i ++ )
			{
				if ( b.BingoMarkers[i] && (! bingo_marker[i]->_visible) )
				{
					bingo_marker[i]->show();
					Vector3 scale = Vector3(1.2f, 1.2f, 1.2f);

					if ( i == 10 || i == 11 )
					{
						bingo_marker[i]->setScale(scale);
					}
				}
			}
		}

		else
		{
			if ( newBingoCount == - 1 )
			{
				gotDauber = true;
			}
		}
		
		b.GetDauber(dauber);
		
		for ( int x = 0; x < 5; ++ x )
		{
			
			for ( int y = 0; y < 5; ++ y )
			{
			
				thisNumber = board[x][y];
			
				if ( ! dauber[x][y] )
				{
			
					QuadManager::BingoCardQuadSet[i].init(positionCounter, thisNumber, pos, i);
				}
				else
				{
			
					QuadManager::BingoCardQuadSet[i].daub();

					if ( (newBingoCount == - 1) && (thisNumber == lastHit) )
					{
						SoundManager::getSoundManager()->Play_Sound(SoundManager::SOUND_DAUBER);
						ScoreManager::getScoreManager()->addToScore(SCORE_DAUBER,ScoreType::_DAUBER);
						BubbleNotify::getSingleton()->setMessage("Got One!", 20, 2);
						DAUBERCOUNT ++;
						gamedata::getSingleton()->logMessage("DAUB=" + StringConverter::toString(lastHit));
					}
				}

				++ i;
				++ CurrRow;
				pos.z += 1.67f;

				if ( CurrRow > 4 )
				{
					CurrRow = 0;
					pos.x += 1.62f;
					pos.z = initZ;
				}

				positionCounter += 2;
			}
		}
		
		return gotDauber;
	}


void BingoManager::initBingoMarkers ()
	{
		Vector3 basePos;
		basePos.x = - .8f;
		basePos.y = .25f;
		basePos.z = 3.0f;

		for ( int i = 0; i < 5; i ++ )
		{
			loadinganimation::getSingleton()->increment();
			bingo_marker[i] = new BingoMarker(basePos, 0.0f);
			basePos.z += 1.6f;
		}

		basePos.x = - 4.0f;
		basePos.y = .26f;
		basePos.z = 6.25f;

		for ( int i = 5; i < 10; i ++ )
		{
			loadinganimation::getSingleton()->increment();
			bingo_marker[i] = new BingoMarker(basePos, 90.0f);
			basePos.x += 1.6f;
		}

		basePos.x = - .8f;
		basePos.y = .27f;
		basePos.z = 6.0f;
		bingo_marker[10] = new BingoMarker(basePos, 45.0f);
		bingo_marker[11] = new BingoMarker(basePos, - 45.0f);
	}


