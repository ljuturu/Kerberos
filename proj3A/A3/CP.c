#include <stdio.h>
#include <rpc/rpc.h>
#include "C1K.h"
#include "C2S.h"
#include <stdlib.h>
#include <string.h>

char * nullassigner(unsigned char * hosts);
char * nullassignertoken(unsigned char * h, int x);

main(argc, argv)
int     argc;
char    *argv[];
{
	CLIENT *cl, *c;            /* RPC handle */
	char *host, *hn;
	unsigned char uid[8];
	unsigned char sid[8];
    struct clientReq req;
    struct serverREQ request;
    int strLen;
	if(argc != 5)
	{
		printf("Usage <UniqueId> <UserId> <ServerId> <Keyhost> <ServerHost> \n");
		exit(1);
	}
	strcpy(uid, argv[1]);
	memset(req.C,'\0',8);
	memcpy(req.C,uid,8);
	strcpy(sid,argv[2]);
	memset(req.S,'\0',8);
	memcpy(req.S,sid,8);
	host = argv[3];

	memset(request.C1,'\0',8);
	memcpy(request.C1,argv[1],8);
	memset(request.S1,'\0',8);
	memcpy(request.S1,argv[2],8);
	hn = argv[4];

/********************************************/
	/* Set up connection with the server which provides services given in AS_PROG.
	Connection is called client "handle." */
	if ( (cl = clnt_create(host, CK_PROG, CK_VERS, "udp")) == NULL) {
	 clnt_pcreateerror(host);
	 exit(2);
	}

	if ( (c = clnt_create(hn, CP_PROG, CP_VERS, "udp")) == NULL) {
	 clnt_pcreateerror(hn);
	 exit(2);
	}

	//Constructed token
//	char t[25];
//	strcat(t,argv[1]);
//	strcat(t,argv[2]);
//	strcat(t,"12345678");
//	t[24]='\0';
//	memcpy(request.ST,t,24);
	//Add a new service request here
	struct netReply *nReply;
	int x;
	req.len=16;
	if((nReply = requestsessionkey_1(req,cl)) == NULL)
	{
		clnt_perror(cl,"Call failed \n");
		exit(3);
	}
	else
	{
		printf("Reply received from SERVER_CK: \n C = %s\n S = %s\n KeyCS = %s\n", nullassigner(nReply->R.C),nullassigner(nReply->R.S),nullassigner(nReply->R.Key));
		x = strlen(nullassigner(nReply->R.C))+strlen(nullassigner(nReply->R.S))+strlen(nullassigner(nReply->R.Key));
//		printf("x length %d\n",x);

		printf("The token = %s\n",nullassignertoken(nReply->R.token ,x));
	}

	char t[25];
	memset(t,'\0',25);
	memcpy(t,nReply->R.token,x);
	memset(request.ST,'\0',24);
	memcpy(request.ST,t,x);
	//printf ("token legth after setting sp..%d\n",x);
    int inpNo;
	char inpStr[100];
	struct repSTR *rStr;
	printf("Enter the service No. you want: 1 for ALPHA or 2 for NUMERIC\n");
	scanf("%d",&inpNo);
	printf("Enter the input string for the above service\n");
	scanf("%s",inpStr);
	strLen=strlen(inpStr);
	memcpy(request.AS.aSTR,inpStr,100);
	request.length=140;
	if(inpNo==1)
	{
		printf("ALPHA service: Non Alphabet characters will be removed \n");
		if((rStr = alpha_1(request,c)) == NULL)
		{
			clnt_perror(c,"Call Failed \n");
			exit(3);
		}

	}
	else if(inpNo==2)
	{
		printf("NUMERIC service: Non Digit characters will be removed \n");
		if((rStr = numeric_1(request,c)) == NULL)
			{
				clnt_perror(c,"Call Failed \n");
				exit(3);
			}

	}
	else {
		printf("Please enter either 1 or 2 \n");
		exit(4);
	}
    char d[100];
    int l =strlen(rStr->rSTR);
    memset(d,'\0',100);
    //printf("reply str len..%d\n",l);
    memcpy(d,rStr->rSTR,l);
    d[l]='\0';
	printf("Reply received from Server_CS : = %s\n", d);
/********************************************/
    clnt_destroy(cl); /* done with the handle */
    clnt_destroy(c);
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
