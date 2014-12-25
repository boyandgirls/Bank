ifndef SystemRoot
    LIBS = -w -pthread -ldl
endif

bank: main.c sqlite3.c Database.c Dialog.c
	gcc -o bank main.c sqlite3.c Database.c Dialog.c -std=c99 $(LIBS)

