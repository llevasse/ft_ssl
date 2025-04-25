#include "../include/ft_ssl.h"
int OPTIONS = 0;
int ar_idx = 2;
int command_idx = 1;

char	*allowed_command[] = {"md5", "sha256", "whirlpool", // ft_ssl_md5
	"base64", "des", "des-ecb", "des-cbc", // ft_ssl_des
	"genrsa", "rsa", "rsautl", // ft_ssl_rsa
	0x0
};

int (*command_functions[])() = {&ft_md5, &ft_sha256, &ft_whirlpool, // ft_ssl_md5
	&ft_base64, &ft_des, &ft_des_ecb, &ft_des_cbc,  // ft_ssl_des
	&ft_genrsa, &ft_rsa, &ft_rsault  // ft_ssl_rsa
};

int main (int ac, char **av){
	if (ac <= 1){
    write(2, "usage: ft_ssl command [flags] [file/string]\n", 44);
		return 0;
	}
	
	parse_option(ac, av);
	for (int i = 0; i < NB_AVAILABLE_COMMAND; i++){
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
	write(2, "ft_ssl: Error: ", 15);
	write(2, av[command_idx], strlen(av[command_idx]));
	write(2, " is an invalid command.\n\n", 26);
	help();
	return 0;
}

void	help(){
	write(2, "Commands:\n", 11); 
  for (int i = 0; i < NB_AVAILABLE_COMMAND; i++){
    write(2, allowed_command[i], strlen(allowed_command[i])); 
    write(2, "\n", 1); 
  }
	write(2, "\n", 1); 
  write(2, "Flags:\n\
-p -q -r -s\n", 20);
}