#ifndef FIG_INTERPRETER_HEADER
#define FIG_INTERPRETER_HEADER
#include "../globals.h"

typedef enum {
	Error_OK = 0,
  Error_Args,
  Error_Type,
	Error_Syntax,
  Error_Unbound
} Error;

typedef enum {
		nil_t = 0,
		sym_t,
		num_t,
    str_t,
    subr_t,
		tuple_t,
    cfunc_t
} Type;

typedef struct Noun Noun;

typedef Error (*CFunc)(Noun args, Noun* result);

typedef union {
  CFunc cfunc;
  struct Tuple* tuple;
  const char* symbol;
  double integer;
} Value;

struct Noun {
  Type type;
	Value value;
};

struct Tuple {
	Noun atom[2];
};

#define head(p) ((p).value.tuple->atom[0])
#define tail(p) ((p).value.tuple->atom[1])
#define nilp(atom) ((atom).type == nil_t)

static Noun
  nil = { nil_t, { NULL } },
  INTERPRETER_ENV = { nil_t, { NULL } },
  INTERPRETER_VARS = { nil_t, { NULL } };

char* to_string(Noun atom);

void
  print_expr(Noun n),
  print_error(Error err),
  exit_with_error(Error err),
  exit_with_error_and_expression(Error err, Noun expr);

Noun
  new_sym(char* s),
  new_num(double x),
  new_cfunc(CFunc fn),
  copy_list(Noun list),
  cons(Noun head_val, Noun tail_val);

Error
  listp(Noun expr),
  apply(Noun fn, Noun args, Noun* result),
  eval(Noun expr, Noun env, Noun* result),
  env_set(Noun env, Noun symbol, Noun value),
  env_get(Noun env, Noun symbol, Noun* result),
  new_subr(Noun env, Noun args, Noun body, Noun* result),
  read(const char* input, const char** end, Noun* result),
  parse(const char* start, const char* end, Noun* result),
  read_list(const char* start, const char** end, Noun* result),
  tokenise(const char* str, const char** start, const char** end);

Error
  builtin_head(Noun args, Noun* result),
  builtin_tail(Noun args, Noun* result),
  builtin_cons(Noun args, Noun* result),
  builtin_add(Noun args, Noun* result),
  builtin_sub(Noun args, Noun* result),
  builtin_mul(Noun args, Noun* result),
  builtin_div(Noun args, Noun* result);

#endif /* FIG_INTERPRETER_HEADER */
