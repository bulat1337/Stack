#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <assert.h>

#include "stack.h"
#include "helpers.h"

enum Err_ID stack_ctor( Stack *stk, size_t starter_capacity,
						const char *file_name,
						size_t line, const char *func_name)
{
	if(!stk)
	{
		#ifdef DEBUG
			fprintf(stderr, "Stack address is NULL\n");
		#endif

		return STK_NULL_PTR;
	}

	stk->left_canary = canary_value;

	stk->capacity = starter_capacity;

	size_t data_size = sizeof(elem_t) * stk->capacity;
	size_t alignment_space = sizeof(canary_t) - data_size % sizeof(canary_t);
	stk->data = (elem_t *)calloc(1, data_size + 2 * sizeof(canary_t) + alignment_space);

	if(stk->data == NULL)
	{
		#ifdef DEBUG
			fprintf(stderr, "Not enough memory for allocation\n");
		#endif

		return	NOT_ENOUGH_MEM;
	}

	ptr_realloc_redirect(stk);

	stk->size = 0;
	stk->file_name = file_name;
	stk->func_name = func_name;
	stk->line = line;

	stk->right_canary = canary_value;
	stk->hash_check_value = hash_count((Stack *)stk); //stk->hash_check_value = hash_count(stk)

	return	ALL_GOOD;
}

enum Err_ID stack_push( Stack *stk, elem_t value,
						const char *file_name, const char *func_name, size_t line)
{
	LOG(STACK_PUSH);

	enum Err_ID error_code;

	stk->file_name = file_name; //copypaste можно макрос?
	stk->func_name = func_name;
	stk->line = line;

	hash_check_n_count(stk);


	#ifdef DEBUG
		if((error_code = stack_verifier(stk)) != ALL_GOOD)
		{
			SMART_DUMP(stk, error_code);
		}
	#endif

	if((error_code = stack_verifier(stk)) != ALL_GOOD)
	{
		return error_code;
	}

	if(stk->size >= stk->capacity)
	{
		stk->capacity *= 2; //сделать возможным расширение с нуля

		stack_buf_realloc(stk);//редирект в реаллок

		printf("new capacity: expanded\n");
	}
	else
	{
		stk->data[stk->size] = value;
		stk->size++;
	}

	for(size_t i = 0; i < stk->capacity; i++)//copyp
	{
		printf("[%lu]%d ", i, (stk->data)[i]);
	}
	printf("\nsize = %lu, capacity = %lu", stk->size, stk->capacity);
	printf("\n");

	stk->hash_check_value = hash_count((Stack *)stk);

	return error_code;
}

struct Stack_pop_result stack_pop(  Stack *stk, const char *file_name,
									const char *func_name, size_t line)
{
	LOG(STACK_POP);//func__

	struct Stack_pop_result result = {};

	stk->file_name = file_name; //copypaste
	stk->func_name = func_name;
	stk->line = line;

	hash_check_n_count(stk);

	#ifdef DEBUG
		if((result.error_code = stack_verifier(stk)) != ALL_GOOD)
		{
			SMART_DUMP(stk, result.error_code); //smart_dump(stk, __LINE__);
		}
	#endif

	if((result.error_code = stack_verifier(stk)) != ALL_GOOD)
	{
		return result;
	}

	if(stk->size == 0)
	{
		result.error_code = SIZE_ZERO;

		#ifdef DEBUG
			fprintf(stderr, "Stack_pop failed: size == 0\n");
		#endif

		return result;
	}

	(stk->size)--;
	result.deleted_element = (stk->data)[stk->size];
	if(stk->size <= stk->capacity / (2 * 2))
	{
		printf("Here is why capacity is going to be shrinked:\n"); //fpritf all log
		printf("\t size[%lu] <= capacity[%lu]\n", stk->size, stk->capacity / (2 * 2));
		stk->capacity /= 2;

		stack_buf_realloc(stk);

		printf("new capacity: shrinked\n");
	}

	for(size_t i = 0; i < stk->capacity; i++) //copypaste log
	{
		printf("[%lu]%d ", i, (stk->data)[i]);
	}
	printf("\nsize = %lu, capacity = %lu", stk->size, stk->capacity);
	printf("\n");

	stk->hash_check_value = hash_count((Stack *)stk);

	return result;
}

void stack_dtor(Stack *stk)
{
	hash_check_n_count(stk);
	LOG(STACK_DTOR);
	free(stk->buf_left_canary);
	stk->file_name = NULL;
	stk->func_name = NULL;
	stk->size = 0;
	stk->capacity = 0;
	printf("Stack has been destroyed\n");
}

enum Err_ID stack_verifier(const Stack *stk)
{
	int error_ID = 0;

	#define CHECK_CONDITION(cond, ERROR_TYPE)					\
	if((cond))													\
	{															\
		(error_ID) |= (ERROR_TYPE);								\
	}															\

	CHECK_CONDITION(stk->size < 0, SIZE_ZERO);
	CHECK_CONDITION(stk->size > stk->capacity, SIZE_IS_GREATER);
	CHECK_CONDITION(stk->data == NULL, DATA_NULL_PTR);
	CHECK_CONDITION(stk->capacity <= 0, CAP_ZERO); //такая проверка не нужнаБ ситуация с расширением нулевого стека корректна
	CHECK_CONDITION(stk->file_name == NULL, FILE_NAME_NULL_PTR);
	CHECK_CONDITION(stk->func_name == NULL, FUNC_NAME_NULL_PTR);
	CHECK_CONDITION(stk->left_canary != canary_value, L_CANARY_DIED);
	CHECK_CONDITION(stk->right_canary != canary_value, R_CANARY_DIED);
	CHECK_CONDITION(*(stk->buf_left_canary) != buf_canary_value, L_BUF_CANARY_DIED);
	CHECK_CONDITION(*(stk->buf_right_canary) != buf_canary_value, R_BUF_CANARY_DIED);

	#undef CHECK_CONDITION

	return (enum Err_ID)error_ID;
}

size_t hash_count(void *stk) //void * //hash должен быть применим к другим типам данных
{
	size_t hash_check_value = 0;
	for(size_t stk_elem_ID = 0; stk_elem_ID < sizeof(stk); stk_elem_ID++)
	{
		hash_check_value += (size_t)*((char*)stk + stk_elem_ID);
	}

	return hash_check_value;
}

void hash_data_count(Stack *stk)
{
	stk->hash_data_check_value = 0;
	for(size_t stk_elem_ID = 0; stk_elem_ID < sizeof(stk->data); stk_elem_ID++)
	{
		stk->hash_data_check_value+= (unsigned long)*((char*)(&stk->data) + stk_elem_ID);
	}
}

void hash_data_check_n_count(Stack *stk)
{
	size_t temp_hash_data_check_value = stk->hash_data_check_value;

	stk->hash_data_check_value = 0;
	for(size_t stk_elem_ID = 0; stk_elem_ID < sizeof(stk->data); stk_elem_ID++) //sizeof(stk->data) -> capacity
	{//copypaste
		stk->hash_data_check_value+= (unsigned long)*((char*)(stk->data) + stk_elem_ID);
	}

	if(stk->hash_data_check_value != temp_hash_data_check_value)
	{
		enum Err_ID error_code = DATA_HASH_AINT_RIGHT;
		#ifdef DEBUG
			fprintf(stderr ,"hash_data_check_value[%lu] != temp_hash_data_check_value[%lu]\n",
					stk->hash_data_check_value, temp_hash_data_check_value);
		#endif

		SMART_DUMP(stk, error_code)
	}
}

void hash_check_n_count(Stack *stk)
{
	size_t temp_hash_check_value = stk->hash_check_value;
	stk->hash_check_value = 0;
	for(size_t stk_elem_ID = 0; stk_elem_ID < sizeof(stk); stk_elem_ID++)
	{
		stk->hash_check_value += (unsigned long)*((char*)(stk) + stk_elem_ID);
	}
	if(stk->hash_check_value != temp_hash_check_value)
	{
		enum Err_ID error_code = HASH_AINT_RIGHT;
		#ifdef DEBUG
			fprintf(stderr ,"hash_check_value[%lu] != temp_hash_check_value[%lu]\n",
					stk->hash_check_value, temp_hash_check_value);
		#endif

		SMART_DUMP(stk, error_code)
	}
}

void stack_buf_realloc(Stack *stk)
{
	size_t data_size = sizeof(elem_t) * stk->capacity;
	size_t alignment_space = sizeof(canary_t) - data_size % sizeof(canary_t);
	stk->data =
		(elem_t *)realloc(stk->buf_left_canary,
		data_size + 2 * sizeof(canary_t) + alignment_space);

	ptr_realloc_redirect(stk);
}

void ptr_realloc_redirect(Stack *stk)
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

void stack_dump(const Stack *stk, const char *stk_name, enum Err_ID error_code)
{
	printf( "%s[%p] is not ok\n"
			"left_canary[%p] = %llX\nright_canary[%p] = %llX\n"
			"buf_left_canary[%p] = %llX\nbuf_right_canary[%p] = %llX\n"
			"size = %lu\ncapacity = %lu\n"
			"data addres = %p\nfunction: %s\nline: %lu\nfile: %s\n",
			stk_name, (stk),
			&( (stk)->left_canary ),
			(stk)->left_canary, &( (stk)->right_canary ), (stk)->right_canary,
			(stk)->buf_left_canary, *( (stk)->buf_left_canary ),
			(stk)->buf_right_canary, *( (stk)->buf_right_canary ),
			(stk)->size, (stk)->capacity, (stk)->data,
			(stk)->func_name, (stk)->line, (stk)->file_name);

	for(size_t stk_ID = 0; stk_ID < (stk)->capacity; stk_ID++)
	{
		printf("[%lu] = %d\n", stk_ID, (stk)->data[stk_ID]);
	}

	show_bits(error_code);
}
