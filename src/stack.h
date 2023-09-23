#ifndef STACK_H
#define STACK_H

#define STACK_DUMP(stk) printf("%s[%p] is not ok\nsize = %lu\ncapacity = %lu\n"\
								"data addres = %p\nfunction: %s\nline: %lu\nfile: %s\n",\
								#stk, &(stk) ,(stk).size, (stk).capacity, (stk).data,\
								(stk).func_name, (stk).line, (stk).file_name);\
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
	unsigned long long left_canary;

	elem_t *data;
	size_t size;
	size_t capacity;
	const char *stk_name;
	const char *file_name;
	const char *func_name;
	size_t line;
	
	unsigned long long right_canary;
};

void stack_ctor(Stack *stk, size_t default_capacity, const char *stk_name, const char *file_name, size_t line, const char *func_name);
void stack_dtor(Stack *stk);
void stack_push(Stack *stk, elem_t value);
struct Stack_pop_result stack_pop(Stack *stk);
bool stack_verifier(Stack stk);





#endif
