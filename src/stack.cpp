#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <assert.h>

#include "stack.h"
#include "helpers.h"

void stack_ctor(Stack *stk, size_t starter_capacity,
				const char *stk_name, const char *file_name,
				size_t line, const char *func_name)
{
	assert(stk);

	stk->left_canary = canary_value;

	stk->capacity = starter_capacity;

	size_t data_size = sizeof(elem_t) * stk->capacity;
	size_t alignment_space = sizeof(canary_t) - data_size % sizeof(canary_t);
	stk->data = (elem_t *)calloc(1, data_size + 2 * sizeof(canary_t) + alignment_space);

	if(stk->data == NULL)
	{
		printf("Not enough memory for allocation");
		exit(EXIT_FAILURE);
	}

	ptr_redirect(stk);

	stk->size = 0;
	stk->stk_name = stk_name;
	stk->file_name = file_name;
	stk->func_name = func_name;
	stk->line = line;

	stk->right_canary = canary_value;

	hash_count(stk);
}

void stack_push(Stack *stk, elem_t value)
{
	LOG(STACK_PUSH);

	hash_check_n_count(stk);

	if((stk->error_code = (enum Err_ID)stack_verifier(stk)) != ALL_GOOD)
	{
		SMART_DUMP(stk);
	}

	if(stk->size >= stk->capacity)
	{
		stk->capacity *= 2;

		stack_buf_realloc(stk);

		ptr_redirect(stk);

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


	stk->hash_check_value = 0;
	for(size_t stk_elem_ID = 0; stk_elem_ID < sizeof(stk); stk_elem_ID++)
	{
		stk->hash_check_value += *((char*)(&stk) + stk_elem_ID);
	}

	hash_count(stk);
}

struct Stack_pop_result stack_pop(Stack *stk)
{
	hash_check_n_count(stk);

	LOG(STACK_POP);
	if((stk->error_code = (enum Err_ID)stack_verifier(stk)) != ALL_GOOD)
	{
		SMART_DUMP(stk);
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
		exit(EXIT_FAILURE); //return errorcode  w all wxit_failures
	}

	(stk->size)--;
	result.deleted_element = (stk->data)[stk->size];
	if(stk->size <= stk->capacity / (2 * 2))
	{
		printf("Here is why capacity is going to be shrinked:\n"); //fpritf all log
		printf("\t size[%lu] <= capacity[%lu]\n", stk->size, stk->capacity / (2 * 2));
		stk->capacity /= 2;

		stack_buf_realloc(stk);
		ptr_redirect(stk);

		printf("new capacity: shrinked\n");
	}

	for(size_t i = 0; i < stk->capacity; i++) //copypaste log
	{
		printf("[%lu]%d ", i, (stk->data)[i]);
	}
	printf("\nsize = %lu, capacity = %lu", stk->size, stk->capacity);
	printf("\n");

	hash_count(stk);

	return result;
}

void stack_dtor(Stack *stk)
{
	if(stack_verifier(stk) != ALL_GOOD)
	{
		fprintf(stderr, "Stack has already been destructed\n");
		exit(EXIT_FAILURE);
	}
	hash_check_n_count(stk);
	LOG(STACK_DTOR);
	free(stk->buf_left_canary);
	stk->file_name = NULL;
	stk->func_name = NULL;
	stk->stk_name = NULL;
	stk->size = 0;
	stk->capacity = 0;
	printf("Stack has been destroyed\n");
}

int stack_verifier(const Stack *stk)
{
	int error_ID = 0;

	CHECK_CONDITION(error_ID, stk->size < 0, SIZE_ZERO);
	CHECK_CONDITION(error_ID, stk->size > stk->capacity, SIZE_IS_GREATER);
	CHECK_CONDITION(error_ID, stk->data == NULL, DATA_NULL_PTR);
	CHECK_CONDITION(error_ID, stk->capacity < 0, CAP_ZERO);
	CHECK_CONDITION(error_ID, stk->file_name == NULL, FILE_NAME_NULL_PTR);
	CHECK_CONDITION(error_ID, stk->func_name == NULL, FUNC_NAME_NULL_PTR);
	CHECK_CONDITION(error_ID, stk->stk_name == NULL, STK_NAME_NULL_PTR);
	CHECK_CONDITION(error_ID, stk->left_canary != canary_value, L_CANARY_DIED);
	CHECK_CONDITION(error_ID, stk->right_canary != canary_value, R_CANARY_DIED);
	CHECK_CONDITION(error_ID, *(stk->buf_left_canary) != buf_canary_value, L_BUF_CANARY_DIED);
	CHECK_CONDITION(error_ID, *(stk->buf_right_canary) != buf_canary_value, R_BUF_CANARY_DIED);

	return error_ID;
}

void ptr_redirect(Stack *stk)
{
	size_t data_size = sizeof(elem_t) * stk->capacity;
	size_t alignment_space = sizeof(canary_t) - data_size % sizeof(canary_t);
	size_t additional_mem = sizeof(canary_t) + alignment_space;

	stk->buf_left_canary = (canary_t *)stk->data;
	stk->buf_right_canary = (canary_t *)((char *)(stk->data) + additional_mem + data_size);
	stk->data = stk->data + (sizeof(canary_t) / sizeof(elem_t));
	*(stk->buf_left_canary) = buf_canary_value;
	*(stk->buf_right_canary) = buf_canary_value;
}

void hash_count(Stack *stk)
{
	stk->hash_check_value = 0;
	for(size_t stk_elem_ID = 0; stk_elem_ID < sizeof(stk); stk_elem_ID++)
	{
		stk->hash_check_value += *((char*)(&stk) + stk_elem_ID);
	}
}

void hash_check_n_count(Stack *stk)
{
	size_t temp_hash_check_value = stk->hash_check_value;
	stk->hash_check_value = 0;
	for(size_t stk_elem_ID = 0; stk_elem_ID < sizeof(stk); stk_elem_ID++)
	{
		stk->hash_check_value += *((char*)(&stk) + stk_elem_ID);
	}
	if(stk->hash_check_value != temp_hash_check_value)
	{
		printf("hash_check_value[%lu] != temp_hash_check_value[%lu]\n",
				stk->hash_check_value, temp_hash_check_value);

		ACTUALIZED_STACK_DUMP(stk)
	}
}

void stack_buf_realloc(Stack *stk)
{
	size_t data_size = sizeof(elem_t) * stk->capacity;
	size_t alignment_space = sizeof(canary_t) - data_size % sizeof(canary_t);
	stk->data =
		(elem_t *)realloc(stk->buf_left_canary,
		data_size + 2 * sizeof(canary_t) + alignment_space);
}
