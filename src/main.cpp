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
		.left_canary = 0xBADC0FFEE,
		.capacity = 0,
		.buf_left_canary = NULL,
		.data = NULL,
		.size = 0,
		.stk_name = NULL,
		.file_name = NULL,
		.func_name = NULL,
		.line = 0,
		.error_code = ALL_GOOD,
		.hash_check_value = 0,
		.right_canary = 0xBADC0FFEE,
	};
	stack_ctor(&stk, 10, "stk", __FILE__, __LINE__, __func__);

	if((stk.error_code = (enum Err_ID)stack_verifier(&stk)) != ALL_GOOD)
	{
		ACTUALIZED_STACK_DUMP(&stk);
	}

	stack_push(&stk, 1);


	result = stack_pop(&stk);


	stack_dtor(&stk);

	return 0;
}
