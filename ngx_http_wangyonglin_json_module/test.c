#include <ngx_config.h>
#include <ngx_core.h>
#include<ngx_string.h>
int main(int args,char *argv[])
{
	u_char *p;
	ngx_str_t str = ngx_null_string;
	unsigned char buffer[1024];
	printf("str長度爲：%d\n",(int)str.len);	
	ngx_str_set(&str,"hello world");                 //注意是&str而不是str
	p=ngx_snprintf(buffer, str.len, "%V", &str);   //注意是&str而不是str
	buffer[str.len]='\0';
	printf("格式化str輸出爲:%s\n",buffer);
	printf("格式化長度爲:%d\n",(int)((p-buffer)/sizeof(char)));   //p返回的是最後輸出的位置,所以由此可以得出最後格式化的字符個數
	return 1;
}