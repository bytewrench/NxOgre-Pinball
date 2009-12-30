#ifndef _GAMEDATA

#define _GAMEDATA
#include "stdafx.h"

class gamedata
    {
        public:

            gamedata();
			

            static gamedata * getSingleton ();
            void endGame(float FPS);
			void beginGame();
            void logMessage(String message);
            
			void sendToServer(String PlayerInitials);
            String getLog();
			
			void setToDefault();
			void sendGameOver(DWORD dwScore);
			void SendGameDataItem ( LPCSTR lpitemName, LPCSTR value );
			void LoadGameDetailsFromServer ();
			void LoadStaticSettings ();
			void getStandings();
		

			int NUMBALLS;	
			int CURRENTBALL;
			int ENDSCORE;
			int ENDLEVEL;
			int HIGHSCORE;
			int NUMBUMPS;
			int MAXBUMPS;
			int CURRENTBUMP;
			int FPS;

			int ALLNUMCOUNT;
			int LOWSCORE;
			int LASTSCORE;
			String PlayerInitials;
			bool sendLogToServer;
			bool fileSent;
			string szPlayerInitials;
			std::vector<std::string> szVecStandings;
			std::vector<std::string> szVecStandingsUserNames;
			std::vector<std::string> szVecStandingsScores;
			DWORD endTime;
			DWORD startTime;
			DWORD gameTime;
			bool InetOK;
			bool scoreposted;
			bool BETAMODE;
        protected:
        private:
		CHAR _fileName[MAX_PATH];
		

			String _datapacket;
			String getCurrentTime();
    };

#endif
