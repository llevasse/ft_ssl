#include "../include/ft_ssl.h"
int OPTIONS = 0;
int ar_idx = 2;
int command_idx = 1;

char	*allowed_command[] = {"md5", "sha256", // ft_ssl_md5
	"base64", "des", "des-ecb", "des-cbc", // ft_ssl_des
	"genrsa", "rsa", "rsautl", // ft_ssl_rsa
	0x0
};

int (*command_functions[])() = {&ft_md5, &ft_sha256, // ft_ssl_md5
	&ft_base64, &ft_des, &ft_des_ecb, &ft_des_cbc,  // ft_ssl_des
	&ft_genrsa, &ft_rsa, &ft_rsault  // ft_ssl_rsa
};

int main (int ac, char **av){
	if (ac <= 1){
		printf("usage: ft_ssl command [flags] [file/string]\n");
		return 1;
	}
	
	parse_option(ac, av);
	for (int i = 0; allowed_command[i]; i++){
		if (!strcmp(av[command_idx], allowed_command[i])){
      if (OPTIONS & OPT_P)
        command_functions[i](0x0);
      if (ar_idx < ac){
        while (ar_idx < ac){
          command_functions[i](av[ar_idx++]);
          if (OPTIONS & OPT_STRING)
            OPTIONS ^= OPT_STRING;
        }
      }
      else{
        command_functions[i](0x0);
      }
      return 0;
		}
	}
	printf("ft_ssl: Error: %s is an invalid command.\n\n", av[1]);
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