#include "../include/ft_ssl.h"

char	*allowed_command[] = {"md5", "sha256", // ft_ssl_md5
	"base64", "des", "des-ecb", "des-cbc", // ft_ssl_des
	"genrsa", "rsa", "rsautl" // ft_ssl_rsa
};

int (*command_functions[])() = {&ft_md5, &ft_sha256, // ft_ssl_md5
	&ft_base64, &ft_des, &ft_des_ecb, &ft_des_cbc,  // ft_ssl_des
	&ft_genrsa, &ft_rsa, &ft_rsault  // ft_ssl_rsa
};

int main (int ac, char **av){
	(void)av[ac - 1];
	
	if (ac <= 1){
		printf("ft_ssl: Error: Not enough arguments.\n\n");
		help();
		return 1;
	}
	
	for (int i = 0; allowed_command[i]; i++){
		if (!strcmp(av[ac - 1], allowed_command[i])){
			return (command_functions[i]());
		}
	}
	printf("ft_ssl: Error: %s is an invalid command.\n\n", av[ac - 1]);
	help();
	return 0;
}

void	help(){
	printf("Commands:\n\
md5\n\
sha256\n\n\
Flags:\n\
-p -q -r -s\n");
}