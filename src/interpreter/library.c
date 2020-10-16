#include "interpreter.h"

Error builtin_tail(Noun args, Noun* result) {
	if (nilp(args) || !nilp(tail(args))) {
		return Error_Args;
  }

	if (nilp(head(args))) {
		*result = nil;
  } else if (head(args).type != tuple_t) {
		return Error_Type;
  } else {
		*result = tail(head(args));
  }

	return Error_OK;
}

Error builtin_cons(Noun args, Noun* result) {
	if (nilp(args) || nilp(tail(args)) || !nilp(tail(tail(args)))) {
		return Error_Args;
  }

	*result = cons(head(args), head(tail(args)));

	return Error_OK;
}

Error builtin_add(Noun args, Noun* result) {
	Noun a, b;

	if (nilp(args) || nilp(tail(args)) || !nilp(tail(tail(args)))) {
		return Error_Args;
  }

	a = head(args);
	b = head(tail(args));

	if (a.type != num_t || b.type != num_t) {
		return Error_Type;
  }

	*result = new_num(a.value.integer + b.value.integer);

	return Error_OK;
}

Error builtin_sub(Noun args, Noun* result) {
	Noun a, b;

	if (nilp(args) || nilp(tail(args)) || !nilp(tail(tail(args)))) {
		return Error_Args;
  }

	a = head(args);
	b = head(tail(args));

	if (a.type != num_t || b.type != num_t) {
		return Error_Type;
  }

	*result = new_num(a.value.integer - b.value.integer);

	return Error_OK;
}

Error builtin_mul(Noun args, Noun* result) {
	Noun a, b;

	if (nilp(args) || nilp(tail(args)) || !nilp(tail(tail(args)))) {
		return Error_Args;
  }

	a = head(args);
	b = head(tail(args));

	if (a.type != num_t || b.type != num_t) {
		return Error_Type;
  }

	*result = new_num(a.value.integer*  b.value.integer);

	return Error_OK;
}

Error builtin_div(Noun args, Noun* result) {
	Noun a, b;

	if (nilp(args) || nilp(tail(args)) || !nilp(tail(tail(args)))) {
		return Error_Args;
  }

	a = head(args);
	b = head(tail(args));

	if (a.type != num_t || b.type != num_t) {
		return Error_Type;
  }

	*result = new_num(a.value.integer / b.value.integer);

	return Error_OK;
}
