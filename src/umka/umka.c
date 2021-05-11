#include <stdio.h>
#include <stddef.h>

#include "../../submodules/umka/src/umka_api.h"
#include "bindings.h"
#include "umka.h"
#include "../debug.h"

void* umka;
int umkaUpdateFunc, umkaKeyDown, umkaKeyUp;

void initUmka() {
	dprint("UMKA START");

	umka = umkaAlloc();
	int umkaOK;

	umkaOK = umkaInit(umka, "scripts/game.um", NULL, 1024 * 1024, 1024 * 1024, 0, NULL);
	if (!umkaOK) { eprint("umka init failed"); }
	else         { dprint("UMKA - init successful"); }

	umkaBind(umka);
	dprint("UMKA - bind done");

	umkaOK = umkaCompile(umka);
	if (!umkaOK) {
		UmkaError error;
		umkaGetError(umka, &error);
		char msg[1072];
		sprintf(msg, "Umka compile error %s (%d, %d): %s\n", error.fileName, error.line, error.pos, error.msg);
		eprint(msg);
	}
	dprint("UMKA - compiled successfully");

	umkaUpdateFunc = umkaGetFunc(umka, NULL, "update");
	umkaKeyDown    = umkaGetFunc(umka, NULL, "keyDown");
	umkaKeyUp      = umkaGetFunc(umka, NULL, "keyUp");

	dprint("UMKA DONE");
}

void exitUmka() {
	umkaFree(umka);
}
