#include "YahtzeeDice.h"

#include <tice.h>

void ResetDice( struct Dice* pDice )
{
	pDice->m_bHold = 0;
	pDice->m_nValue = -1;
}

void RollDice(struct Dice* pDice)
{
	pDice->m_nValue = randInt(1, 6 );
}

void HoldDice(struct Dice* pDice, int bHold)
{
	pDice->m_bHold = bHold;
}

int GetDiceHolded(struct Dice* pDice)
{
	return pDice->m_bHold;
}

int GetDiceValue(struct Dice* pDice)
{
	return pDice->m_nValue;
}

///
void ResetYahtzeeDice(struct YahtzeeDice* pDice)
{
	int i=0;
	for(; i<NUMBER_OF_DICE; i++)
		ResetDice(&pDice->m_Dice[i]);
}

void RollYahtzeeDice(struct YahtzeeDice* pDice)
{
	int i=0;
	for(; i<NUMBER_OF_DICE; i++)
		if( !GetYahtzeeDiceHolded(pDice, i) )
			RollDice(&pDice->m_Dice[i]);
}

void HoldYahtzeeDice(struct YahtzeeDice* pDice, int nDiceIndex, int bHold)
{
	if( nDiceIndex <0 || nDiceIndex >= NUMBER_OF_DICE )
		return;
	
	HoldDice(&pDice->m_Dice[nDiceIndex], bHold);
}

int GetYahtzeeDiceHolded(struct YahtzeeDice* pDice, int nDiceIndex)
{
	if( nDiceIndex <0 || nDiceIndex >= NUMBER_OF_DICE )
		return 0;
	
	return GetDiceHolded(&pDice->m_Dice[nDiceIndex]);
}

int GetYahtzeeDiceValue(struct YahtzeeDice* pDice, int nDiceIndex)
{
	if( nDiceIndex < 0 || nDiceIndex >= NUMBER_OF_DICE )
		return 0;
	
	return GetDiceValue(&pDice->m_Dice[nDiceIndex]);
}

int GetNumberDiceWithValue(struct YahtzeeDice* pDice, int nValue)
{
	int nCount = 0;
	int i=0;
	for (; i<NUMBER_OF_DICE; i++)
	{
		if (GetDiceValue(&pDice->m_Dice[i]) == nValue)
			nCount++;
	}
	return nCount;
}

int LargestOfAKind(struct YahtzeeDice* pDice)
{
	int nLargest = 0;
	int i = 0;
	for (; i<NUMBER_OF_DICE; i++)
	{
		int nCount = 1;
		int j = 0;
		for (; j<NUMBER_OF_DICE; j++)
		{
			if (i == j)
				continue;

			if (GetDiceValue(&pDice->m_Dice[i]) == GetDiceValue(&pDice->m_Dice[j]))
				nCount++;
		}
		if (nCount > nLargest)
			nLargest = nCount;
	}
	return nLargest;
}

int IsFullHouse(struct YahtzeeDice* pDice)
{
	int i = 2;
	int nNumber1, nNumber2;
	//Basically if it consists of 2 numbers and the Largest of a Kind is 3.
	nNumber1 = GetDiceValue(&pDice->m_Dice[0]);
	nNumber2 = GetDiceValue(&pDice->m_Dice[1]);
	if (nNumber1 == nNumber2)//It could be possible dice 0 & 1 are the same number
		nNumber2 = GetDiceValue(&pDice->m_Dice[2]);
	if (nNumber1 == nNumber2)//This fixes that!
		nNumber2 = GetDiceValue(&pDice->m_Dice[3]);
	for (; i<NUMBER_OF_DICE; i++)
	{
		if (GetDiceValue(&pDice->m_Dice[i]) != nNumber1 && GetDiceValue(&pDice->m_Dice[i]) != nNumber2)
			return 0;
	}

	if( LargestOfAKind(pDice) == 3 )
		return 1;//Exactly 3; the dice consist of 2 numbers and one of the numbers shows up 3 times.
	return 0;
}

int HighestStraight(struct YahtzeeDice* pDice)
{
	int nHighestStraight = 0;
	int nStartingDice = 1;
	for (; nStartingDice <= 6; nStartingDice++)//Could be improved if speed is an issue
	{
		int nNumber = nStartingDice, nCount = 0;
		int i = 0;
		for (; i<NUMBER_OF_DICE; i++)
		{
			int j = 0;
			for (; j<NUMBER_OF_DICE; j++)
			{
				if (GetDiceValue(&pDice->m_Dice[j]) == nNumber)
				{
					nNumber++;
					nCount++;
					break;
				}
			}
		}
		if (nCount > nHighestStraight)
			nHighestStraight = nCount;
	}
	return nHighestStraight;
}

int SumOfAllDice(struct YahtzeeDice* pDice)
{
	int nSum = 0;
	int i = 0;
	for (; i<NUMBER_OF_DICE; i++)
		nSum += GetDiceValue(&pDice->m_Dice[i]);
	return nSum;
}

