#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#define STACK_DUMP(stk) printf("stack is not ok\nsize = %lu\ncapacity = %lu\n"\
								"data addres = %p\nline: %d\nfunction: %s\nfile: %s\n",\
								(stk).size, (stk).capacity, (stk).data,\
								__LINE__, __func__, __FILE__);\
								for(size_t stk_ID = 0; stk_ID < (stk).capacity; stk_ID++) \
								{\
									printf("[%lu] = %d\n", stk_ID, (stk).data[stk_ID]);\
								}


typedef int elem_t;

enum Err_ID
{
	ALL_GOOD = 0,
	SIZE_ZERO = 1,
};

struct Stack_pop_result
{
	enum Err_ID error_code;
	elem_t deleted_element;
};

struct Stack
{
	elem_t *data;
	size_t size;
	size_t capacity;
};

void stack_ctor(Stack *stk, size_t default_capacity = 10);
void stack_dtor(Stack *stk);
void stack_push(Stack *stk, elem_t value);
struct Stack_pop_result stack_pop(Stack *stk);
bool stack_verifier(Stack stk);

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
		.size = 0
	};
	stack_ctor(&stk, 10);

	if(stack_verifier(stk) != 1)
	{
		STACK_DUMP(stk);
	}
	
	return 0;
}

void stack_ctor(Stack *stk, size_t default_capacity)
{
	stk->data = (elem_t *)calloc(10, sizeof(elem_t));
	if(stk->data == NULL)
	{
		printf("Not enoough memory for allocation");
		exit(EXIT_FAILURE);
	}
	stk->size = 0;
	stk->capacity = default_capacity;
}

void stack_push(Stack *stk, elem_t value)
{
	if(stack_verifier(*stk) != 1)
	{
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

	for(size_t i = 0; i <= stk->capacity; i++)
	{
		printf("%d", (stk->data)[i]);
	}
	printf("\t size = %lu, capacity = %lu", stk->size, stk->capacity);
	printf("\n");
}

struct Stack_pop_result stack_pop(Stack *stk)
{
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
		printf("\t %lu <= %lu\n", stk->size, stk->capacity / (2 * 2));
		stk->capacity /= 2;
		stk->data = (elem_t *)realloc(stk->data, stk->capacity * sizeof(elem_t)); //copy paste1
		printf("new capacity: shrinked\n");
	}

	for(size_t i = 0; i <= stk->capacity; i++)
	{
		printf("%d", (stk->data)[i]);
	}
	printf("\t size = %lu, capacity = %lu", stk->size, stk->capacity);
	printf("\n");

	return result;
}

void stack_dtor(Stack *stk)
{
	free(stk->data);
	stk->size = 0;
	stk->capacity = 0;
}

void show_bits(int x)
{
    int i = 0;
    for (i = (sizeof(int) * 8) - 1; i >= 0; i--)
    {
       putchar(x & (1 << i) ? '1' : '0');
	   if(i % 4 == 0)
	   {
			putchar(' ');
	   }
    }
    printf("\n");
}

bool stack_verifier(Stack stk)
{
	bool ok_ID = 1;
	if(!stk.data)
		ok_ID = 0;
	if(stk.size > stk.capacity)
		ok_ID = 0;
	if(stk.data == NULL)
		ok_ID = 0;

	return ok_ID;
}
