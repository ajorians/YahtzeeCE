#ifndef YAHTZEESCORE_H
#define YAHTZEESCORE_H

#define YAHTZEE_SCORE_FIELDS	13

struct YahtzeeDice;

enum ScoreCategory
{
	AcesCategory = 0,
	TwosCategory,
	ThreesCategory,
	FoursCategory,
	FivesCategory,
	SixesCategory,
	ThreeOfAKindCategory,
	FourOfAKindCategory,
	FullHouseCategory,
	SmStraightCategory,
	LgStraightCategory,
	YahtzeeCategory,
	ChanceCategory
};

struct ScoreField
{
	int m_bValueSupplied;//Whether a value was supplied
	int m_nValue;//The value
};

void ResetScoreField( struct ScoreField* pScore );
void SetScoreField(struct ScoreField* pScore, int nScore);
int GetScoreField(struct ScoreField* pScore);
int GetValueSuppliedField(struct ScoreField* pScore);

struct YahtzeeScore
{
	struct ScoreField m_Scores[YAHTZEE_SCORE_FIELDS];
	int m_nYahtzeeBonus;
};

void ResetScore(struct YahtzeeScore* pScore);
void SetScore(struct YahtzeeScore* pScore, enum ScoreCategory eCategory, int nScore);
int GetScore(struct YahtzeeScore* pScore, enum ScoreCategory eCategory);
int GetValueSupplied(struct YahtzeeScore* pScore, enum ScoreCategory eCategory);
void HandleIfYahtzee(struct YahtzeeScore* pScore, struct YahtzeeDice* pDice);

int AnyCategoriesNotSupplied(struct YahtzeeScore* pScore);

int GetTotalNumbers(struct YahtzeeScore* pScore);
int GetBonus(struct YahtzeeScore* pScore);
int GetTotalUpper(struct YahtzeeScore* pScore);
int GetYahtzeeBonus(struct YahtzeeScore* pScore);
int GetTotalLower(struct YahtzeeScore* pScore);
int GetTotal(struct YahtzeeScore* pScore);

int ScoreForCategory(enum ScoreCategory eCategory, struct YahtzeeDice* pDice);
int IsYahtzee(struct YahtzeeDice* pDice);

#endif
