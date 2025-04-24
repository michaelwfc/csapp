# Reference
[curl-cookbook](https://catonmat.net/cookbooks/curl)


# sends GET requests
```bash 
# By default, curl sends GET requests. 
curl https://www.example.com
```

# sends POST requests
```bash 
curl -X POST https://www.example.com

# -d: Send a POST Request with Form Data
# This argument assumes that your data is URL-encoded already.
# When using the -d argument, curl also sets the Content-Type header to application/x-www-form-urlencoded
curl -d 'login=emma&password=123' -X POST https://google.com/login

# when the -d option is set, the -X POST argument can be skipped as curl will automatically set the request's type to POST.
curl -d 'login=emma&password=123' https://google.com/login

# Send a POST Request with JSON Data
# passing JSON to the -d option and also using the -H option that sets the Content-Type header to application/json. 
# I've also removed the -X POST argument as it can be skipped because -d forces a POST request.
curl -d '{"login": "emma", "pass": "123"}' -H 'Content-Type: application/json' https://google.com/login


# Send a POST Request with Plain Text Data
url -d 'hello world' -H 'Content-Type: text/plain' https://google.com/login

# Send a POST Request with Data from a File
curl -d '@data.txt' https://google.com/login

# POST a Binary File
# This recipe uses the -F argument that forces curl to make a multipart form data POST request. 
# It's a more complicated content type that's more efficient at sending binary files. 
# This recipe makes curl read an image photo.png and upload it to https://google.com/profile with a name file. 
# The -F argument also sets the Content-Type header to multipart/form-data.

curl -F 'file=@photo.png' https://google.com/profile
```

# use proxy


```bash 
## Use an HTTP Proxy

# In this recipe, the protocol:// part of the proxy address is not specified. 
# In this case, the HTTP proxy is assumed. Just like in the previous two recipes, the username and password are set to james:cats and the proxy host and port are set to myproxy.com:8080.

curl -x james:cats@myproxy.com:8080 https://catonmat.net

## Don't Use a Proxy for Host.com
curl --no-proxy host.com -x https://myproxy.com:8080 host.com https://catonmat.net https://digg.com https://host.com


# curl --proxy sends the entire URL to a proxy, which then contacts the real server.
# curl -x is exactly the same as curl --proxy
curl -v --proxy http://localhost:8000 http://localhost:8001/home.html
# *   Trying 127.0.0.1:8000...
# * TCP_NODELAY set
# * Connected to localhost (127.0.0.1) port 8000 (#0)
# > GET http://localhost:8001/home.html HTTP/1.1       //According to the HTTP specification, proxies must receive the full URL (including scheme, host, and path) in the request line, so they can forward it correctly. 
# > Host: localhost:8001
# > User-Agent: curl/7.68.0
# > Accept: */*
# > Proxy-Connection: Keep-Alive
# > 
# * Mark bundle as not supporting multiuse
# * HTTP 1.0, assume close after body
# < HTTP/1.0 404 Not found
# < Content-type: text/html
# < 
# <html><title>Tiny Error</title><body bgcolor=ffffff>
# 404: Not found
# <p>Tiny couldn't find this file: .http://localhost:8001/home.html
# <hr><em>The Tiny Web server</em>
# * Closing connection 0


# A normal curl sends the request directly to the server using just the relative path.
curl -v http://localhost:8001/home.html
# *   Trying 127.0.0.1:8001...
# * TCP_NODELAY set
# * Connected to localhost (127.0.0.1) port 8001 (#0)
# > GET /home.html HTTP/1.1            // A normal curl sends the request directly to the server using just the relative path.
# > Host: localhost:8001
# > User-Agent: curl/7.68.0
# > Accept: */*
# > 
# * Mark bundle as not supporting multiuse
# * HTTP 1.0, assume close after body
# < HTTP/1.0 200 OK
# < Server: Tiny Web Server
# < Content-length: 120
# < Content-type: text/html
# < 
# <html>
# <head><title>test</title></head>
# <body> 
# <img align="middle" src="godzilla.gif">
# Dave O'Hallaron
# </body>
# </html>
# * Closing connection 0

```

