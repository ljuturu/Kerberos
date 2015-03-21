#
BIN_ALL = SERVER_CK SERVER_CS CLIENT_ALL 
#BIN_C1K = KEYSERVER C1P
GEN_C1K = C1K_xdr.c C1K_clnt.c C1K_svc.c C1K.h
#BIN_C2S = SERVER C2P
GEN_C2S = C2S_xdr.c C2S_clnt.c C2S_svc.c C2S.h
LIB = -lnsl
CFLAGS = -I.. -O 
CC = gcc -DDEBUG

#all:            $(GEN_C1K) $(BIN_C1K)
all:            $(GEN_C1K) $(GEN_C2S) $(BIN_ALL)

$(GEN_C1K):         C1K.x
		rpcgen -N C1K.x
$(GEN_C2S):         C2S.x
		rpcgen -N C2S.x		

SERVER_CK:         C1K_svc.o C1K_xdr.o KP_proc.o
		$(CC) -o $@ KP_proc.c C1K_svc.o C1K_xdr.o $(LIB)
SERVER_CS:         C2S_svc.o C2S_xdr.o SP_proc.o
		$(CC) -o $@ SP_proc.c C2S_svc.o C2S_xdr.o $(LIB)		

#C1P:         C1K_clnt.c C1K_xdr.o C1P.o
#		$(CC) -o $@ C1P.c C1K_clnt.c C1K_xdr.o $(LIB)
CLIENT_ALL:         C1K_clnt.c C1K_xdr.o C2S_clnt.c C2S_xdr.o CP.o
		$(CC) -o $@ CP.c C1K_clnt.c C1K_xdr.o C2S_clnt.c C2S_xdr.o $(LIB)		

#C1P.o:       C1P.c C1K.h
CP.o:       CP.c C1K.h C2S.h

KP_proc.o:         KP_proc.c C1K.h
SP_proc.o:         SP_proc.c C2S.h

clean:
		rm -f $(GEN_C1K) $(GEN_C2S) *.o $(BIN_ALL)


