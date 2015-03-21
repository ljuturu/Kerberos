#include <stdio.h>
#include <rpc/rpc.h>
#include "C1K.h"
#include <stdlib.h>

char * nullassigner(unsigned char * hosts);
char * nullassignertoken(unsigned char * hosts, int x);

main(argc, argv)
int     argc;
char    *argv[];
{
	CLIENT *cl;            /* RPC handle */
	char *host;
	unsigned char uid[8];
	unsigned char sid[8];
    struct clientReq req;

	if(argc != 4)
	{
		printf("Usage <UniqueId> <UserId> <ServerId> <Keyhost>\n");
		exit(1);
	}
	strcpy(uid, argv[1]);
	memset(req.C,'\0',8);
	memcpy(req.C,uid,8);
	strcpy(sid,argv[2]);
	memset(req.S,'\0',8);
	memcpy(req.S,sid,8);
	host = argv[3];

/********************************************/
	/* Set up connection with the server which provides services given in AS_PROG.
	Connection is called client "handle." */
	if ( (cl = clnt_create(host, CK_PROG, CK_VERS, "udp")) == NULL) {
	 clnt_pcreateerror(host);
	 exit(2);
	}
	//Add a new service request here
	struct netReply *nReply;
	req.len=16;
	if((nReply = requestsessionkey_1(req,cl)) == NULL)
	{
		clnt_perror(cl,"Call failed \n");
		exit(3);
	}
	else
	{
		printf("Reply received: \n CLIENT = %s\n SERVER = %s\n KeyCS = %s\n", nullassigner(nReply->R.C),nullassigner(nReply->R.S),nullassigner(nReply->R.Key));
		int x = strlen(nullassigner(nReply->R.C))+strlen(nullassigner(nReply->R.S))+strlen(nullassigner(nReply->R.Key));
		//printf("x length %d\n",x);

		printf("The token = %s\n",nullassignertoken(nReply->R.token ,x));
	}

/********************************************/
    clnt_destroy(cl);               /* done with the handle */
	exit(0);
}

char * nullassigner(unsigned char * hosts)
{
	char * K = malloc(sizeof(char) * 9);
	memcpy(K, hosts, 8);
 	K[8] = '\0';
 	return K;
}

char * nullassignertoken(unsigned char * h, int x)
{
	char * K = malloc(sizeof(char) * 25);
	memcpy(K, h, x);
	K[x] = '\0';
	return K;

}
