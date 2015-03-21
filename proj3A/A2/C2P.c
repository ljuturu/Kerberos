#include <stdio.h>
#include <rpc/rpc.h>
#include "C2S.h"
#include <stdlib.h>
#include <string.h>

main(argc, argv)
int     argc;
char    *argv[];
{
	CLIENT *c;            /* RPC handle */
	char *hn;
//	unsigned char cid[8];
//	unsigned char srid[8];
    struct serverREQ request;
    int strLen;
	if(argc != 4)
	{
		printf("Usage <UniqueId> <UserId> <ServerId> <ServerHost>\n");
		exit(1);
	}
	//strcpy(cid, argv[1]);
	memset(request.C1,'\0',8);
	memcpy(request.C1,argv[1],8);
	//strcpy(srid,argv[2]);
	memset(request.S1,'\0',8);
	memcpy(request.S1,argv[2],8);
	hn = argv[3];

/********************************************/
	/* Set up connection with the server which provides services given in AS_PROG.
	Connection is called client "handle." */
	if ( (c = clnt_create(hn, CP_PROG, CP_VERS, "udp")) == NULL) {
	 clnt_pcreateerror(hn);
	 exit(2);
	}
	//Constructed token
	char t[25];
	memset(t,'\0',25);
	strcat(t,argv[1]);
	strcat(t,argv[2]);
	strcat(t,"11112222");
	t[24]='\0';
	memcpy(request.ST,t,24);

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
    clnt_destroy(c);               /* done with the handle */
	exit(0);
}
