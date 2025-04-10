#ifndef FT_SSL_H
# define FT_SSL_H

# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <unistd.h>
# include <stdlib.h>
# include <math.h>

# ifndef FT_SSL_MD5
#  define FT_SSL_MD5 0
# endif

# ifndef FT_SSL_DES
#  define FT_SSL_DES 0
# endif

# ifndef FT_SSL_RSA
#  define FT_SSL_RSA 0
# endif

int ft_md5(char *input_path);
int ft_sha256(char *input_path);
int ft_base64(char *input_path);
int ft_des(char *input_path);
int ft_des_ecb(char *input_path);
int ft_des_cbc(char *input_path);
int ft_genrsa(char *input_path);
int ft_rsa(char *input_path);
int ft_rsault(char *input_path);

char *read_file(char *path);
char *read_stdin();


void	help();
#endif