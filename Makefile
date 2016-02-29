CC=i586-mingw32msvc-g++

all:
	$(CC) -o httpget file_download.c -lws2_32
test:
	bash test.sh
        
