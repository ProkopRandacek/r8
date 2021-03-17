/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "sceneio.h"
#include "debug.h"

regex_t floatRegex;
regex_t intRegex;
int regexStatus;
char msgbuf[100];

int isShape(const char* word) {
	return (strcmp(word, "SPHERE") ||
		strcmp(word, "TORUS") ||
		strcmp(word, "CCONE") );
}

void readScene() {
	const char* filename = "test.scene";

	regexStatus = regcomp(&floatRegex, "^-?[0-9]+.[0-9]+$", REG_EXTENDED);
	if (regexStatus) {
		fprintf(stderr, "Could not compile regex\n");
		exit(1);
	}

	regexStatus = regcomp(&intRegex, "^-?[0-9]+$", REG_EXTENDED);
	if (regexStatus) {
		fprintf(stderr, "Could not compile regex\n");
		exit(1);
	}

	FILE* fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Error opening file \"%s\"\n", filename);
		exit(1);
	}

	const unsigned int wmaxlen = 30;
	char word[wmaxlen];
	memset(word, 0, wmaxlen);
	char c;
	unsigned int wlen = 0;
	char activeKey = ' ';
	//char* sTypes; // types of shapes in array TODO
	int a = -1;
	int b = -1;
	float* shapes;

	int prmvNum = -1;

	dprint("scene file parsing start");

	while ((c = (char)fgetc(fp)) != EOF) { // read file byte by byte until EOF
		if (wlen >= wmaxlen) {
			char l[103];
			sprintf(l, "error parsing file \"%s\"\n the word \"%s...\" is too long (max is %d)\n", filename, word, wmaxlen);
			eprint(l);
		}
		if (c == ' ' || c == '\n') {
			if (wlen != 0) {
				if (word[0] == '/' && word[1] == '/') { // if word == "//"
					while ((c = (char)fgetc(fp)) != '\n') { } // skip until end of line
				} else {
					int floatR = regexec(&floatRegex, word, 0, NULL, 0);
					int intR = regexec(&intRegex, word, 0, NULL, 0);

					if (!floatR) { // its float
						if (activeKey == 'S') {
							shapes[a * 15 + b] = (float)atof(word);
							b++;
						}
					} else if (!intR) { // its int
						if (activeKey == 'P') {
							prmvNum = atoi(word);
							shapes = malloc(  (long unsigned int)(prmvNum * 15) * sizeof(float));
							memset(shapes, 0, (long unsigned int)(prmvNum * 15) * sizeof(float));
						}
					} else if (floatR == REG_NOMATCH || intR == REG_NOMATCH) { // or keyword
						if (strcmp(word, "#PRIMITIVES") == 0) { activeKey = 'P'; }
						else {
							activeKey = 'S';
							a++;
							b = 0;
							if (strcmp(word, "SPHERE") == 0) {}
						}
					} else {
						regerror(regexStatus, &floatRegex, msgbuf, sizeof(msgbuf));
						printf("Regex match failed: %s\n", msgbuf);
						exit(1);
					}
				}
				wlen = 0;
				memset(word, 0, wmaxlen);
			}
			continue;
		}
		word[wlen++] = c;
	}

	regfree(&floatRegex);
	fclose(fp);
	dprint("scene file parsing done");

	printf("\nprmvNum: %d\n", prmvNum);
	for (int i = 0; i < prmvNum; i++) {
		for (int j = 0; j < 15; j++) {
			printf(" %.1f", shapes[i * 15 + j]);
		}
		printf("\n");
	}
	printf("\n");
}
*/
