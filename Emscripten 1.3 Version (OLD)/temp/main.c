#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#include "ceu_types.h"

s32 WCLOCK_nxt;
#define ceu_out_wclock_set(us) WCLOCK_nxt = us;
#define ceu_out_assert(X) assert(X)
#define ceu_out_log(X) printf("%s\n", X)

// This is the C code generated by the Ceu compiler
#include "_ceu_app.c"


static byte CEU_DATA[sizeof(CEU_Main)];
static tceu_app app;


void async_call() {
	ceu_sys_go(&app, CEU_IN__ASYNC, NULL);
	#ifdef CEU_RET
	if (! app.isAlive) {
		printf("%d\n", app.ret);
	}
	#endif
}

void update(s32 time) {

	s32 dt_us = time;
	ceu_sys_go(&app, CEU_IN__WCLOCK, &dt_us);


	s32 prev = WCLOCK_nxt;
	while (WCLOCK_nxt <= 0) {
		s32 dt_us = 0;
		ceu_sys_go(&app, CEU_IN__WCLOCK, &dt_us);

		//When there are no timed events
		if (WCLOCK_nxt == prev) {
			WCLOCK_nxt = 1;

		}
	}

}

void begin() {
	memset(CEU_DATA, 0, sizeof(CEU_Main));
	app.data = (tceu_org*) &CEU_DATA;
	app.init = &ceu_app_init;
	app.init(&app);
}
