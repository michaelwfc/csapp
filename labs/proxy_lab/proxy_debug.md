
# Debug with curl
vscode debug tiny server (--port 8001)
vscode debug proxy (--port 8000)

## Directly GET Request on tiny server
curl -v http://localhost:8001/home.html
* Uses proxy env variable no_proxy == 'localhost,127.0.0.1,::1'
*   Trying 127.0.0.1:8001...
* TCP_NODELAY set
* Connected to localhost (127.0.0.1) port 8001 (#0)
> GET /home.html HTTP/1.1
> Host: localhost:8001
> User-Agent: curl/7.68.0
> Accept: */*
> 
* Mark bundle as not supporting multiuse
* HTTP 1.0, assume close after body
< HTTP/1.0 200 OK
< Server: Tiny Web Server
< Content-length: 120
< Content-type: text/html
< 
<html>
<head><title>test</title></head>
<body> 
<img align="middle" src="godzilla.gif">
Dave O'Hallaron
</body>
</html>
* Closing connection 0
```

## curl with proxy

### on curl shell
```bash
# Use localhost:8000 as an HTTP proxy server
# And request the URL http://localhost:8001/home.html through that proxy
curl -v --proxy http://localhost:8000  http://localhost:8001/home.html

curl -v --proxy http://localhost:8000  http://localhost:8001/home.html -o ./.proxy/home.html

*   Trying 127.0.0.1:8000...
* TCP_NODELAY set
* Connected to localhost (127.0.0.1) port 8000 (#0)
> GET http://localhost:8001/home.html HTTP/1.1
> Host: localhost:8001
> User-Agent: curl/7.68.0
> Accept: */*
> Proxy-Connection: Keep-Alive
> 
* Mark bundle as not supporting multiuse
* HTTP 1.0, assume close after body
< HTTP/1.0 200 OK
< Server: Tiny Web Server
< Content-length: 120
< Content-type: text/html
< 
<html>
<head><title>test</title></head>
<body> 
<img align="middle" src="godzilla.gif">
Dave O'Hallaron
</body>
</html>
```

### on proxy shell

```bash
start proxy at port:8000
Accepted connection from (localhost, 48564)

Proxy: parsing request from client:
GET http://localhost:8001/home.html HTTP/1.1
Host: localhost:8001
User-Agent: curl/7.68.0
Accept: */*
Proxy-Connection: Keep-Alive

Proxy: Forwarding to localhost:8001
GET /home.html HTTP/1.0
Host: localhost:8001
User-Agent: User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3
Connection: close
Proxy-Connection: close
Accept: */*



Proxy Receiving  response from localhost:8001
HTTP/1.0 200 OK
Server: Tiny Web Server
Content-length: 120
Content-type: text/html

<html>
<head><title>test</title></head>
<body> 
<img align="middle" src="godzilla.gif">
Dave O'Hallaron
</body>
</html>

```

# Debug with chrome
## Web browser directly on tiny server port:  http://localhost:8001/home.html

when I input  browser with  http://localhost:8001/home.html, why the browser  
- first make request line with GET /home.html HTTP/1.1 from Host: localhost:8001,
  return response:
```html
  HTTP/1.0 200 OK
  Server: Tiny Web Server
  Content-length: 120
  Content-type: text/html

  <html>
  <head><title>test</title></head>
  <body> 
  <img align="middle" src="godzilla.gif">
  Dave O'Hallaron
  </body>
  </html>
```

- then make second request:  GET /godzilla.gif HTTP/1.1
- but why the web browser  make a thrid request GET /favicon.ico HTTP/1.1?
Browsers automatically try to fetch a favicon (the small icon shown in browser tabs or bookmarks) by requesting:


1. Initial HTML Request:
First Request: Fetch the main HTML document (/home.html).
2. Parsing the HTML:
Upon receiving the HTML content, the browser parses it to render the page.
In your case, the HTML contains an image tag <img align="middle" src="godzilla.gif">.
3. Subsequent Resource Requests:
Fetch additional resources referenced within the HTML (in this case, the image godzilla.gif).

This multi-step process ensures that all elements required to display the complete web page are retrieved from the server.

```bash
start tiny web server at port: 8001
Accepted connection from (localhost, 50548)
GET /home.html HTTP/1.1
Host: localhost:8001
Connection: keep-alive
sec-ch-ua: "Google Chrome";v="135", "Not-A.Brand";v="8", "Chromium";v="135"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Windows"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/135.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Sec-Fetch-Site: none
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: zh-CN,zh;q=0.9

Accepted connection from (localhost, 50558)
GET /godzilla.gif HTTP/1.1
Host: localhost:8001
Connection: keep-alive
sec-ch-ua-platform: "Windows"
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/135.0.0.0 Safari/537.36
sec-ch-ua: "Google Chrome";v="135", "Not-A.Brand";v="8", "Chromium";v="135"
sec-ch-ua-mobile: ?0
Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: no-cors
Sec-Fetch-Dest: image
Referer: http://localhost:8001/home.html
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: zh-CN,zh;q=0.9

Accepted connection from (localhost, 50566)
```

# evalution
$ ./driver.sh 
*** Basic ***
Starting tiny on 14824
Port 14824 is in use.
Starting proxy on 4618
./driver.sh: line 254: f: command not found
1: home.html
   Fetching ./tiny/home.html into ./.proxy using the proxy
   Fetching ./tiny/home.html into ./.noproxy directly from Tiny
   Comparing the two files
   Success: Files are identical.
2: csapp.c
   Fetching ./tiny/csapp.c into ./.proxy using the proxy
   Fetching ./tiny/csapp.c into ./.noproxy directly from Tiny
   Comparing the two files
   Success: Files are identical.
3: tiny.c
   Fetching ./tiny/tiny.c into ./.proxy using the proxy
   Fetching ./tiny/tiny.c into ./.noproxy directly from Tiny
   Comparing the two files
   Success: Files are identical.
4: godzilla.jpg
   Fetching ./tiny/godzilla.jpg into ./.proxy using the proxy
   Fetching ./tiny/godzilla.jpg into ./.noproxy directly from Tiny
   Comparing the two files
   Success: Files are identical.
5: tiny
   Fetching ./tiny/tiny into ./.proxy using the proxy
   Fetching ./tiny/tiny into ./.noproxy directly from Tiny
   Comparing the two files
   Success: Files are identical.
Killing tiny and proxy
basicScore: 40/40

*** Concurrency ***
Starting tiny on port 24665
Port 24665 is in use.
Starting proxy on port 13375
Port 13375 is in use.
Starting the blocking NOP server on port 14622
HOME_DIR = /mnt/e/projects/csapp/labs/proxy_lab/proxylab-handout
Server listening on port 14622
Port 14622 is in use.
Trying to fetch a file from the blocking nop-server
Fetching ./tiny/home.html into ./.noproxy directly from Tiny
Fetching ./tiny/home.html into ./.proxy using the proxy
Checking whether the proxy fetch succeeded
Success: Was able to fetch tiny/home.html from the proxy.
Killing tiny, proxy, and nop-server
concurrencyScore: 15/15

*** Cache ***
Starting tiny on port 23785
Port 23785 is in use.
Starting proxy on port 13787
Port 13787 is in use.
Fetching ./tiny/tiny.c into ./.proxy using the proxy
Fetching ./tiny/home.html into ./.proxy using the proxy
Fetching ./tiny/csapp.c into ./.proxy using the proxy
Killing tiny
Fetching a cached copy of ./tiny/home.html into ./.noproxy
Success: Was able to fetch tiny/home.html from the cache.
Killing proxy
cacheScore: 15/15

totalScore: 70/70
