#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wyl_json.h"

typedef struct {
    ngx_str_t wangyonglin_string;
    ngx_int_t wangyonglin_counter;
} ngx_http_wangyonglin_json_loc_conf_t;

static ngx_int_t ngx_http_wangyonglin_json_init(ngx_conf_t *cf);

static void *ngx_http_wangyonglin_json_create_loc_conf(ngx_conf_t *cf);

static char *ngx_http_wangyonglin_json_string(ngx_conf_t *cf,
                                   ngx_command_t *cmd,
                                   void *conf);
static char *ngx_http_wangyonglin_json_counter(ngx_conf_t *cf,
                                    ngx_command_t *cmd,
                                    void *conf);

static ngx_command_t ngx_http_wangyonglin_json_commands[] = {
    {ngx_string("wangyonglin_string"),
     NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS | NGX_CONF_TAKE1,
     ngx_http_wangyonglin_json_string, NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_wangyonglin_json_loc_conf_t, wangyonglin_string), NULL},
    {ngx_string("wangyonglin_counter"), NGX_HTTP_LOC_CONF | NGX_CONF_FLAG,
     ngx_http_wangyonglin_json_counter, NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_wangyonglin_json_loc_conf_t, wangyonglin_counter), NULL},
    ngx_null_command};

/*
static u_char ngx_wangyonglin_default_string[] = "Default String: wangyonglin, world!";
*/
static int ngx_wangyonglin_visited_times = 0;

static ngx_http_module_t ngx_http_wangyonglin_json_module_ctx = {
    NULL,                /* preconfiguration */
    ngx_http_wangyonglin_json_init, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    ngx_http_wangyonglin_json_create_loc_conf, /* create location configuration */
    NULL                            /* merge location configuration */
};

ngx_module_t ngx_http_wangyonglin_json_module = {
    NGX_MODULE_V1,
    &ngx_http_wangyonglin_json_module_ctx, /* module context */
    ngx_http_wangyonglin_json_commands,    /* module directives */
    NGX_HTTP_MODULE,            /* module type */
    NULL,                       /* init master */
    NULL,                       /* init module */
    NULL,                       /* init process */
    NULL,                       /* init thread */
    NULL,                       /* exit thread */
    NULL,                       /* exit process */
    NULL,                       /* exit master */
    NGX_MODULE_V1_PADDING};

static ngx_int_t ngx_http_wangyonglin_json_handler(ngx_http_request_t *r)
{
    ngx_int_t rc;
    ngx_buf_t *b;
    ngx_chain_t out;
    ngx_http_wangyonglin_json_loc_conf_t *my_conf;
    u_char ngx_wangyonglin_string[1024] = {0};
    ngx_uint_t content_length = 0;
	char *jsonbuf;
	 
    ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0,
                  "ngx_http_wangyonglin_json_handler is called!");

    my_conf = ngx_http_get_module_loc_conf(r, ngx_http_wangyonglin_json_module);
    if (my_conf->wangyonglin_string.len == 0) {
        ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0,
                      "wangyonglin_string is empty!");
        return NGX_ERROR;
    }

    if (my_conf->wangyonglin_counter == NGX_CONF_UNSET ||
        my_conf->wangyonglin_counter == 0) {
        ngx_sprintf(ngx_wangyonglin_string, "%s", my_conf->wangyonglin_string.data);
    } else {
       ngx_sprintf(ngx_wangyonglin_string, "%s Visited Times:%d",my_conf->wangyonglin_string.data, ++ngx_wangyonglin_visited_times);
		
		// 申请响应正文内存
    jsonbuf = ngx_pcalloc(r->pool, sizeof(jsonbuf));
    if (jsonbuf == NULL) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "ngx_pcalloc return null");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

		
		
		wyl_json_body(r,&jsonbuf,++ngx_wangyonglin_visited_times,"ok",NULL);
		ngx_memcpy(ngx_wangyonglin_string,jsonbuf,ngx_strlen(jsonbuf));
    }
    ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "wangyonglin_string:%s",
                  ngx_wangyonglin_string);
    content_length = ngx_strlen(ngx_wangyonglin_string);

    /* we response to 'GET' and 'HEAD' requests only */
    if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    /* discard request body, since we don't need it here */
    rc = ngx_http_discard_request_body(r);

    if (rc != NGX_OK) {
        return rc;
    }

    /* set the 'Content-type' header */
    /*
     *r->headers_out.content_type.len = sizeof("text/html") - 1;
     *r->headers_out.content_type.data = (u_char *)"text/html";
     */
    ngx_str_set(&r->headers_out.content_type, "application/json");

    /* send the header only, if the request type is http 'HEAD' */
    if (r->method == NGX_HTTP_HEAD) {
        r->headers_out.status = NGX_HTTP_OK;
        r->headers_out.content_length_n = content_length;

        return ngx_http_send_header(r);
    }

    /* allocate a buffer for your response body */
    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    /* attach this buffer to the buffer chain */
    out.buf = b;
    out.next = NULL;

    /* adjust the pointers of the buffer */
    b->pos = ngx_wangyonglin_string;
    b->last = ngx_wangyonglin_string + content_length;
    b->memory = 1;   /* this buffer is in memory */
    b->last_buf = 1; /* this is the last buffer in the buffer chain */

    /* set the status line */
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = content_length;

    /* send the headers of your response */
    rc = ngx_http_send_header(r);

    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }

    /* send the buffer chain of your response */
    return ngx_http_output_filter(r, &out);
}

static void *ngx_http_wangyonglin_json_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_wangyonglin_json_loc_conf_t *local_conf = NULL;
    local_conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_wangyonglin_json_loc_conf_t));
    if (local_conf == NULL) {
        return NULL;
    }

    ngx_str_null(&local_conf->wangyonglin_string);
    local_conf->wangyonglin_counter = NGX_CONF_UNSET;

    return local_conf;
}

static char *ngx_http_wangyonglin_json_string(ngx_conf_t *cf,
                                   ngx_command_t *cmd,
                                   void *conf)
{
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_wangyonglin_json_handler;

    char *rv = ngx_conf_set_str_slot(cf, cmd, conf);

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "wangyonglin_string:%s",
                       ((ngx_http_wangyonglin_json_loc_conf_t *) conf)->wangyonglin_string.data);

    return rv;
}

static char *ngx_http_wangyonglin_json_counter(ngx_conf_t *cf,
                                    ngx_command_t *cmd,
                                    void *conf)
{
    char *rv = ngx_conf_set_flag_slot(cf, cmd, conf);
    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "wangyonglin_counter:%d",
                       ((ngx_http_wangyonglin_json_loc_conf_t *) conf)->wangyonglin_counter);
    return rv;
}

static ngx_int_t ngx_http_wangyonglin_json_init(ngx_conf_t *cf)
{
    return NGX_OK;
}
