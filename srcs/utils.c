#include "../include/ft_ssl.h"

#define INPUT_BUF_LEN 512
extern int OPTIONS;
extern int ar_idx;
extern int command_idx;

FT_FILE *read_file(char *path, FT_FILE *file){
	char *input = 0x0, *tmp;
  int fd = 0;
  if ((fd = open(path, __O_DIRECTORY)) >= 0){
    close(fd);
    write(2, "Error opening input path : path is a directory\n", 47);
    return (0x0);
  }

	fd = open(path, O_RDONLY);
	
	if (fd < 0){
    write(2, "Error opening input file '", 26);
    write(2, path, strlen(path));
    write(2, "' : ", 4);
    write(2, strerror(errno), strlen(strerror(errno)));
    write(2, "\n", 1);
		return (0x0);
	}
  char buf[INPUT_BUF_LEN];
  size_t len;
  size_t size = 0;
  while ((len = read(fd, buf, INPUT_BUF_LEN)) > 0){
    tmp = realloc(input, (size + len + 1) * sizeof(char));
    if (!tmp){
      write(2, "Error opening input file '", 26);
      write(2, path, strlen(path));
      write(2, "' : ", 4);
      write(2, strerror(errno), strlen(strerror(errno)));
      write(2, "\n", 1);
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
      write(2, "Error reading STDIN : ", 22);
      write(2, strerror(errno), strlen(strerror(errno)));
      write(2, "\n", 1);
			free(input);
			return (0x0);
		}
    memset(tmp + size, 0, len + 1);
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


/**
 * Get hashing input from STDIN, file, or string, and print hashing 'introduction'
 *
 * @param arg  input argument (filename, input string, or 0x0 if read from STDIN)
 * @param name name of the hashing algorithm to use.
*/
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
      if (!(OPTIONS & OPT_REVERSE)){
        write(1, name, strlen(name));
        write(1, " (", 2);
        if ((OPTIONS & OPT_STRING)){
          write(1, "\"", 1);
          write(1, arg, strlen(arg));
          write(1, "\"", 1);
        }
        else
          write(1, arg, strlen(arg));
        write(1, ")= ", 3);
      }
		}
	}
	else{
		if ((read_stdin(f)) == 0x0){
			free(f);
			return (0x0);
		}
		if (!(OPTIONS & OPT_QUIET)){
      if (OPTIONS & OPT_P){
        write(1, "(\"", 2);
        write(1, f->content, strlen(f->content));
        write(1, "\")= ", 4);
      }
      else
        write(1, "(stdin)= ", 9);
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

/**
 * Parse command options and reorganise arguments order to have the options at the front of the array
 * update ar_idx to position of first non-option and non-command argument, and update command_idx to command argument position
 *
 * @param ac  number of arguments
 * @param ar  array of arguments.
*/
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
          if (!ar[ar_idx + 1] || ar[ar_idx + 1][0] != '-')
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
    else
      break;
  }
}

void	ft_putnbr_base(uint64_t n, const char *base, uint64_t min_len){
  if (min_len != 0){
    uint64_t n_len = 0, n_copy = n;
    size_t base_len = strlen(base);
    if (n_copy == 0)
      n_len++;
    while (n_copy > 0){
      n_len++;
      n_copy /= base_len;
    }
    uint64_t i = min_len - n_len;
    while (i-- > 0){
      write(1, "0", 1);
    }
  }
	if ((size_t)n < strlen(base))
	  write(1, &base[n], 1);
	else
	{
		ft_putnbr_base(n / strlen(base), base, 0);
	  write(1, &base[n % strlen(base)], 1);		
	}
}

/**
 * Print hash's context buffer as it's digest
 *
 * @param name    input name (filename, input string, or 0x0 if from STDIN)
 * @param digest  pointer to context buffer/digest array.
 * @param type    *digest type (UINT8 | UINT32 | UINT64) (UINT64 by default)
 * @param digest_arr_length length of digest array
 * @param padding_length      minimum length of printed digest element
*/
void  print_digest(char *name, void *digest, int type, size_t digest_arr_length, size_t padding_length){
  
  if (type == UINT8){
    uint8_t *d = (uint8_t *)digest;
    for (uint32_t i = 0; i < digest_arr_length; i++){
      ft_putnbr_base(d[i], "0123456789abcdef", padding_length);
    }
  }
  else if (type == UINT32){
    uint32_t *d = (uint32_t *)digest;
    for (uint32_t i = 0; i < digest_arr_length; i++){
      ft_putnbr_base(d[i], "0123456789abcdef", padding_length);
    }
  }
  else{
    uint64_t *d = (uint64_t *)digest;
    for (uint32_t i = 0; i < digest_arr_length; i++){
      ft_putnbr_base(d[i], "0123456789abcdef", padding_length);
    }
  }
  if (name){
    if (OPTIONS & OPT_REVERSE && !(OPTIONS & OPT_QUIET)){
      if ((OPTIONS & OPT_STRING)){
        write(1, " \"", 2);
        write(1, name, strlen(name));
        write(1, "\"", 1);
      }
      else{
        write(1, " ", 1);
        write(1, name, strlen(name));
      }
    }
  }
	write(1, "\n", 1);
}

