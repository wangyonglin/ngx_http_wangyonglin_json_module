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

ngx_int_t wyl_json_body(ngx_http_request_t *r,char ** out,int error,const char * reason,cJSON* result){
	cJSON* root;
	char *datetime; 

	struct tm t;
	struct timeval tv;

    datetime = ngx_pcalloc(r->pool, sizeof(datetime));
    if (datetime == NULL) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "datetime ngx_pcalloc return null");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
	
	
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec,&t);
	sprintf(datetime,"%d-%d-%d %d:%d:%d.%03d",t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec,(int)(tv.tv_usec/1000));	
	
	root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "error_code", error);
	cJSON_AddStringToObject(root, "reason", reason);
	if(result == NULL)	cJSON_AddItemToObject(root, "result", cJSON_CreateObject());
	else	cJSON_AddItemToObject(root, "result", result);
	cJSON_AddStringToObject(root,"datetime",datetime);
	*out=cJSON_Print(root);
	cJSON_Delete(root);
	return NGX_OK;
}
