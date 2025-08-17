#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// How many functions a context should keep?
#define FNS_LIMIT 16

// In x86_64 and C ABI, only 6 arguments can fit in registers
// without the help of stack.
#define ARGS_LIMIT 6

typedef struct defer_fn {
	void *fn;
	void *args[ARGS_LIMIT];
	int  args_count;
} defer_fn_t;

typedef struct defer_ctx {
	defer_fn_t fns[FNS_LIMIT];
	int fns_count;
} defer_ctx_t;

// This function is defined in defer.s
extern void defer_call(void *fn, void *args[ARGS_LIMIT], int args_count);

// add a function and it's arguments to defer context's stack
void defer_add(defer_ctx_t *ctx, void *fn, void *args[ARGS_LIMIT], int args_count) {
	if (ctx->fns_count == FNS_LIMIT)
		return;
	defer_fn_t *current_fn = &ctx->fns[ctx->fns_count++];
	current_fn->args_count = args_count;
	if (args_count > 0)
		memcpy(current_fn->args, args, sizeof(void*) * ARGS_LIMIT);
	current_fn->fn = fn;
}

// call all registered functions
void defer_do(defer_ctx_t *ctx) {
	int i;
	for (i = 0; i < ctx->fns_count; ++i)
		defer_call(ctx->fns[i].fn, ctx->fns[i].args, ctx->fns[i].args_count);
}

void print_defer(void) {
	puts("printing a string with defer!");
}

void my_fclose(FILE *fp) {
	puts("fclose in defer!");
	fclose(fp);
}

void print_msgs_defer(const char *m1, const char *m2, const char *m3)
{
	puts(m1);
	puts(m2);
	puts(m3);
}

int main(void) {
	defer_ctx_t ctx;
	memset(&ctx, 0, sizeof(ctx));

	FILE *fp = fopen("main.c", "rb");
	if (!fp) {
		perror("fopen");
		return 1;
	}
	// add `my_fclose` to defer context stack
	defer_add(&ctx, my_fclose, (void*[ARGS_LIMIT]){ fp }, 1);
	//              ^^^^^^^^^   ^^^^^^^^^^^^^^^^^^^^^^^   ^
	//                   |                  |             |
	// function to call -+                  |             |
	// function arguments ------------------+             |
	// arguments count -----------------------------------+

	// add print_defer to defer context stack
	defer_add(&ctx, print_defer, NULL, 0);

	defer_add(&ctx, print_msgs_defer, (void*[ARGS_LIMIT]){"msg1", "msg2", "msg3"}, 3);

	// do some work
	puts("some random string to print!");

	// call all registered defers
	defer_do(&ctx);
	return 0;
}
