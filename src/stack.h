#ifndef STACK_H
#define STACK_H

#define DEBUG

#define LOG(stk) fprintf((stk)->log_file ,"\n%s LOG:\n", __func__)

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

enum Err_ID
{
	ALL_GOOD 			 = 0,
	SIZE_ZERO 			 = 1 << 0,
	SIZE_IS_GREATER		 = 1 << 1,
	DATA_NULL_PTR		 = 1 << 2,
	FILE_NAME_NULL_PTR	 = 1 << 3,
	FUNC_NAME_NULL_PTR	 = 1 << 4,
	DATA_HASH_AINT_RIGHT = 1 << 5,
	L_CANARY_DIED		 = 1 << 6,
	R_CANARY_DIED		 = 1 << 7,
	L_BUF_CANARY_DIED	 = 1 << 8,
	R_BUF_CANARY_DIED	 = 1 << 9,
	STK_NULL_PTR 		 = 1 << 10,
	NOT_ENOUGH_MEM		 = 1 << 11,
	HASH_AINT_RIGHT		 = 1 << 12,
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
	FILE *log_file;

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
size_t hash_count(void *stk, size_t hash_object_size);
void hash_check_n_count(Stack *stk);
void hash_data_check_n_count(Stack *stk);
void stack_dump(const Stack *stk, const char *stk_name, enum Err_ID error_code);
void print_stack_elems(Stack *stk);
void ultimate_stack_hash_count(Stack *stk);
void update_stack_position( Stack *stk, const char *file_name,
							size_t line, const char *func_name);
bool unit_test_1(Stack *stk);


#endif
