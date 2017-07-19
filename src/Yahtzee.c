#include "Yahtzee.h"

void YahtzeeReset(struct Yahtzee* pYahtzee)
{
	ResetScore( &pYahtzee->m_Score );
	YahtzeeResetTurn(pYahtzee);
}

void YahtzeeResetTurn(struct Yahtzee* pYahtzee)
{
	pYahtzee->m_nRollNumber = 0;
	ResetYahtzeeDice( &pYahtzee->m_Dice );
}

void YahtzeeRoll(struct Yahtzee* pYahtzee)
{
	if( pYahtzee->m_nRollNumber >= ROLL_LIMIT )
		return;
	
	pYahtzee->m_nRollNumber++;
	RollYahtzeeDice( &pYahtzee->m_Dice);
}

void YahtzeeHoldDice(struct Yahtzee* pYahtzee, int nDiceIndex, int bHold)
{
	if( pYahtzee->m_nRollNumber <= 0 )
		return;
	
	HoldYahtzeeDice( &pYahtzee->m_Dice, nDiceIndex, bHold);
}

void ScoreSpot(struct Yahtzee* pYahtzee, enum ScoreCategory eCategory)
{
	if( GetValueSupplied(&pYahtzee->m_Score, eCategory) || pYahtzee->m_nRollNumber <= 0 )
		return;

	HandleIfYahtzee(&pYahtzee->m_Score, &pYahtzee->m_Dice);
	
	SetScore(&pYahtzee->m_Score, eCategory, ScoreForCategory(eCategory, &pYahtzee->m_Dice));
	
	ResetYahtzeeDice( &pYahtzee->m_Dice);
	pYahtzee->m_nRollNumber = 0;
}

int IsGameOver(struct Yahtzee* pYahtzee)
{
	return !AnyCategoriesNotSupplied(&pYahtzee->m_Score);
}

int GetRollNumber(struct Yahtzee* pYahtzee)
{
	return pYahtzee->m_nRollNumber;
}

int YahtzeeGetDiceHolded(struct Yahtzee* pYahtzee, int nDiceIndex)
{
	return GetYahtzeeDiceHolded(&pYahtzee->m_Dice, nDiceIndex);
}

int YahtzeeGetDiceValue(struct Yahtzee* pYahtzee, int nDiceIndex)
{
	return GetYahtzeeDiceValue(&pYahtzee->m_Dice, nDiceIndex);
}

int YahtzeeGetScore(struct Yahtzee* pYahtzee, enum ScoreCategory eCategory)
{
	return GetScoreEntered(pYahtzee, eCategory) ? GetScore(&pYahtzee->m_Score, eCategory) : ScoreForCategory(eCategory, &pYahtzee->m_Dice);
}

int GetScoreEntered(struct Yahtzee* pYahtzee, enum ScoreCategory eCategory)
{
	return GetValueSupplied(&pYahtzee->m_Score, eCategory);
}
int YahtzeeGetTotalNumbers(struct Yahtzee* pYahtzee) 	{ return GetTotalNumbers(&pYahtzee->m_Score); }
int YahtzeeGetTotalBonus(struct Yahtzee* pYahtzee) 	{ return GetBonus(&pYahtzee->m_Score); 		}
int YahtzeeGetTotalUpper(struct Yahtzee* pYahtzee) 	{ return GetTotalUpper(&pYahtzee->m_Score); 	}
int YahtzeeGetYahtzeeBonus(struct Yahtzee* pYahtzee) 	{ return GetYahtzeeBonus(&pYahtzee->m_Score); }
int YahtzeeGetTotalLower(struct Yahtzee* pYahtzee) 	{ return GetTotalLower(&pYahtzee->m_Score); 	}
int YahtzeeGetTotal(struct Yahtzee* pYahtzee) 			{ return GetTotal(&pYahtzee->m_Score); 		}
