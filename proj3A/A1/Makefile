#
BIN_C1K = KEYSERVER C1P
GEN_C1K = C1K_xdr.c C1K_clnt.c C1K_svc.c C1K.h
LIB = -lnsl
CFLAGS = -I.. -O 
CC = gcc -DDEBUG

all:            $(GEN_C1K) $(BIN_C1K)

$(GEN_C1K):         C1K.x
		rpcgen -N C1K.x

KEYSERVER:         C1K_svc.o C1K_xdr.o KP_proc.o
		$(CC) -o $@ KP_proc.c C1K_svc.o C1K_xdr.o $(LIB)

C1P:         C1K_clnt.c C1K_xdr.o C1P.o
		$(CC) -o $@ C1P.c C1K_clnt.c C1K_xdr.o $(LIB)

C1P.o:       C1P.c C1K.h

KP_proc.o:         KP_proc.c C1K.h

clean:
		rm -f $(GEN_C1K) *.o $(BIN_C1K)


