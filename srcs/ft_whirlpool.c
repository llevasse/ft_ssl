#include "../include/ft_whirlpool.h"

int ft_whirlpool(char *arg){
  (void) arg;
  return (0);
}


void whirlpool_init(WHIRLPOOL_CONTEXT *ctx){
  for (int i = 0; i < 8; i++)
    ctx->buffer[i] = 0;
}
