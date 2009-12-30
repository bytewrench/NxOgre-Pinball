#ifndef _RECENTSCORES
#define _RECENTSCORES

#include "stdafx.h"


class recentScoreManager
{
public:
	numberMachine * nmItem[10];
	numberMachine * nmLastScore;
	recentScoreManager();
	static recentScoreManager * getRecentScoreManager();
	~recentScoreManager();
	void update(int addValue,Vector3 col);
	void display();
	void reset();
protected:
private:

	struct RECENT_SCORES{
		int score;
		Vector2 pos;
		float alpha;
		Vector3 color;
	} g_RecentScores[10];
	static recentScoreManager * recentSM;
	float SCORECARD_X;
	float SCORECARD_Y;
	int MAX_SCORECARD_NUMBER;
};

#endif