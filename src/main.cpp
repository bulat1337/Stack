#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#include "stack.h"
#include "helpers.h"

int main()
{
	struct Stack_pop_result result =
	{
		.deleted_element = 0,
		.error_code = ALL_GOOD,
	};

	struct Stack stk =
	{
		.capacity = 0,
		.data = NULL,
		.size = 0,
		.stk_name = NULL,
		.file_name = NULL,
		.func_name = NULL,
		.line = 0,
	};
	stack_ctor(&stk, 10, "stk", __FILE__, __LINE__, __func__);

	if(stack_verifier(stk) != 0)
	{
		STACK_DUMP(stk);
	}

	stack_push(&stk, 40);
	result = stack_pop(&stk);

	stack_dtor(&stk);

	return 0;
}
