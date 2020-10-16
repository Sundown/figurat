#include "interpreter.h"

/* Interpreter? I barely know 'er! */

char* EXPAND(const char* INPUT) {
  char* OUTPUT = NULL;
  Error err = Error_OK;
  Noun expression, result;

  while (read(INPUT, &INPUT, &expression) == Error_OK) {
      err = eval(expression, INTERPRETER_ENV, &result);

      if (err) {
        exit_with_error_and_expression(err, expression);
      } else {
        /* to_string() handles string allocation. */
        OUTPUT = to_string(result);
      }
    }

  return OUTPUT;
}

void exit_with_error(Error err) {
  print_error(err);
  fclose(ROOT_CONFIG);

  exit(1);
}

void exit_with_error_and_expression(Error err, Noun expr) {
  print_error(err);
  print_expr(expr);
  fclose(ROOT_CONFIG);

  exit(1);
}

void INIT_INTERPRETER() {
  INTERPRETER_ENV = cons(INTERPRETER_ENV, nil);

	env_set(INTERPRETER_ENV, new_sym("head"), new_cfunc(builtin_head));
	env_set(INTERPRETER_ENV, new_sym("tail"), new_cfunc(builtin_tail));
	env_set(INTERPRETER_ENV, new_sym("cons"), new_cfunc(builtin_cons));
	env_set(INTERPRETER_ENV, new_sym("+"), new_cfunc(builtin_add));
}

Noun cons(Noun head_val, Noun tail_val) {
	Noun p;

	p.type = tuple_t;
	p.value.tuple = calloc(1, sizeof(struct Tuple));

	head(p) = head_val;
	tail(p) = tail_val;

	return p;
}

Noun new_num(double x) {
	Noun a;
	a.type = num_t;
	a.value.integer = x;
	return a;
}

Noun new_cfunc(CFunc fn) {
	Noun a;
	a.type = cfunc_t;
	a.value.cfunc = fn;
	return a;
}

Error new_subr(Noun env, Noun args, Noun body, Noun* result) {
	Noun p;

	if (!listp(args) || !listp(body)) {
		return Error_Syntax;
	}

	p = args;
	while (!nilp(p)) {
		if (head(p).type != sym_t) {
			return Error_Type;
		}

		p = tail(p);
	}

	*result = cons(env, cons(args, body));
	result->type = subr_t;

	return Error_OK;
}

Error env_get(Noun env, Noun symbol, Noun* result) {
	Noun parent = head(env);
	Noun bs = tail(env);

	while (!nilp(bs)) {
		Noun b = head(bs);
		if (head(b).value.symbol == symbol.value.symbol) {
			*result = tail(b);
			return Error_OK;
		}

		bs = tail(bs);
	}

	if (nilp(parent)) {
		return Error_Unbound;
  }

	return env_get(parent, symbol, result);
}

Error env_set(Noun env, Noun symbol, Noun value) {
	Noun bs = tail(env);
	Noun b = nil;

	while (!nilp(bs)) {
		b = head(bs);
		if (head(b).value.symbol == symbol.value.symbol) {
			tail(b) = value;
			return Error_OK;
		}

		bs = tail(bs);
	}

	b = cons(symbol, value);
	tail(env) = cons(b, tail(env));

	return Error_OK;
}

Error listp(Noun expr) {
	while (!nilp(expr)) {
		if (expr.type != tuple_t) {
			return 0;
    }

		expr = tail(expr);
	}

	return 1;
}

Noun new_sym(char* s) {
	Noun a, p;

	p = INTERPRETER_VARS;
	while (!nilp(p)) {
		a = head(p);

		if (!strcmp(a.value.symbol, s)) {
			return a;
    }

		p = tail(p);
	}

	a.type = sym_t;
	a.value.symbol = strdup(s);
	INTERPRETER_VARS = cons(a, INTERPRETER_VARS);

	return a;
}

char* to_string(Noun atom) {
	char* ret = calloc(1024, sizeof(char));
	char* ret2 = NULL;
	switch (atom.type) {
    case nil_t: {
      sprintf(ret, "nil");
      break;
    } case tuple_t: {
      putchar('(');
      print_expr(head(atom));
      atom = tail(atom);

      while (!nilp(atom)) {
        if (atom.type == tuple_t) {
          putchar(' ');
          print_expr(head(atom));
          atom = tail(atom);
        } else {
          sprintf(ret, " . ");
          print_expr(atom);
          break;
        }
      }

      putchar(')');
      break;
    } case str_t:
    case sym_t: {
      sprintf(ret, "%s", atom.value.symbol);
      break;
    } case subr_t: {
			sprintf(ret, "%s", to_string(tail(atom)));
			break;
		} case num_t: {
     	sprintf(ret, "%f", atom.value.integer);
      break;
    } case cfunc_t: {
      sprintf(ret, "#<BUILTIN>");
      break;
    }
  }

	ret2 = calloc(strlen(ret) + 1, sizeof(char));
	ret2 = strdup(ret);
	free(ret);

	return ret2;
}

void print_expr(Noun n) {
	printf("%s\n", to_string(n));
}

Noun copy_list(Noun list) {
	Noun a, p;

	if (nilp(list)) {
		return nil;
  }

	a = cons(head(list), nil);
	p = a;
	list = tail(list);

	while (!nilp(list)) {
		tail(p) = cons(head(list), nil);
		p = tail(p);
		list = tail(list);
	}

	return a;
}

Error apply(Noun fn, Noun args, Noun* result) {
	Noun env, arg_names, body;

	if (fn.type == cfunc_t) {
		return (*fn.value.cfunc)(args, result);
	} else if (fn.type != subr_t) {
		return Error_Type;
	}

	env = cons(head(fn), nil);
	arg_names = head(tail(fn));
	body = tail(tail(fn));

	/* Bind the arguments */
	while (!nilp(arg_names)) {
		if (nilp(args)) {
			return Error_Args;
		}

		env_set(env, head(arg_names), head(args));
		arg_names = tail(arg_names);
		args = tail(args);
	}

	if (!nilp(args)) {
		return Error_Args;
	}

	while (!nilp(body)) {
		Error err = eval(head(body), env, result);
		if (err) {
			return err;
		}

		body = tail(body);
	}

	return Error_OK;
}

Error tokenise(const char* str, const char** start, const char** end) {
	str += strspn(str, " \t\r\n");

	if (str[0] == '\0') {
		*start =* end = NULL;
		return Error_Syntax;
	}

	*start = str;

	if (strchr("()", str[0]) != NULL) {
		*end = str + 1;
	} else {
		*end = str + strcspn(str, "() \t\r\n");
	}

	return Error_OK;
}

Error parse(const char* start, const char* end, Noun* result) {
	char* buf,* p;

	/* Is it an integer? */
	double val = strtod(start, &p);
	if (p == end) {
		result->type = num_t;
		result->value.integer = val;
		return Error_OK;
	}

	/* NIL or symbol */
	buf = malloc(end - start + 1);
	p = buf;
	while (start != end)
		*p++ =* start, ++start;
	*p = '\0';

	if (strcmp(buf, "nil") == 0)
		*result = nil;
	else
		*result = new_sym(buf);

	free(buf);

	return Error_OK;
}

Error read_list(const char* start, const char** end, Noun* result) {
	Noun p;

	*end = start;
	p =* result = nil;

	for (;;) {
		const char* token;
		Noun item;
		Error err;

		err = tokenise(*end, &token, end);
		if (err)
			return err;

		if (token[0] == ')') {
			return Error_OK;
    }

		if (token[0] == '.' &&* end - token == 1) {
			/* Improper list */
			if (nilp(p)) {
				return Error_Syntax;
      }

			err = read(*end, end, &item);
			if (err) {
				return err;
      }

			tail(p) = item;

			/* Read the closing ')' */
			err = tokenise(*end, &token, end);
			if (!err && token[0] != ')')
				err = Error_Syntax;

			return err;
		}

		err = read(token, end, &item);
		if (err)
			return err;

		if (nilp(p)) {
			/* First item */
			*result = cons(item, nil);
			p =* result;
		} else {
			tail(p) = cons(item, nil);
			p = tail(p);
		}
	}
}

Error read(const char* input, const char** end, Noun* result) {
	const char* token;
	Error err;

	err = tokenise(input, &token, end);
	if (err) {
		return err;
  }

	if (token[0] == '(') {
		return read_list(*end, end, result);
	} else if (token[0] == ')') {
		return Error_Syntax;
	} else {
		return parse(token,* end, result);
  }
}

Error eval(Noun expr, Noun env, Noun* result) {
	Noun op, args, p;
	Error err;

	if (expr.type == sym_t) {
		return env_get(env, expr, result);
	} else if (expr.type != tuple_t) {
		*result = expr;
		return Error_OK;
	}

	if (!listp(expr)) {
		return Error_Syntax;
  }

	op = head(expr);
	args = tail(expr);

	if (op.type == sym_t) {
		if (!strcmp(op.value.symbol, "quote")) {
			if (nilp(args) || !nilp(tail(args))) {
				return Error_Args;
      }

			*result = head(args);
			return Error_OK;
		} else if (strcmp(op.value.symbol, "fn") == 0) {
			if (nilp(args) || nilp(tail(args))) {
				return Error_Args;
			}

			return new_subr(env, head(args), tail(args), result);
		} else if (!strcmp(op.value.symbol, "var")) {
			Noun sym, val;

			if (nilp(args) || nilp(tail(args)) || !nilp(tail(tail(args)))) {
				return Error_Args;
      }

			sym = head(args);
			if (sym.type != sym_t) {
				return Error_Type;
      }

			err = eval(head(tail(args)), env, &val);

			if (err) {
				return err;
      }

			*result = sym;
			return env_set(env, sym, val);
		}
	}

  /* Evaluate operator */
	err = eval(op, env, &op);
	if (err) {
		return err;
	}

	/* Evaulate arguments */
	args = copy_list(args);
	p = args;
	while (!nilp(p)) {
		err = eval(head(p), env, &head(p));
		if (err)
			return err;

		p = tail(p);
	}

	return apply(op, args, result);
}

Error builtin_head(Noun args, Noun* result) {
	if (nilp(args) || !nilp(tail(args))) {
		return Error_Args;
  }

	if (nilp(head(args))) {
		*result = nil;
  } else if (head(args).type != tuple_t) {
		return Error_Type;
	} else {
    *result = head(head(args));
  }

	return Error_OK;
}

void print_error(Error err) {
	printf("Error: ");
	switch (err) {
		case Error_OK:
			break;
		case Error_Args:
			printf("args");
			break;
		case Error_Type:
			printf("type");
			break;
		case Error_Syntax:
			printf("syntax");
			break;
		case Error_Unbound:
			printf("unbound");
			break;
		default:
			printf("unknown");
			break;
	}

	putchar('\n');
}
