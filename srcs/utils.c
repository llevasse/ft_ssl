#include "../include/ft_ssl.h"

#define INPUT_BUF_LEN 512
extern int OPTIONS;
extern int ar_idx;
extern int command_idx;



FT_FILE *read_file(char *path, FT_FILE *file){
	char *input = 0x0, *tmp;
	int fd = open(path, O_RDONLY);
	
	if (fd < 0){
		fprintf(stderr, "Error opening input file '%s' : %s\n", path, strerror(errno));
		return (0x0);
	}
  char buf[INPUT_BUF_LEN];
  size_t len;
  size_t size = 0;
  while ((len = read(fd, buf, INPUT_BUF_LEN)) > 0){
    tmp = realloc(input, (size + len + 1) * sizeof(char));
    if (!tmp){
      fprintf(stderr, "Error opening input file '%s' : %s\n", path, strerror(errno));
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
		if (!(OPTIONS & OPT_QUIET)){
      if ((OPTIONS & OPT_STRING))
        printf("%s(\"%s\")= ", name, arg);
      else if (!(OPTIONS & OPT_REVERSE))
        printf("%s(%s)= ", name, arg);
		}
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

void pass_arg_to_front(int idx, int ac, char **ar){
  if (idx <= 1 || idx > ac)
    return;
  char *tmp = ar[idx];
  ar[idx] = ar[idx - 1];
  ar[idx - 1] = tmp;
  pass_arg_to_front(idx - 1, ac, ar);  
}

void parse_option(int ac, char **ar){
  if (ar_idx > ac)
    return;
  for (;ar_idx < ac; ar_idx++){
    if (ar[ar_idx][0] == '-'){
      char c = ar[ar_idx][1];
      switch (c){
        case 's':
          OPTIONS |= OPT_STRING;
          pass_arg_to_front(ar_idx, ac, ar);
          command_idx++;   
          ar_idx++;
          if (ar[ar_idx + 1][0] && ar[ar_idx + 1][0] != '-')
            return;
          continue;
        case 'q':
          OPTIONS |= OPT_QUIET;
          continue;
        case 'p':
          OPTIONS |= OPT_P;
          continue;	
        case 'r':
          OPTIONS |= OPT_REVERSE;
          continue;
        default :
          break;
      }
    }
  }
}





