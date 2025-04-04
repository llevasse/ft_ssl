#ifndef FT_SSL_H
# define FT_SSL_H

# include <stdio.h>
# include <string.h>

# ifndef FT_SSL_MD5
#  define FT_SSL_MD5 0
# endif

# ifndef FT_SSL_DES
#  define FT_SSL_DES 0
# endif

# ifndef FT_SSL_RSA
#  define FT_SSL_RSA 0
# endif

int ft_md5();
int ft_sha256();
int ft_base64();
int ft_des();
int ft_des_ecb();
int ft_des_cbc();
int ft_genrsa();
int ft_rsa();
int ft_rsault();

void	help();
#endif