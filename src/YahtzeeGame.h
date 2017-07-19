#ifndef YAHTZEEGAME_H
#define YAHTZEEGAME_H

enum SelectorSpot
{
	PlayAgainSpot = -1,
	RollSpot = 0,//And Quit spot
	AcesSpot,
	TwosSpot,
	ThreesSpot,
	FoursSpot,
	FivesSpot,
	SixesSpot,
	ThreeOfAKindSpot,
	FourOfAKindSpot,
	FullHouseSpot,
	SmStraightSpot,
	LgStraightSpot,
	YahtzeeSpot,
	ChanceSpot,
	Dice1Spot,
	Dice2Spot,
	Dice3Spot,
	Dice4Spot,
	Dice5Spot,
	MAXSPOT
};

void InitYahtzeeGame();
int Loop();//False means exit game
int HandleGameKeyPresses();//True means continue; False means quit
void DisplayBoard();
void DrawSelector(enum SelectorSpot eSpot, int bON);
void DrawScoreValue(int nX, int nY, int nValue, int bUsed);
void DrawDice(int nX, int nY, int nDiceSideLength, int nNumber);
void DrawBox(int nX, int nY, int nWidth, int nHeight, int c);



#endif
