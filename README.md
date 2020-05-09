# ngx_http_wangyonglin_json_module

###Nginx 开发模块入门项目
```
	./configure --prefix=/usr/local/nginx --user=nginx --group=nginx --add-module=/home/ngx_http_wangyonglin_json_module
```

###Nginx 配置文件 nginx.conf
```
	location /count {
			wangyonglin_string wangyonglin;
			wangyonglin_counter on;
			}
```

