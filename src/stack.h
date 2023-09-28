#ifndef STACK_H
#define STACK_H

#define STACK_DUMP(stk) printf("%s[%p] is not ok\nleft_canary[%p] = %llX"\
								"\nright_canary[%p] = %llX\nbuf_left_canary[%p] = %llX\n"\
								"buf_right_canary[%p] = %llX\nsize = %lu\ncapacity = %lu\n"\
								"data addres = %p\nfunction: %s\nline: %lu\nfile: %s\n",\
								#stk, &(stk), &(stk).left_canary, \
								(stk).left_canary,&(stk).right_canary, (stk).right_canary, \
								(stk).buf_left_canary, *(stk).buf_left_canary,  \
								(stk).buf_right_canary, *(stk).buf_right_canary, \
								(stk).size, (stk).capacity, (stk).data,\
								(stk).func_name, (stk).line, (stk).file_name);\
								for(size_t stk_ID = 0; stk_ID < (stk).capacity; stk_ID++) \
								{\
									printf("[%lu] = %d\n", stk_ID, (stk).data[stk_ID]);\
								}\
								show_bits((stk).error_code);

#define ACTUALIZED_STACK_DUMP(stk)	(stk)->file_name = __FILE__;\
									(stk)->func_name = __func__;\
									(stk)->line = __LINE__;\
									STACK_DUMP(*(stk));\
									exit(EXIT_FAILURE);

#define LOG(func_name)printf("\n%s LOG:\n", #func_name)

#define CHECK_CONDITION(error_ID, cond, ERROR_TYPE)\
	if((cond))\
	{\
		(error_ID) |= (ERROR_TYPE);\
	}\

#define SMART_DUMP(stk)\
	if((stk) == NULL)\
	{\
		fprintf(stderr, "Stack is not ok. Stack address: %p", (stk));\
	}\
	else\
	{\
		ACTUALIZED_STACK_DUMP((stk));\
	}\

typedef int elem_t;
typedef unsigned long long canary_t;

const canary_t canary_value = 0xBADC0FEE;
const canary_t buf_canary_value = 0xBAD;
//macros
//case ALL_GOOD:
//	printf("ALL_GOOD");
//	break;


enum Err_ID //считать побитово
{
	ALL_GOOD = 0, // прописать сдвигами
	SIZE_ZERO = 1,
	SIZE_IS_GREATER = 2,
	DATA_NULL_PTR = 4,
	CAP_ZERO = 8,
	FILE_NAME_NULL_PTR = 16,
	FUNC_NAME_NULL_PTR = 32,
	STK_NAME_NULL_PTR = 64,
	L_CANARY_DIED = 128,
	R_CANARY_DIED = 256,
	L_BUF_CANARY_DIED = 512,
	R_BUF_CANARY_DIED = 1024,
};


struct Stack_pop_result
{
	enum Err_ID error_code;
	elem_t deleted_element;
};

struct Stack
{
	canary_t left_canary; //canary_t

	canary_t *buf_left_canary;
	elem_t *data;
	canary_t *buf_right_canary;

	size_t size;
	size_t capacity;
	const char *stk_name; //debug struct
	const char *file_name;
	const char *func_name;
	size_t line;
	enum Err_ID error_code;

	size_t hash_check_value;

	canary_t right_canary;
};

void stack_ctor(Stack *stk, size_t starter_capacity,
				const char *stk_name, const char *file_name,
				size_t line, const char *func_name);
void stack_dtor(Stack *stk);
void stack_push(Stack *stk, elem_t value);
struct Stack_pop_result stack_pop(Stack *stk);
int stack_verifier(const Stack *stk);
void ptr_redirect(Stack *stk);
void hash_count(Stack *stk);
void hash_check_n_count(Stack *stk);
void stack_buf_realloc(Stack *stk);


#endif
