#ifndef STACK_H
#define STACK_H

#define CANARY_PROTECTION_ON

#define DEBUG

#ifdef CANARY_PROTECTION_ON
	#define IF_DEFINED_CANARY(TODO) TODO
#else
	#define IF_DEFINED_CANARY(TODO)
#endif

#define STACK_DUMP(stk, error_code) printf("%s[%p] is not ok\n"											\
								"left_canary[%p] = %llX"                  \
							    "\nright_canary[%p] = %llX\nbuf_left_canary[%p] = %llX\n"   \
								"buf_right_canary[%p] = %llX\n"							\
								"size = %lu\ncapacity = %lu\n"								\
								"data addres = %p\nfunction: %s\nline: %lu\nfile: %s\n",	\
								#stk, &(stk),												\
								&(stk).left_canary,						\
								(stk).left_canary,&(stk).right_canary, (stk).right_canary,	\
								(stk).buf_left_canary, *(stk).buf_left_canary,				\
								(stk).buf_right_canary, *(stk).buf_right_canary,			\
								(stk).size, (stk).capacity, (stk).data,						\
								(stk).func_name, (stk).line, (stk).file_name);				\
								for(size_t stk_ID = 0; stk_ID < (stk).capacity; stk_ID++)	\
								{															\
									printf("[%lu] = %d\n", stk_ID, (stk).data[stk_ID]);		\
								}															\
								show_bits(error_code);

#define ACTUALIZED_STACK_DUMP(stk, error_code)\
									(stk)->file_name = __FILE__;\
									(stk)->func_name = __func__;\
									(stk)->line = __LINE__;\
									STACK_DUMP(*(stk), error_code);\
									exit(EXIT_FAILURE);

#define LOG(func_name)printf("\n%s LOG:\n", #func_name) //log file в стек

#define SMART_DUMP(stk, error_code)\
	if((stk) == NULL)\
	{\
		fprintf(stderr, "Stack is not ok. Stack address: %p", (stk));\
	}\
	else\
	{\
		stack_dump(stk, #stk, error_code);\
	}\

#define STACK_PUSH(stk, push_value) stack_push((stk), (push_value), __FILE__, __func__, __LINE__);
#define STACK_POP(stk) stack_pop((stk), __FILE__, __func__, __LINE__);
#define STACK_CTOR(stk, starter_capacity) stack_ctor(  (stk), starter_capacity,\
														__FILE__, __LINE__, __func__);

typedef int elem_t;
typedef unsigned long long canary_t;

const canary_t canary_value = 0xBADC0FFEE;
const canary_t buf_canary_value = 0xBAD;
//macros
//case ALL_GOOD:
//	printf("ALL_GOOD");
//	break;


enum Err_ID
{
	ALL_GOOD = 0, //выровнить
	SIZE_ZERO = 1 << 0,
	SIZE_IS_GREATER = 1 << 1,
	DATA_NULL_PTR = 1 << 2,
	CAP_ZERO = 1 << 3,
	FILE_NAME_NULL_PTR = 1 << 4,
	FUNC_NAME_NULL_PTR = 1 << 5,
	DATA_HASH_AINT_RIGHT = 1 << 6,
	L_CANARY_DIED = 1 << 7,
	R_CANARY_DIED = 1 << 8,
	L_BUF_CANARY_DIED = 1 << 9,
	R_BUF_CANARY_DIED = 1 << 10,
	STK_NULL_PTR = 1 << 11,
	NOT_ENOUGH_MEM = 1 << 12,
	HASH_AINT_RIGHT = 1 << 13,

};


struct Stack_pop_result
{
	enum Err_ID error_code;
	elem_t deleted_element;
};

struct Stack
{
	canary_t left_canary;

	canary_t *buf_left_canary;
	elem_t *data;
	canary_t *buf_right_canary;

	size_t size;
	size_t capacity;
	const char *file_name;
	const char *func_name;
	size_t line;

	size_t hash_check_value;
	size_t hash_data_check_value;

	canary_t right_canary;
};

enum Err_ID stack_ctor( Stack *stk, size_t starter_capacity,
						const char *file_name,
						size_t line, const char *func_name);
enum Err_ID stack_push( Stack *stk, elem_t value,
						const char *file_name, const char *func_name, size_t line);
struct Stack_pop_result stack_pop(  Stack *stk, const char *file_name,
									const char *func_name, size_t line);
void stack_dtor(Stack *stk);
enum Err_ID stack_verifier(const Stack *stk);
void stack_buf_realloc(Stack *stk);
void ptr_realloc_redirect(Stack *stk);
size_t hash_count(Stack *stk);
void hash_check_n_count(Stack *stk);
void stack_dump(const Stack *stk, const char *stk_name, enum Err_ID error_code);

void hash_data_count(Stack *stk);
void hash_data_check_n_count(Stack *stk);


#endif
