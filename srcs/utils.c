#include "../include/ft_ssl.h"

#define INPUT_BUF_LEN 512
extern int OPTIONS;



FT_FILE *read_file(char *path, FT_FILE *file){
	char *input = 0x0, *tmp;
	FILE *f = 0x0;
	if (path)
		f = fopen(path, "r");
	
	if (!f){
		fprintf(stderr, "Error opening input file : %s\n", strerror(errno));
		return (0x0);
	}
  char buf[INPUT_BUF_LEN];
  size_t len;
  size_t size = 0;
  while ((len = fread(buf, sizeof(char), INPUT_BUF_LEN, f))){
    tmp = realloc(input, (size + len + 1) * sizeof(char));
    if (!tmp){
      fprintf(stderr, "Error opening input file : %s\n", strerror(errno));
      free(input);
      fclose(f);
      return (0x0);
    }
    input = tmp;
    memcpy(input + size, buf, len);
    input[size + len] = 0;
    size += len;
  }
		if (ferror(f) != 0) {
			fprintf(stderr, "Error reading input file : %s\n", strerror(errno));
			fclose(f);
			free(input);
			return (0x0);
		}
	fclose(f);
	file->content = input;
	file->size = size;
	return (file);
}

FT_FILE *read_stdin(FT_FILE *file){
	char *input = 0x0, *tmp = 0x0;
	size_t len = 1;
	int first = 1;
	char buf[INPUT_BUF_LEN];
	while (fgets(buf, INPUT_BUF_LEN, stdin) != 0x0){
		len += strlen(buf);
		tmp = realloc(input, len * sizeof(char));
		if (!tmp){
			free(input);
			return (0x0);
		}
		input = tmp;
		if (first){
			input[0] = 0;
			first = 0;
		}
		strcat(input, buf);
		buf[0] = 0;
	}
	file->content = input;
	file->size = len - 1;
	return (file);
}

FT_FILE *get_input(char *arg, char *name){
  FT_FILE *f = malloc(sizeof(*f));
  if (!f)
    return 0x0;
	char *input = 0x0;
	
	if (arg){
		if (OPTIONS & OPT_STRING){
			f->content = arg;
		}
		else if ((read_file(arg, f)) == 0x0){
			free(f);
			return (0x0);
		}
		if (!(OPTIONS & OPT_REVERSE) && !(OPTIONS & OPT_QUIET))
		  printf("%s(%s)= ", name, arg);
	}
	else{
		if ((read_stdin(f)) == 0x0){
			free(f);
			return (0x0);
		}
		if (!(OPTIONS & OPT_QUIET)){
      if (OPTIONS & OPT_P)
        printf("(\"%s\")= ", input);
      else
        printf("%s(stdin)= ", name);
		}
	}
	return f;
}