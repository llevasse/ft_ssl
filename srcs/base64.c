#include "../include/ft_ssl.h"

int ft_base64(){
	if (!FT_SSL_DES){
		printf("ft_ssl: Error: base64 is an invalid command.\n\n");
		help();
		return (1);
	}
	return 0;
}

int ft_des(){
	if (!FT_SSL_DES){
		printf("ft_ssl: Error: des is an invalid command.\n\n");
		help();
		return (1);
	}
	return 0;
}

int ft_des_ecb(){
	if (!FT_SSL_DES){
		printf("ft_ssl: Error: des-ecb is an invalid command.\n\n");
		help();
		return (1);
	}
	return 0;
}

int ft_des_cbc(){
	if (!FT_SSL_DES){
		printf("ft_ssl: Error: des-cbc is an invalid command.\n\n");
		help();
		return (1);
	}
	return 0;
}