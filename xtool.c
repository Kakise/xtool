#include <os.h>

#define SRC_DIR "\\documents\\MyLib\\"
#define DST_DIR "\\exammode\\usr\\Press-to-Test\\"

#define BUF_SIZE 256

#define PATCHED_VERSION " 3.2.3.1233"

static unsigned v_addrs[] = {0x107C1DDC, 0x10801E3C, 0x107A2DC8, 0x107FF0D0};
#define v_addr ((char **)nl_osvalue((int *)v_addrs, 4))

int is_ptt(void) {
	char buffer[BUF_SIZE] = {'\0'};
	NU_Current_Dir("A:", buffer);
	return strcmp(buffer, "\\documents\\");
}

int copy_file(const char *src, const char *dst) {
	FILE *in = fopen(src, "rb"), *out;
	int c;
	if (!in)
		return -1;
	out = fopen(dst, "wb");
	if (!out) {
		fclose(in);
		return -1;
	}
	while ((c = fgetc(in)) != EOF)
		fputc(c, out);
	fclose(in);
	fclose(out);
	return 0;
}

void copy_files(void) {
	struct dstat file;
	NU_Get_First(&file, SRC_DIR "*.*");
	do {
		char src[BUF_SIZE] = {'\0'};
		char dst[BUF_SIZE] = {'\0'};
		sprintf(src, SRC_DIR "%s", file.filepath);
		sprintf(dst, DST_DIR "%s", file.filepath);
		copy_file(src, dst);
	} while (!NU_Get_Next(&file));
	NU_Done(&file);
}

void patch_version(void) {
	char *s = malloc(16 * sizeof *s);
	strcpy(s, PATCHED_VERSION);
	*v_addr = s;
}

int main(int argc, char **argv) {
	if (is_ptt())
		copy_files();
	patch_version();
	return EXIT_SUCCESS;
}