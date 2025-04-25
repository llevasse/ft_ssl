#ifndef FT_SSL_H
# define FT_SSL_H

# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <unistd.h>
# include <stdlib.h>
# include <math.h>
# include <getopt.h>

# ifndef FT_SSL_MD5
#  define FT_SSL_MD5 0
# endif

# ifndef FT_SSL_DES
#  define FT_SSL_DES 0
# endif

# ifndef FT_SSL_RSA
#  define FT_SSL_RSA 0
# endif

typedef struct ft_file {
  char *content;
  size_t size;
} FT_FILE;

#ifndef size_t
	typedef unsigned long size_t;
#endif

typedef	__uint32_t uint32_t;
typedef	__uint64_t uint64_t;
typedef	unsigned char uint8_t;

#define UINT8 8
#define UINT32 32
#define UINT64 64

void parse_option(int ac, char **ar);
void ft_putnbr_base(uint64_t n, const char *base, uint64_t min_len);
void  print_digest(char *name, void *digest, int type, size_t digest_arr_length, size_t padding_length);


int ft_md5(char *arg);
int ft_sha256(char *arg);
int ft_whirlpool(char *arg);
int ft_base64(char *input_path);
int ft_des(char *input_path);
int ft_des_ecb(char *input_path);
int ft_des_cbc(char *input_path);
int ft_genrsa(char *input_path);
int ft_rsa(char *input_path);
int ft_rsault(char *input_path);

FT_FILE *get_input(char *arg, char *name);
FT_FILE *read_file(char *path, FT_FILE *file);
FT_FILE *read_stdin(FT_FILE *file);


void	help();

#define OPT_STRING 1
#define OPT_P 2
#define OPT_QUIET 4
#define OPT_REVERSE 8

#endif