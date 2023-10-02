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

	struct Stack stk = {};
	enum Err_ID error_code = ALL_GOOD;

	//STACK_CTOR(&stk, 0);
	//STACK_PUSH(&stk, 666);
	//result = STACK_POP(&stk);
	//stack_dtor(&stk);

	printf("%d", unit_test_1(&stk));

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
