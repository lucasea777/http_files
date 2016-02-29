CC=i586-mingw32msvc-g++

all:
	$(CC) -o msockc file_download.c -lws2_32
