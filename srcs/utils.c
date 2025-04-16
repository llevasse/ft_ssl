#include "../include/ft_ssl.h"

#define INPUT_BUF_LEN 512
extern int OPTIONS;



FT_FILE *read_file(char *path, FT_FILE *file){
	char *input = 0x0, *tmp;
	int fd = open(path, O_RDONLY);
	
	if (fd < 0){
		fprintf(stderr, "Error opening input file : %s\n", strerror(errno));
		return (0x0);
	}
  char buf[INPUT_BUF_LEN];
  size_t len;
  size_t size = 0;
  while ((len = read(fd, buf, INPUT_BUF_LEN)) > 0){
    tmp = realloc(input, (size + len + 1) * sizeof(char));
    if (!tmp){
      fprintf(stderr, "Error opening input file : %s\n", strerror(errno));
      free(input);
      close(fd);
      return (0x0);
    }
    input = tmp;
    memcpy(input + size, buf, len);
    input[size + len] = 0;
    size += len;
  }
	close(fd);
	file->content = input;
	file->size = size;
	return (file);
}

FT_FILE *read_stdin(FT_FILE *file){
	char *input = 0x0, *tmp = 0x0;
	size_t size = 0;
	ssize_t len;
	int first = 1;
	char buf[INPUT_BUF_LEN];
	while ((len = read(STDIN_FILENO, buf, INPUT_BUF_LEN)) > 0){
    tmp = realloc(input, (size + len + 1) * sizeof(char));
		if (!tmp){
			free(input);
			return (0x0);
		}
		input = tmp;
		if (first){
      input[0] = 0;
			first = 0;
		}
    memcpy(input + size, buf, len);
    size += len;
		buf[0] = 0;
	}
	file->content = input;
	file->size = size ;
	return (file);
}

FT_FILE *get_input(char *arg, char *name){
  FT_FILE *f = malloc(sizeof(*f));
  if (!f)
    return 0x0;
	
	if (arg){
		if (OPTIONS & OPT_STRING){
			f->content = arg;
			f->size = strlen(arg);
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
        printf("(\"%s\")= ", f->content);
      else
        printf("%s(stdin)= ", name);
		}
	}
	return f;
}