#include "../include/ft_ssl.h"

int ft_md5(char *input_path){
	char *input = 0x0;
	
	if (input_path){
		if ((input = read_file(input_path)) == 0x0)
			return (1);
		printf("MD5(%s)= ", input_path);
	}
	else
		printf("MD5(stdin)= ");	
	printf("%s", input);
	free(input);
	return (0);
}