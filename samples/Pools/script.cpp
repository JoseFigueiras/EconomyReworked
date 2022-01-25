/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "script.h"
#include <string>
#include <vector>
#include <math.h>

//init values from LessMoney.ini
int		g_min_cash_amount = 100 * (int)GetPrivateProfileInt("Main", "MinCashAmount", 0, ".\\LessMoney.ini");
int		g_max_cash_amount = 100 * (int)GetPrivateProfileInt("Main", "MaxCashAmount", 1500, ".\\LessMoney.ini");
float	g_min_tax_percent = (float)GetPrivateProfileInt("Main", "MinTaxPercentage", 0, ".\\LessMoney.ini") / 100;
float	g_max_tax_percent = (float)GetPrivateProfileInt("Main", "MaxTaxPercentage", 85, ".\\LessMoney.ini") / 100;

#define EXCEPTION_1 -500	// placeholder

int g_prev_loop_balance = -1;

void update()
{
	int balance_diff = 0;

	if (g_prev_loop_balance == -1)
		g_prev_loop_balance = MONEY::_MONEY_GET_CASH_BALANCE();
	
	balance_diff = MONEY::_MONEY_GET_CASH_BALANCE() - g_prev_loop_balance;

	if (balance_diff > 0)
	{
		if (balance_diff == EXCEPTION_1)	// for exceptions that might break the game
			;
		else if (balance_diff < g_min_cash_amount)
		{
			double	tax_percent = g_min_tax_percent;
			
			int		decrement_amount = (int)((float)balance_diff * tax_percent);

			MONEY::_MONEY_DECREMENT_CASH_BALANCE(decrement_amount);
		}
		else if (balance_diff < g_max_cash_amount)		// if diff below max threshold
		{
			double	tax_percent =	g_min_tax_percent + 
									(g_max_tax_percent - g_min_tax_percent) * 
									sqrt(
										((double)balance_diff - g_min_cash_amount) /
										(double)(g_min_cash_amount + g_max_cash_amount)
										);
			
			int		decrement_amount = (int)((float)balance_diff * tax_percent);

			MONEY::_MONEY_DECREMENT_CASH_BALANCE(decrement_amount);
		}
		else		// if diff above max threshold
		{
			double	tax_percent = g_max_tax_percent;

			int		decrement_amount = (int)((float)balance_diff * tax_percent);

			MONEY::_MONEY_DECREMENT_CASH_BALANCE(decrement_amount);
		}
	}

	g_prev_loop_balance = MONEY::_MONEY_GET_CASH_BALANCE();
}

void main()
{
	while (true)
	{
		update();
		WAIT(0);
	}
}

void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
