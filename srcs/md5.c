#include "../include/ft_ssl.h"

int ft_md5(char *input_path){
	char *input = 0x0;
	if (input_path){
		int fd = open(input_path, O_RDONLY);
		if (fd < 0){
			fprintf(stderr, "Error opening input file : %s\n", strerror(errno));
			return (1);
		}
		close(fd);
	}
	else{
	
	}
	(void)input;
	(void)input_path;
	printf("md5\n");
	return (0);
}