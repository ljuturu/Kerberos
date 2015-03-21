#
BIN_C2S = SERVER C2P
GEN_C2S = C2S_xdr.c C2S_clnt.c C2S_svc.c C2S.h
LIB = -lnsl
CFLAGS = -I.. -O 
CC = gcc -DDEBUG

all:            $(GEN_C2S) $(BIN_C2S)

$(GEN_C2S):         C2S.x
		rpcgen -N C2S.x

SERVER:         C2S_svc.o C2S_xdr.o SP_proc.o
		$(CC) -o $@ SP_proc.c C2S_svc.o C2S_xdr.o $(LIB)

C2P:         C2S_clnt.c C2S_xdr.o C2P.o
		$(CC) -o $@ C2P.c C2S_clnt.c C2S_xdr.o $(LIB)

C2P.o:       C2P.c C2S.h

SP_proc.o:         SP_proc.c C2S.h

clean:
		rm -f $(GEN_C2S) *.o $(BIN_C2S)


