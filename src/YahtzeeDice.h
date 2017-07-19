#ifndef YAHTZEEDICE_H
#define YAHTZEEDICE_H

#define NUMBER_OF_DICE	5

struct Dice
{
	int m_nValue;
	int m_bHold;
};

void ResetDice( struct Dice* pDice );
void RollDice(struct Dice* pDice);
void HoldDice(struct Dice* pDice, int bHold);
int GetDiceHolded(struct Dice* pDice);
int GetDiceValue(struct Dice* pDice);

struct YahtzeeDice
{
	struct Dice m_Dice[NUMBER_OF_DICE];
};

void ResetYahtzeeDice(struct YahtzeeDice* pDice);
void RollYahtzeeDice(struct YahtzeeDice* pDice);
void HoldYahtzeeDice(struct YahtzeeDice* pDice, int nDiceIndex, int bHold);
int GetYahtzeeDiceHolded(struct YahtzeeDice* pDice, int nDiceIndex);
int GetYahtzeeDiceValue(struct YahtzeeDice* pDice, int nDiceIndex);
int GetNumberDiceWithValue(struct YahtzeeDice* pDice, int nValue);
int LargestOfAKind(struct YahtzeeDice* pDice);
int IsFullHouse(struct YahtzeeDice* pDice);
int HighestStraight(struct YahtzeeDice* pDice);
int SumOfAllDice(struct YahtzeeDice* pDice);

#endif
