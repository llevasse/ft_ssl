#include "../include/ft_ssl.h"

int ft_genrsa(char *input_path){
	(void)input_path;
	if (!FT_SSL_RSA){
		printf("ft_ssl: Error: genrsa is an invalid command.\n\n");
		help();
		return (1);
	}
	return 0;
}

int ft_rsa(char *input_path){
	(void)input_path;
	if (!FT_SSL_RSA){
		printf("ft_ssl: Error: rsa is an invalid command.\n\n");
		help();
		return (1);
	}
	return 0;
}

int ft_rsault(char *input_path){
	(void)input_path;
	if (!FT_SSL_RSA){
		printf("ft_ssl: Error: rsault is an invalid command.\n\n");
		help();
		return (1);
	}
	return 0;
}