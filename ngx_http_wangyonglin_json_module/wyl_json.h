#ifndef __WYL_JSON_H__
#define __WYL_JSON_H__

#include <stdio.h>
#include "cJSON.h"

#ifdef __cplusplus
extern "C"{
#endif

	extern	ngx_int_t wyl_json_body(ngx_http_request_t *r,char ** out,int error,const char * reason,cJSON* result);
	
#ifdef __cplusplus
}
#endif
#endif