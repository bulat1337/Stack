#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#include "stack.h"
#include "helpers.h"

void stack_ctor(Stack *stk, size_t default_capacity, const char *stk_name, const char *file_name, size_t line, const char *func_name)
{
	stk->left_canary = 0xBADC0FFEE;

	stk->data = (elem_t *)calloc(10, sizeof(elem_t));
	if(stk->data == NULL)
	{
		printf("Not enoough memory for allocation");
		exit(EXIT_FAILURE);
	}
	stk->size = 0;
	stk->capacity = default_capacity;
	stk->stk_name = stk_name;
	stk->file_name = file_name;
	stk->func_name = func_name;
	stk->line = line;

	stk->right_canary = 0xBADC0FFEE;
}

void stack_push(Stack *stk, elem_t value)
{
	printf("\nSTACK_PUSH LOG:\n");
	if((stk->error_code = stack_verifier(*stk)) != 0)
	{
		stk->file_name = __FILE__;
		stk->func_name = __func__;
		stk->line = __LINE__;
		STACK_DUMP(*stk);

		exit(EXIT_FAILURE);
	}

	if(stk->size >= stk->capacity)
	{
		stk->capacity *= 2;
		stk->data = (elem_t *)realloc(stk->data, stk->capacity * sizeof(elem_t)); //copy paste1
		printf("new capacity: expanded\n");
	}
	else
	{
		stk->data[stk->size] = value;
		stk->size++;
	}

	for(size_t i = 0; i < stk->capacity; i++)
	{
		printf("[%lu]%d ", i, (stk->data)[i]);
	}
	printf("\nsize = %lu, capacity = %lu", stk->size, stk->capacity);
	printf("\n");
}

struct Stack_pop_result stack_pop(Stack *stk)
{
	printf("\nSTACK_POP LOG:\n");
	if((stk->error_code = stack_verifier(*stk)) != 0)
	{
		stk->file_name = __FILE__;
		stk->func_name = __func__;
		stk->line = __LINE__;
		STACK_DUMP(*stk);

		exit(EXIT_FAILURE);
	}
	struct Stack_pop_result result =
	{
		.deleted_element = 0,
		.error_code = ALL_GOOD,
	};

	if(stk->size == 0)
	{
		result.error_code = SIZE_ZERO;
		fprintf(stderr, "stack_pop failed: size == 0\n");
		exit(EXIT_FAILURE);
	}

	(stk->size)--;
	result.deleted_element = (stk->data)[stk->size];
	if(stk->size <= stk->capacity / (2 * 2)) //2 -> const
	{
		printf("Here is why capacity is going to be shrinked:\n");
		printf("\t size[%lu] <= capacity[%lu]\n", stk->size, stk->capacity / (2 * 2));
		stk->capacity /= 2;
		stk->data = (elem_t *)realloc(stk->data, stk->capacity * sizeof(elem_t)); //copy paste1
		printf("new capacity: shrinked\n");
	}

	for(size_t i = 0; i < stk->capacity; i++)
	{
		printf("%d", (stk->data)[i]);
	}
	printf("\t size = %lu, capacity = %lu", stk->size, stk->capacity);
	printf("\n");

	return result;
}

void stack_dtor(Stack *stk)
{
	printf("\nSTACK_DTOR LOG:\n");
	free(stk->data);
	stk->file_name = NULL;
	stk->func_name = NULL;
	stk->stk_name = NULL;
	stk->size = 0;
	stk->capacity = 0;
	printf("Stack has been destroyed\n");
}

int stack_verifier(Stack stk)
{
	int ok_ID = 0;
	if(stk.left_canary != 0xBADC0FFEE)
	{
		ok_ID |= 1;
	}

	ok_ID = ok_ID << 1;
	if(stk.right_canary != 0xBADC0FFEE)
	{
		ok_ID |= 1;
	}

	ok_ID = ok_ID << 1;
	if(stk.size > stk.capacity)
	{
		ok_ID |= 1;
	}

	ok_ID = ok_ID << 1;
	if(stk.data == NULL)
	{
		ok_ID |= 1;
	}

	ok_ID = ok_ID << 1;
	if(stk.size < 0)
	{
		ok_ID |= 1;
	}

	ok_ID = ok_ID << 1;
	if(stk.capacity < 0)
	{
		ok_ID |= 1;
	}

	ok_ID = ok_ID << 1;
	if(stk.file_name == NULL)
	{
		ok_ID |= 1;
	}

	ok_ID = ok_ID << 1;
	if(!stk.func_name)
	{
		ok_ID |= 1;
	}

	ok_ID = ok_ID << 1;
	if(stk.stk_name == NULL)
	{
		ok_ID |= 1;
	}

	return ok_ID;
}
