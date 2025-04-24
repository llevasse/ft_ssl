#include "../include/ft_whirlpool.h"

extern int OPTIONS;

int ft_whirlpool(char *arg){
	FT_FILE *f = get_input(arg, "WHIRLPOOL");
	if (!f)
		return 1;
    
	WHIRLPOOL_CONTEXT ctx;
	whirlpool_init(&ctx);
	whirlpool_process(&ctx, (uint8_t *)f->content, f->size);
	// whirlpool_finalize(&ctx);
	for (uint32_t i = 0; i < 8; i++){
    ft_putnbr_base(ctx.buffer[i], "0123456789abcdef", 8);
	}
	write(1, "\n", 1);
	if ((arg && !(OPTIONS & OPT_STRING)) || !arg)
	free(f->content);
	free(f);
	return (0);
}


void whirlpool_init(WHIRLPOOL_CONTEXT *ctx){
  for (int i = 0; i < 8; i++)
    ctx->buffer[i] = 0;
}

void whirlpool_process(WHIRLPOOL_CONTEXT *ctx, uint8_t *input, size_t N){
  uint64_t source_bit_len = N * sizeof(uint8_t);
  uint64_t block[8];

  for (int i = 0; i < N)

  (void)ctx;
  (void)input;
}