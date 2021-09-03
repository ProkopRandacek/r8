// embeds a binary file into a header file char array
#include <stdio.h>
#include <stdlib.h>

void replace(char* string, char from, char to) {
	int pos = 0;
	while (string[pos] != 0) {
		if (string[pos] == from)
			string[pos] = to;
		pos++;
	}
}

int main(int argc, char* argv[]) {
	FILE *fi, *fo;
	long size;

	if (argc != 4)
		printf("usage: ./biem input-binary-file input-binary-file-basename output-header-file\n"),exit(1);

	char* filename_in  = argv[1];
	char* basename     = argv[2];
	char* filename_out = argv[3];

	fi = fopen(filename_in , "rb");
	fo = fopen(filename_out, "w");

	// get file size
	fseek(fi, 0, SEEK_END);
	size = ftell(fi);
	rewind(fi);

	replace(basename, '.', '_');

	// header start
	fprintf(fo, "#ifndef %s_H\n", basename);
	fprintf(fo, "#define %s_H\n", basename);
	fprintf(fo, "const unsigned char %s_bytes[] = {", basename);

	// char array values
	for (int i = 0; i < size - 1; i++)
		fprintf(fo, "%d,", fgetc(fi));
	fprintf(fo, "%d", fgetc(fi));
	fprintf(fo, "};\n");

	// char array size
	fprintf(fo, "const int %s_size = %ld;\n", basename, size);

	// header end
	fprintf(fo, "#endif");

	fclose(fi);
	fclose(fo);
	return 0;
}

