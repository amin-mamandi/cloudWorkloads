# nginx-rs

On server's machine,
```bash
To start Nginx server
$ cd server
$ sudo ./nginxServer.sh <taskset to cores> <path to custom nginx.conf file>
```

On client's machine, 
```bash
Build wrk tool
$ make

Use wrk
$ ./wrk -t 40 -c 80 -d 60s --latency http://127.0.0.1/
```

nghttp2 manual which includes 2 benchmarking tools: nghttpx and h2load (have UNIX Socket)

https://nghttp2.org/documentation/h2load.1.html

https://nghttp2.org/documentation/nghttpx.1.html 
```bash
Build nghttp2 tool
$ autoreconf -i
$ automake
$ autoconf
$ ./configure --enable-app
$ make
nghttpx and h2load are located at src/
```
