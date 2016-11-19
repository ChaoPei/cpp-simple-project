#
# Makefile
# web server
#

CXX = g++

EXEC_HTTP = server_http
EXEC_HTTPS = server_https

SOURCE_HTTP = main_http.cpp
SOURCE_HTTPS = main_https.cpp


OBJECTS_HTTP = main_http.o
OBJECTS_HTTPS = main_https.o


# 开启编译器优化和对现场支持
# 链接动态库库

# -l: 要连接的库名(libboost_system.so)
LDFLAGES_COMMON = -std=c++11 -O3 -pthread -lboost_system
LDFLAGES_HTTP =
LDFLAGES_HTTPS = -lssl -lcrypto


# -L: 第一个寻找库文件的目录(还有默认目录)
LLIB_COMMON = -L/usr/lib
LLIB_HTTP = 
LLIB_HTTPS = 


# -I: 第一个寻找头文件的目录(include)(还有默认目录)
LPATH_COMMON = -I/usr/include/boost
LPATH_HTTP = 
LPATH_HTTPS = -I/usr/include/openssl


all:
	make http
	make https

# http任务
http: 
	$(CXX) $(SOURCE_HTTP) $(LDFLAGES_COMMON) $(LDFLAGES_HTTP) $(LLIB_COMMON) $(LLIB_HTTP) $(LPATH_COMMON) $(LPATH_HTTP) -o $(EXEC_HTTP)


# https任务
https:
	$(CXX) $(SOURCE_HTTPS) $(LDFLAGES_COMMON) $(LDFLAGES_HTTPS) $(LLIB_COMMON) $(LLIB_HTTPS) $(LPATH_COMMON) $(LPATH_HTTPS) -o $(EXEC_HTTPS)

clean:
	rm -f $(EXEC_HTTP) $(EXEC_HTTPS) *.o

