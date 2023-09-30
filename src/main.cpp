#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#include "stack.h"
#include "helpers.h"
// unit_test1, 2, ...
//{
//	struct stack stk
//	struct stack exp_stk
//	expected_stack;
//
//	ctor(0)
//	push

//	stk != expected_stk
//}



int main()
{
	struct Stack_pop_result result =
	{
		.deleted_element = 0,
		.error_code = ALL_GOOD,
	};

	enum Err_ID error_code = ALL_GOOD;

	struct Stack stk = {};
	STACK_CTOR(&stk, 10);

	if((error_code = (enum Err_ID)stack_verifier(&stk)) != ALL_GOOD) //явный каст не нужен
	//copypaste
	{
		ACTUALIZED_STACK_DUMP(&stk, error_code);
	}

	STACK_PUSH(&stk, 17);


	result = STACK_POP(&stk);


	stack_dtor(&stk);

	return 0;
}

/*
0x1
0x12
0x100

bool is_registered[---]

is_registered[0x100] = true;

[0; 1000]

bool is_registred[]
*/
