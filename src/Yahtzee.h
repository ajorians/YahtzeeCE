#ifndef YAHTZEE_H
#define YAHTZEE_H

#include "YahtzeeDice.h"
#include "YahtzeeScore.h"

#define ROLL_LIMIT	3

struct Yahtzee;

void YahtzeeReset(struct Yahtzee* pYahtzee);
void YahtzeeResetTurn(struct Yahtzee* pYahtzee);
void YahtzeeRoll(struct Yahtzee* pYahtzee);
void YahtzeeHoldDice(struct Yahtzee* pYahtzee, int nDiceIndex, int bHold);
void ScoreSpot(struct Yahtzee* pYahtzee, enum ScoreCategory eCategory);

int IsGameOver(struct Yahtzee* pYahtzee);

int GetRollNumber(struct Yahtzee* pYahtzee);
int YahtzeeGetDiceHolded(struct Yahtzee* pYahtzee, int nDiceIndex);//Whether a dice is pull and not to be rolled
int YahtzeeGetDiceValue(struct Yahtzee* pYahtzee, int nDiceIndex);//The number value that dice has
int YahtzeeGetScore(struct Yahtzee* pYahtzee, enum ScoreCategory eCategory);//Score of spot and if not taken then what would be if taken
int GetScoreEntered(struct Yahtzee* pYahtzee, enum ScoreCategory eCategory);//Whether a spot was taken
int YahtzeeGetTotalNumbers(struct Yahtzee* pYahtzee);
int YahtzeeGetTotalBonus(struct Yahtzee* pYahtzee);
int YahtzeeGetTotalUpper(struct Yahtzee* pYahtzee);
int YahtzeeGetYahtzeeBonus(struct Yahtzee* pYahtzee);
int YahtzeeGetTotalLower(struct Yahtzee* pYahtzee);
int YahtzeeGetTotal(struct Yahtzee* pYahtzee);

//A data organizing class for Yahtzee
struct Yahtzee
{
	struct YahtzeeDice m_Dice;
	struct YahtzeeScore m_Score;
	int m_nRollNumber;
};

#endif
