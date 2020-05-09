#ifndef __WYL_JSON_H__
#define __WYL_JSON_H__

#include <stdio.h>
#include "cJSON.h"

#ifdef __cplusplus
extern "C"{
#endif
	extern	cJSON* ngx_wangyonglin_result(char * ip,int visited);
	extern	ngx_int_t ngx_wangyonglin_response(ngx_pool_t *pool,cJSON* in,u_char *out);
	
#ifdef __cplusplus
}
#endif
#endif