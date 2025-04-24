#ifndef FT_WHIRLPOOL_H
# define FT_WHIRLPOOL_H
	#include "ft_ssl.h"
 	
	typedef struct whirlpool_context {
		uint64_t buffer[8];
		uint8_t input[64];
		uint8_t digest[32];
		uint64_t size;
 } WHIRLPOOL_CONTEXT;
 
void whirlpool_init(WHIRLPOOL_CONTEXT *ctx);
void whirlpool_process(WHIRLPOOL_CONTEXT *ctx, uint8_t *input, size_t N);
 
#endif
