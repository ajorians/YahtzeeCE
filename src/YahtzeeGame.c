#include "YahtzeeGame.h"

#include "YahtzeeDice.h"//For NUMBER_OF_DICE
#include "YahtzeeScore.h"//For ScoreCategory
#include "Yahtzee.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <graphx.h>

#define SELECTOR_CHOICES_CHARWIDTH	5
#define CHAR_HEIGHT	10
#define CHAR_WIDTH	8
#define CHOICE_ROW1_HEIGHT			3*CHAR_HEIGHT
#define CHOICE_ROW2_HEIGHT			4.5*CHAR_HEIGHT
#define CHOICE_ROW3_HEIGHT			6*CHAR_HEIGHT
#define CHOICE_ROW4_HEIGHT			7.5*CHAR_HEIGHT
#define CHOICE_ROW5_HEIGHT			9*CHAR_HEIGHT
#define CHOICE_ROW6_HEIGHT			10.5*CHAR_HEIGHT
#define CHOICE_ROW7_HEIGHT			12*CHAR_HEIGHT
#define CHOICE_ROW8_HEIGHT			13.5*CHAR_HEIGHT
#define CHOICE_ROW9_HEIGHT			15*CHAR_HEIGHT

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240

#define TRACE(x)

struct YahtzeeGame
{
	struct Yahtzee m_Yahtzee;
	enum SelectorSpot m_eLastSelection;//Where the selection was
	enum SelectorSpot m_eSelection;//Where the selection is
} m_Yahtzee;

int NumberOfDigits(int n)//Very quickly thrown together
{
	int nDigits = 0;
	if( n == 0 )
		return 1;
	while(n/10 > 0 || n%10 > 0)
	{
		nDigits++;
		n/=10;
	}
	return nDigits;
}

void itoa(int n, char buffer[], int nBufferSize)//Very quickly thrown together
{
	int i=2;
	nBufferSize = (nBufferSize < (NumberOfDigits(n)+1)) ? nBufferSize : (NumberOfDigits(n)+1);
	for(; i<=nBufferSize; i++)
	{
		int nRemainder = n%10;
		buffer[nBufferSize-i] = '0' + nRemainder;
		n/=10;
	}
	buffer[nBufferSize-1] = '\0';
}

void InitYahtzeeGame()
{
	m_Yahtzee.m_eLastSelection = RollSpot;
	m_Yahtzee.m_eSelection = RollSpot;
	YahtzeeReset(&m_Yahtzee.m_Yahtzee);
	
	/* Fill the screen white */
	gfx_FillScreen(gfx_white);
	DisplayBoard();
}

int Loop()
{
	while(1)
	{
		if( 0 == HandleGameKeyPresses() )
		{
			return 0;
		}
	}
	return 1;
}

int HandleGameKeyPresses()
{
	sk_key_t key;
	key = os_GetCSC();

	if( key == sk_Clear || key == sk_Power)
	{
		return 0;
	}
	else if( key == sk_Left || key == sk_4 )
	{
		TRACE("Left key pressed");
		
		m_Yahtzee.m_eLastSelection = m_Yahtzee.m_eSelection;
		
		if( m_Yahtzee.m_eSelection == Dice1Spot )//Convenient
		{
			ClearDice( m_Yahtzee.m_eSelection - Dice1Spot );
			m_Yahtzee.m_eSelection = Dice5Spot;
		}
		else if( m_Yahtzee.m_eSelection >= Dice1Spot && m_Yahtzee.m_eSelection <= Dice5Spot )//A Dice
		{
			ClearDice( m_Yahtzee.m_eSelection - Dice1Spot );
			--m_Yahtzee.m_eSelection;
		}
		else if( m_Yahtzee.m_eSelection == ThreeOfAKindSpot )
			m_Yahtzee.m_eSelection = AcesSpot;
		else if( m_Yahtzee.m_eSelection == FourOfAKindSpot )
			m_Yahtzee.m_eSelection = TwosSpot;
		else if( m_Yahtzee.m_eSelection == FullHouseSpot )
			m_Yahtzee.m_eSelection = ThreesSpot;
		else if( m_Yahtzee.m_eSelection == SmStraightSpot )
			m_Yahtzee.m_eSelection = FoursSpot;
		else if( m_Yahtzee.m_eSelection == LgStraightSpot )
			m_Yahtzee.m_eSelection = FivesSpot;
		else if( m_Yahtzee.m_eSelection == YahtzeeSpot )
			m_Yahtzee.m_eSelection = SixesSpot;
		else if( m_Yahtzee.m_eSelection == RollSpot && IsGameOver( &(m_Yahtzee.m_Yahtzee) ) )//Quit spot when gameover
			m_Yahtzee.m_eSelection = PlayAgainSpot;
		
		DisplayBoard();
	}
	else if( key == sk_Right || key == sk_6 )
	{
		TRACE("Right key pressed");
		
		m_Yahtzee.m_eLastSelection = m_Yahtzee.m_eSelection;
		
		if( m_Yahtzee.m_eSelection == Dice5Spot )//Convenient
		{
			ClearDice( m_Yahtzee.m_eSelection - Dice1Spot );
			m_Yahtzee.m_eSelection = Dice1Spot;
		}
		else if( m_Yahtzee.m_eSelection <= Dice4Spot && m_Yahtzee.m_eSelection >= ChanceSpot )
		{
			ClearDice( m_Yahtzee.m_eSelection - Dice1Spot );
			++m_Yahtzee.m_eSelection;
		}
		else if( m_Yahtzee.m_eSelection == AcesSpot )
			m_Yahtzee.m_eSelection = ThreeOfAKindSpot;
		else if( m_Yahtzee.m_eSelection == TwosSpot )
			m_Yahtzee.m_eSelection = FourOfAKindSpot;
		else if( m_Yahtzee.m_eSelection == ThreesSpot )
			m_Yahtzee.m_eSelection = FullHouseSpot;
		else if( m_Yahtzee.m_eSelection == FoursSpot )
			m_Yahtzee.m_eSelection = SmStraightSpot;
		else if( m_Yahtzee.m_eSelection == FivesSpot )
			m_Yahtzee.m_eSelection = LgStraightSpot;
		else if( m_Yahtzee.m_eSelection == SixesSpot )
			m_Yahtzee.m_eSelection = YahtzeeSpot;
		else if( m_Yahtzee.m_eSelection == PlayAgainSpot && IsGameOver(&m_Yahtzee.m_Yahtzee) )
			m_Yahtzee.m_eSelection = RollSpot;//Quit spot when gameover
		
		DisplayBoard();
	}
	else if( key == sk_Up || key == sk_8 )
	{
		TRACE("Up key pressed");
		
		m_Yahtzee.m_eLastSelection = m_Yahtzee.m_eSelection;
		
		if( m_Yahtzee.m_eSelection == RollSpot )
		{
			if( !IsGameOver(&m_Yahtzee.m_Yahtzee) )
				m_Yahtzee.m_eSelection = Dice1Spot;
		}
		else if( m_Yahtzee.m_eSelection <= ChanceSpot && m_Yahtzee.m_eSelection >= AcesSpot )//Choice spot
			--m_Yahtzee.m_eSelection;
		else if( m_Yahtzee.m_eSelection >= Dice1Spot && m_Yahtzee.m_eSelection <= Dice5Spot )//A dice
		{
			ClearDice( m_Yahtzee.m_eSelection - Dice1Spot );
			m_Yahtzee.m_eSelection = ChanceSpot;
		}
		
		DisplayBoard();
	}
	else if( key == sk_Down || key == sk_2 )
	{
		TRACE("Down key pressed");
		
		m_Yahtzee.m_eLastSelection = m_Yahtzee.m_eSelection;
		
		if( m_Yahtzee.m_eSelection == SixesSpot )//If you want it to go to the 3 of a kind; comment this part out
		{
			m_Yahtzee.m_eSelection = Dice1Spot;
		}
		else if( m_Yahtzee.m_eSelection <= ChanceSpot && m_Yahtzee.m_eSelection >= RollSpot )//Roll or choice
		{
			if( !IsGameOver(&m_Yahtzee.m_Yahtzee) )
				++m_Yahtzee.m_eSelection;
		}
		else if( m_Yahtzee.m_eSelection >= Dice1Spot && m_Yahtzee.m_eSelection <= Dice5Spot )//A dice
		{
			ClearDice( m_Yahtzee.m_eSelection - Dice1Spot );
			m_Yahtzee.m_eSelection = RollSpot;
		}
		
		DisplayBoard();
	}
	else if( key == sk_Enter || key == sk_2nd )
	{
		TRACE("Enter key pressed");
		if( m_Yahtzee.m_eSelection == RollSpot )//And quit spot
		{
			if( !IsGameOver(&m_Yahtzee.m_Yahtzee) )
			{
				TRACE("Roll");
				YahtzeeRoll(&m_Yahtzee.m_Yahtzee);
				gfx_FillScreen(gfx_white);
				DisplayBoard();
			}
			else
			{
				return 0;
			}
		}
		else if( m_Yahtzee.m_eSelection >= Dice1Spot && m_Yahtzee.m_eSelection <= Dice5Spot )
		{
			if( GetRollNumber(&m_Yahtzee.m_Yahtzee) > 0 )
			{
				TRACE("Hold Dice");
				YahtzeeHoldDice(&m_Yahtzee.m_Yahtzee, m_Yahtzee.m_eSelection - Dice1Spot, !YahtzeeGetDiceHolded(&m_Yahtzee.m_Yahtzee, m_Yahtzee.m_eSelection - Dice1Spot));
				ClearDice( m_Yahtzee.m_eSelection - Dice1Spot );
				DisplayBoard();
			}
		}
		else if( m_Yahtzee.m_eSelection >= AcesSpot && m_Yahtzee.m_eSelection <= ChanceSpot )
		{
			enum ScoreCategory eCategory = (enum ScoreCategory)(m_Yahtzee.m_eSelection-1);
			if( !GetScoreEntered(&m_Yahtzee.m_Yahtzee, eCategory) && GetRollNumber(&m_Yahtzee.m_Yahtzee) > 0 )
			{
				TRACE("Scored spot");
				ScoreSpot(&m_Yahtzee.m_Yahtzee, eCategory);//The -1 is to convert SelectionSpot to ScoreCategory
				m_Yahtzee.m_eLastSelection = m_Yahtzee.m_eSelection;
				m_Yahtzee.m_eSelection = RollSpot;
				
				if( IsGameOver(&m_Yahtzee.m_Yahtzee) )
				{
					FillBox(0, 1.5 * CHAR_HEIGHT-1, SCREEN_WIDTH, CHAR_HEIGHT+3, gfx_white);//Clear Roll /Play again&Quit area
				}
				
				gfx_FillScreen(gfx_white);
				DisplayBoard();
			}
		}
		else if( m_Yahtzee.m_eSelection == PlayAgainSpot && IsGameOver(&m_Yahtzee.m_Yahtzee) )
		{
			TRACE("Playing again");
			
			YahtzeeReset(&m_Yahtzee.m_Yahtzee);
			m_Yahtzee.m_eLastSelection = m_Yahtzee.m_eSelection = RollSpot;
			
			//FillBox(0, 1.5 * CHAR_HEIGHT-1, SCREEN_WIDTH, CHAR_HEIGHT+3, gfx_white);//Clear Roll /Play again&Quit area
			gfx_FillScreen(gfx_white);
			
			DisplayBoard();
		}
	}
	return 1;
}

void DisplayBoard()
{
	//gfx_FillScreen(gfx_white);

	/* Set the text color */
	gfx_SetTextFGColor(gfx_black);
	//Status
	FillBox(0, 0, SCREEN_WIDTH, CHAR_HEIGHT, gfx_white);//Clear status
	{
		char buffer[2] = {0};
		gfx_PrintStringXY("Roll: ", 0, 0  * CHAR_HEIGHT);
		itoa(GetRollNumber(&m_Yahtzee.m_Yahtzee), buffer, 2);
		gfx_PrintStringXY(buffer, 7 * CHAR_WIDTH, 0  * CHAR_HEIGHT);
		gfx_PrintStringXY("/", 8 * CHAR_WIDTH, 0  * CHAR_HEIGHT);
		itoa(ROLL_LIMIT, buffer, 2);
		gfx_PrintStringXY("3", 9 * CHAR_WIDTH, 0  * CHAR_HEIGHT);
		gfx_PrintStringXY("Score:", SCREEN_WIDTH/2, 0  * CHAR_HEIGHT );//1, 1, BLUE, m_screen);
	}
	{
		char buf[5] = {0};
		itoa(YahtzeeGetTotal(&m_Yahtzee.m_Yahtzee), buf,5);
		gfx_PrintStringXY( buf, SCREEN_WIDTH/2 + 8 * CHAR_WIDTH, 0  * CHAR_HEIGHT);
	}
	
	//Roll Choice
	if( !IsGameOver(&m_Yahtzee.m_Yahtzee) )
	{
		gfx_PrintStringXY("--ROLL--", SCREEN_WIDTH/3, 1.5 * CHAR_HEIGHT);
	}
	else
	{
		gfx_PrintStringXY("--PLAY AGIN--", SCREEN_WIDTH/5, 1.5 * CHAR_HEIGHT);
		gfx_PrintStringXY("--QUIT--", 3*SCREEN_WIDTH/5, 1.5 * CHAR_HEIGHT);
	}
	
	//Draw Choices
	gfx_PrintStringXY( "Aces        :", 0, CHOICE_ROW1_HEIGHT);
	gfx_PrintStringXY( "2s          :", 0, CHOICE_ROW2_HEIGHT);
	gfx_PrintStringXY( "3s          :", 0, CHOICE_ROW3_HEIGHT);
	gfx_PrintStringXY( "4s          :", 0, CHOICE_ROW4_HEIGHT);
	gfx_PrintStringXY( "5s          :", 0, CHOICE_ROW5_HEIGHT);
	gfx_PrintStringXY( "6s          :", 0, CHOICE_ROW6_HEIGHT);
	gfx_PrintStringXY( "Total       :", 0, CHOICE_ROW7_HEIGHT);
	gfx_PrintStringXY( "Bonus (>=63):", 0, CHOICE_ROW8_HEIGHT);
	gfx_PrintStringXY( "Total Upper :", 0, CHOICE_ROW9_HEIGHT);

	gfx_PrintStringXY("3 of a kind:", SCREEN_WIDTH/2, CHOICE_ROW1_HEIGHT);
	gfx_PrintStringXY("4 of a kind:", SCREEN_WIDTH/2, CHOICE_ROW2_HEIGHT);
	gfx_PrintStringXY("Full house :", SCREEN_WIDTH/2, CHOICE_ROW3_HEIGHT);
	gfx_PrintStringXY("Sm Straight:", SCREEN_WIDTH/2, CHOICE_ROW4_HEIGHT);
	gfx_PrintStringXY("Lg Straight:", SCREEN_WIDTH/2, CHOICE_ROW5_HEIGHT);
	gfx_PrintStringXY("Yahtzee    :", SCREEN_WIDTH/2, CHOICE_ROW6_HEIGHT);
	gfx_PrintStringXY("Chance     :", SCREEN_WIDTH/2, CHOICE_ROW7_HEIGHT);
	gfx_PrintStringXY("Yahtzee bonus:", SCREEN_WIDTH/2, CHOICE_ROW8_HEIGHT);
	gfx_PrintStringXY("Total Lower:", SCREEN_WIDTH/2, CHOICE_ROW9_HEIGHT);
	
	//Draw Score Values
	//drawBox_(CHAR_WIDTH*14, CHOICE_ROW1_HEIGHT, CHAR_WIDTH*5, CHOICE_ROW9_HEIGHT+CHAR_HEIGHT-CHOICE_ROW1_HEIGHT, WHITE, m_screen);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW1_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, AcesCategory), GetScoreEntered(&m_Yahtzee.m_Yahtzee, AcesCategory));
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW2_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, TwosCategory), GetScoreEntered(&m_Yahtzee.m_Yahtzee, TwosCategory));
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW3_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, ThreesCategory), GetScoreEntered(&m_Yahtzee.m_Yahtzee, ThreesCategory));
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW4_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, FoursCategory), GetScoreEntered(&m_Yahtzee.m_Yahtzee, FoursCategory));
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW5_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, FivesCategory), GetScoreEntered(&m_Yahtzee.m_Yahtzee, FivesCategory));
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW6_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, SixesCategory), GetScoreEntered(&m_Yahtzee.m_Yahtzee, SixesCategory));
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW7_HEIGHT, 	YahtzeeGetTotalNumbers(&m_Yahtzee.m_Yahtzee), 1);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW8_HEIGHT, 	YahtzeeGetTotalBonus(&m_Yahtzee.m_Yahtzee), 1);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW9_HEIGHT, 	YahtzeeGetTotalUpper(&m_Yahtzee.m_Yahtzee), 1);

	//drawBox_(CHAR_WIDTH*34, CHOICE_ROW1_HEIGHT, CHAR_WIDTH*5, CHOICE_ROW9_HEIGHT+CHAR_HEIGHT-CHOICE_ROW1_HEIGHT, WHITE, m_screen);
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW1_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, ThreeOfAKindCategory), GetScoreEntered(&m_Yahtzee.m_Yahtzee, ThreeOfAKindCategory));
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW2_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, FourOfAKindCategory),  GetScoreEntered(&m_Yahtzee.m_Yahtzee, FourOfAKindCategory));
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW3_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, FullHouseCategory),    GetScoreEntered(&m_Yahtzee.m_Yahtzee, FullHouseCategory));
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW4_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, SmStraightCategory),   GetScoreEntered(&m_Yahtzee.m_Yahtzee, SmStraightCategory));
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW5_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, LgStraightCategory),   GetScoreEntered(&m_Yahtzee.m_Yahtzee, LgStraightCategory));
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW6_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, YahtzeeCategory),      GetScoreEntered(&m_Yahtzee.m_Yahtzee, YahtzeeCategory));
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW7_HEIGHT, 	YahtzeeGetScore(&m_Yahtzee.m_Yahtzee, ChanceCategory),       GetScoreEntered(&m_Yahtzee.m_Yahtzee, ChanceCategory));
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW8_HEIGHT, 	YahtzeeGetYahtzeeBonus(&m_Yahtzee.m_Yahtzee), 1);
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW9_HEIGHT, 	YahtzeeGetTotalLower(&m_Yahtzee.m_Yahtzee), 1);

	//Dice
	{
		int nDiceSideLength = SCREEN_WIDTH/(2*NUMBER_OF_DICE+1);//The +1 is because I want a gap and then the first dice
		int nDisplaceAmount = nDiceSideLength/2;
		int i=0;
		for(; i<NUMBER_OF_DICE; i++)
		{
			int nX = nDiceSideLength*(2*i+1), nY = SCREEN_HEIGHT - nDiceSideLength-1 - nDisplaceAmount;//The -1 is be off the bottom
			DrawBox(nX-1, nY-1, nDiceSideLength+3, 1.5*nDiceSideLength+2, gfx_white);

			DrawDice(nX, nY + (YahtzeeGetDiceHolded( &m_Yahtzee.m_Yahtzee, i ) ? nDisplaceAmount : 0), 
				nDiceSideLength, YahtzeeGetDiceValue( &m_Yahtzee.m_Yahtzee, i));
		}
	}

	if( !(m_Yahtzee.m_eLastSelection >= Dice1Spot && m_Yahtzee.m_eLastSelection <= Dice5Spot) )
		DrawSelector(m_Yahtzee.m_eLastSelection, 0);
	DrawSelector(m_Yahtzee.m_eSelection, 1);
}

void DrawSelector(enum SelectorSpot eSpot, int bON)
{
	int c = bON ? gfx_red : gfx_white;
	int nDiceSideLength = SCREEN_WIDTH/(2*NUMBER_OF_DICE+1);//The +1 is because I want a gap and then the first dice
	int nDisplaceAmount = nDiceSideLength/2;
	switch(eSpot)
	{
		case PlayAgainSpot:
			DrawBox(SCREEN_WIDTH/5-1, 1.5 * CHAR_HEIGHT-1, CHAR_WIDTH * 14+2, CHAR_HEIGHT+2, c);
			break;
		default:
		case RollSpot:
			if( IsGameOver(&m_Yahtzee.m_Yahtzee) )
			{
				DrawBox(3*SCREEN_WIDTH/5-1, 1.5 * CHAR_HEIGHT-1, CHAR_WIDTH * 8+2, CHAR_HEIGHT+2, c);
			}
			else
			{
				DrawBox(SCREEN_WIDTH/3-1, 1.5 * CHAR_HEIGHT-1, CHAR_WIDTH * 8+2, CHAR_HEIGHT+2, c);
			}
			break;
		case AcesSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW1_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case TwosSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW2_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case ThreesSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW3_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case FoursSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW4_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case FivesSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW5_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case SixesSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW6_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case ThreeOfAKindSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW1_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case FourOfAKindSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW2_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case FullHouseSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW3_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case SmStraightSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW4_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case LgStraightSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW5_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case YahtzeeSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW6_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case ChanceSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW7_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c);
			break;
		case Dice1Spot:
			DrawBox(nDiceSideLength*(1)-1, SCREEN_HEIGHT - nDiceSideLength-1 - (YahtzeeGetDiceHolded(&m_Yahtzee.m_Yahtzee, 0) ? 0 : nDisplaceAmount) -1, //The extra -1 is to surround the box
			nDiceSideLength+2, nDiceSideLength+2, c);
			break;
		case Dice2Spot:
			DrawBox(nDiceSideLength*(2+1)-1, SCREEN_HEIGHT - nDiceSideLength-1 - (YahtzeeGetDiceHolded(&m_Yahtzee.m_Yahtzee, 1) ? 0 : nDisplaceAmount) -1, //The extra -1 is to surround the box
			nDiceSideLength+2, nDiceSideLength+2, c);
			break;
		case Dice3Spot:
			DrawBox(nDiceSideLength*(4+1)-1, SCREEN_HEIGHT - nDiceSideLength-1 - (YahtzeeGetDiceHolded(&m_Yahtzee.m_Yahtzee, 2) ? 0 : nDisplaceAmount) -1, //The extra -1 is to surround the box
			nDiceSideLength+2, nDiceSideLength+2, c);
			break;
		case Dice4Spot:
			DrawBox(nDiceSideLength*(6+1)-1, SCREEN_HEIGHT - nDiceSideLength-1 - (YahtzeeGetDiceHolded(&m_Yahtzee.m_Yahtzee, 3) ? 0 : nDisplaceAmount) -1, //The extra -1 is to surround the box
			nDiceSideLength+2, nDiceSideLength+2, c);
			break;
		case Dice5Spot:
			DrawBox(nDiceSideLength*(8+1)-1, SCREEN_HEIGHT - nDiceSideLength-1 - (YahtzeeGetDiceHolded(&m_Yahtzee.m_Yahtzee, 4) ? 0 : nDisplaceAmount) -1, //The extra -1 is to surround the box
			nDiceSideLength+2, nDiceSideLength+2, c);
			break;
	}
}

void DrawScoreValue(int nX, int nY, int nValue, int bUsed)
{
	int c = bUsed ? gfx_black : gfx_green;
	char buf[4] = {0};
	if( nValue < 0 || nValue > 1000 )
		return;
	
	if( !bUsed && GetRollNumber(&m_Yahtzee.m_Yahtzee )<= 0 )//If haven't rolled don't display spots haven't choosen
		return;
	
	FillBox(nX, nY, CHAR_WIDTH*3, CHAR_HEIGHT, gfx_white );
	
	gfx_SetTextFGColor(c);
	itoa(nValue, buf, 4);
	gfx_PrintStringXY(buf, nX, nY);
}

void DrawDice(int nX, int nY, int nDiceSideLength, int nNumber)
{
	//Draw Box
	DrawBox(nX, nY, nDiceSideLength, nDiceSideLength, gfx_black);
	DrawBox(nX+1, nY+1, nDiceSideLength-2, nDiceSideLength-2, gfx_white);

	gfx_SetColor(gfx_black);
	
	//Draw Numbers
	if( nNumber >=1 && nNumber <= 6 )
	{
		int nCircleRadius = 2;
		switch(nNumber)
		{
			default:
			case 1:
				gfx_FillCircle_NoClip(nX + nDiceSideLength/2, nY + nDiceSideLength/2, nCircleRadius);
				break;
			case 2:
				gfx_FillCircle_NoClip(nX + 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius);
				gfx_FillCircle_NoClip(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius);
				break;
			case 3:
				gfx_FillCircle_NoClip(nX + nDiceSideLength/2, nY + nDiceSideLength/2, nCircleRadius);
				gfx_FillCircle_NoClip(nX + 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius);
				gfx_FillCircle_NoClip(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius);
				break;
			case 4:
				gfx_FillCircle_NoClip(nX + 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius);//Top-Left
				gfx_FillCircle_NoClip(nX + nDiceSideLength - 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius);//Top-Right
				gfx_FillCircle_NoClip(nX + 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius);//Bottom-Left
				gfx_FillCircle_NoClip(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius);//Bottom-Right
				break;
			case 5:
				gfx_FillCircle_NoClip(nX + nDiceSideLength/2, nY + nDiceSideLength/2, nCircleRadius);//Center
				gfx_FillCircle_NoClip(nX + 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius);//Top-Left
				gfx_FillCircle_NoClip(nX + nDiceSideLength - 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius);//Top-Right
				gfx_FillCircle_NoClip(nX + 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius);//Bottom-Left
				gfx_FillCircle_NoClip(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius);//Bottom-Right
				break;
			case 6:
				gfx_FillCircle_NoClip(nX + 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius);//Top-Left
				gfx_FillCircle_NoClip(nX + nDiceSideLength - 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius);//Top-Right
				gfx_FillCircle_NoClip(nX + 2*nCircleRadius, nY + nDiceSideLength/2, nCircleRadius);//Middle-Left
				gfx_FillCircle_NoClip(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength/2, nCircleRadius);//Middle-Right
				gfx_FillCircle_NoClip(nX + 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius);//Bottom-Left
				gfx_FillCircle_NoClip(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius);//Bottom-Right
				break;
		}
	}
}

void ClearDice(int nDice)
{
	int nDiceSideLength = SCREEN_WIDTH/(2*NUMBER_OF_DICE+1);//The +1 is because I want a gap and then the first dice
	int nDisplaceAmount = nDiceSideLength/2;
	int nX = nDiceSideLength*(2*nDice+1), nY = SCREEN_HEIGHT - nDiceSideLength-1 - nDisplaceAmount;//The -1 is be off the bottom
	
	FillBox( nX-1, nY, nDiceSideLength+2, SCREEN_HEIGHT-nY, gfx_white);
}

void DrawBox(int nX, int nY, int nWidth, int nHeight, int c)
{
	gfx_SetColor(c);
	gfx_Rectangle_NoClip( nX, nY, nWidth, nHeight);
}

void FillBox(int nX, int nY, int nWidth, int nHeight, int c)
{
	gfx_SetColor(c);
	gfx_FillRectangle( nX, nY, nWidth, nHeight );
}

