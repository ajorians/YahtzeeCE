#include "YahtzeeScore.h"

#include "YahtzeeDice.h"

void ResetScoreField( struct ScoreField* pScore )
{
	pScore->m_bValueSupplied = 0;
	pScore->m_nValue = 0;
}

void SetScoreField(struct ScoreField* pScore, int nScore)
{
	pScore->m_bValueSupplied = 1;
	pScore->m_nValue = nScore;
}

int GetScoreField(struct ScoreField* pScore)
{
	if( 0 == pScore->m_bValueSupplied)
		return 0;
	return pScore->m_nValue;
}

int GetValueSuppliedField(struct ScoreField* pScore)
{
	return pScore->m_bValueSupplied;
}

void ResetScore(struct YahtzeeScore* pScore)
{
	int i=0;
	for(; i<YAHTZEE_SCORE_FIELDS; i++)
		ResetScoreField( &pScore->m_Scores[i] );
	pScore->m_nYahtzeeBonus = 0;
}

void SetScore(struct YahtzeeScore* pScore, enum ScoreCategory eCategory, int nScore)
{
	int nIndex = (int)eCategory;
	if( nIndex < 0 || nIndex > YAHTZEE_SCORE_FIELDS )
		return;
	
	return SetScoreField( &pScore->m_Scores[nIndex], nScore );
}

int GetScore(struct YahtzeeScore* pScore, enum ScoreCategory eCategory)
{
	int nIndex = (int)eCategory;
	if( nIndex < 0 || nIndex > YAHTZEE_SCORE_FIELDS )
		return 0;
	
	return GetScoreField( &pScore->m_Scores[nIndex] );
}

int GetValueSupplied(struct YahtzeeScore* pScore, enum ScoreCategory eCategory)
{
	int nIndex = (int)eCategory;
	if( nIndex < 0 || nIndex > YAHTZEE_SCORE_FIELDS )
		return 0;
	
	return GetValueSuppliedField( &pScore->m_Scores[nIndex] );
}

void HandleIfYahtzee(struct YahtzeeScore* pScore, struct YahtzeeDice* pDice)
{
	//So if you entered a non-zero for the yahtzee field then you get a bonus for each yahtzee
	if( IsYahtzee(pDice) && GetValueSupplied(pScore, YahtzeeCategory) && GetScore(pScore, YahtzeeCategory) > 0 )
	{
		pScore->m_nYahtzeeBonus += 100;
	}
}

int AnyCategoriesNotSupplied(struct YahtzeeScore* pScore)
{
	int i=0;
	for(; i<YAHTZEE_SCORE_FIELDS; i++)
		if( !GetValueSuppliedField( &pScore->m_Scores[i] ) )
			return 1;
	return 0;
}

int GetTotalNumbers(struct YahtzeeScore* pScore)
{
	return GetScore(pScore, AcesCategory) + GetScore(pScore, TwosCategory) + GetScore(pScore, ThreesCategory) + 
		GetScore(pScore, FoursCategory) + GetScore(pScore, FivesCategory) + GetScore(pScore, SixesCategory);
}

int GetBonus(struct YahtzeeScore* pScore)
{
	return GetTotalNumbers(pScore ) >= 63 ? 35 : 0;
}

int GetTotalUpper(struct YahtzeeScore* pScore)
{
	return GetTotalNumbers(pScore) + GetBonus(pScore);
}

int GetYahtzeeBonus(struct YahtzeeScore* pScore)
{
	return pScore->m_nYahtzeeBonus;
}

int GetTotalLower(struct YahtzeeScore* pScore)
{
	return GetScore(pScore, ThreeOfAKindCategory) + GetScore(pScore, FourOfAKindCategory) + GetScore(pScore, FullHouseCategory) + 
		GetScore(pScore, SmStraightCategory) + GetScore(pScore, LgStraightCategory) + GetScore(pScore, YahtzeeCategory) + GetScore(pScore, ChanceCategory) + GetYahtzeeBonus(pScore);
}

int GetTotal(struct YahtzeeScore* pScore)
{
	return GetTotalUpper(pScore) + GetTotalLower(pScore);
}

int ScoreForCategory(enum ScoreCategory eCategory, struct YahtzeeDice* pDice)
{
	switch(eCategory)
	{
		default:
		case AcesCategory:
			return GetNumberDiceWithValue(pDice, 1);
			break;
		case TwosCategory:
			return 2*GetNumberDiceWithValue(pDice, 2);
			break;
		case ThreesCategory:
			return 3*GetNumberDiceWithValue(pDice, 3);
			break;
		case FoursCategory:
			return 4*GetNumberDiceWithValue(pDice, 4);
			break;
		case FivesCategory:
			return 5*GetNumberDiceWithValue(pDice, 5);
			break;
		case SixesCategory:
			return 6*GetNumberDiceWithValue(pDice, 6);
			break;
		case ThreeOfAKindCategory:
			return LargestOfAKind(pDice) >= 3 ? SumOfAllDice(pDice) : 0;
			break;
		case FourOfAKindCategory:
			return LargestOfAKind(pDice) >= 4 ? SumOfAllDice(pDice) : 0;
			break;
		case FullHouseCategory:
			return IsFullHouse(pDice) ? 25 : 0;
			break;
		case SmStraightCategory:
			return HighestStraight(pDice) >= 4 ? 30 : 0;
			break;
		case LgStraightCategory:
			return HighestStraight(pDice) >= 5 ? 40 : 0;
			break;
		case YahtzeeCategory:
			return IsYahtzee(pDice) ? 50 : 0;
			break;
		case ChanceCategory:
			return SumOfAllDice(pDice);
			break;
	}
	
	return 0;
}

int IsYahtzee(struct YahtzeeDice* pDice)
{
	return LargestOfAKind(pDice) >= 5 ? 1 : 0;
}

