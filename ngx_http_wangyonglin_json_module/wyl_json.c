#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_string.h>
#include "cJSON.h"
#include <time.h>
#include <sys/time.h>
#include "wyl_json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
cJSON* ngx_wangyonglin_result(char * ip,int visited){
	cJSON * root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "ip", ip);
	cJSON_AddNumberToObject(root,"visited",visited);
	return root;
}
ngx_int_t ngx_wangyonglin_response(ngx_pool_t *pool,cJSON* in,u_char *out){
	/**声明区**/

	cJSON* root;
	char *datetime; 
	struct tm t;
	struct timeval tv;
	char * build;
	
	/**内存区**/
    datetime = ngx_pcalloc(pool, sizeof(datetime));
    if (datetime == NULL) {
        return NGX_ERROR;
    }
	
	build = ngx_pcalloc(pool, sizeof(build));
    if (build == NULL) {
        return NGX_ERROR;
    }
	
	/**主体区**/
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec,&t);
	sprintf(datetime,"%d-%d-%d %d:%d:%d.%03d",t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec,(int)(tv.tv_usec/1000));	
	
	root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "error_code", 200);
	cJSON_AddStringToObject(root, "reason", "OK");
	if(in == NULL)	cJSON_AddItemToObject(root, "result", cJSON_CreateObject());
	else	cJSON_AddItemToObject(root, "result", in);
	cJSON_AddStringToObject(root,"datetime",datetime);
	build=cJSON_Print(root);
	cJSON_Delete(root);	
	ngx_memcpy(out,build,ngx_strlen(build));
	return strlen(build);
}
