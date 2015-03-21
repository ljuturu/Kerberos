#include <stdio.h>
#include <rpc/rpc.h>
#include "C1K.h"
#include "C2S.h"
#include <stdlib.h>
#include <string.h>
#include "Ckey.h"
#include <md5.h>

char * nullassigner(unsigned char * hosts);
char * nullassignertoken(unsigned char * h, int x);

main(argc, argv)
int     argc;
char    *argv[];
{
	CLIENT *cl, *c;            /* RPC handle */
	char *host, *hn;
	unsigned char uid[8], sid[8], rep[200], decryptedBlock[100], iv[8], db[160],arr[200],newkcs[8];
    clientReq req;
    serverREQ request;
    enc_reply *e_rep;
    enc_rep *er;
    enc_req ereq;
    unsigned int decryptedBlockLen,dbl, i;
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

	printf("Arguments passed to sessionKey request: \n Client ID = %s\n Server ID = %s\n",argv[1],argv[2]);
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
	netReply *nReply;
	int x;
	req.len=16;
	if((e_rep = requestsessionkey_1(req,cl)) == NULL)
	{
		clnt_perror(cl,"Call failed \n");
		exit(3);
	}
	if(e_rep->enc_Len == -1)
	{
		printf("%s\n",e_rep->enc_message_array);
		exit(4);
	}
	memset(rep,'\0',200);
	memcpy(rep,e_rep->enc_message_array,96);
	//memset(rep,e_rep->enc_message_array,strlen(e_rep->enc_message_array));
	printf("Encrypted Reply: \n");
	for (i=0; i<96; i++)
		printf("%03d ", (int)e_rep->enc_message_array[i]);
	printf("\n");

	R_DecryptPEMBlock(decryptedBlock, &decryptedBlockLen, rep,e_rep->enc_Len,key,iv);
	printf("PlainText Reply: %s\n ",decryptedBlock);
	char cid[9], s[9], tkn[45];
	unsigned char kcs[9];
	memset(cid,'\0',9);
	memcpy(cid,decryptedBlock,8);
	cid[8]='\0';
	memset(s,'\0',9);
	memcpy(s,decryptedBlock+8,8);
	s[8]='\0';
	memset(kcs,'\0',9);
	memcpy(kcs,decryptedBlock+16,8);
	kcs[8]='\0';
	strncpy(newkcs,kcs,8);
	printf("Client Id: %s\n Server Id: %s\n ", cid,s);
	printf("SessionKey: \n");
	for (i=0; i<8; i++)
		printf("%03d ", (int)kcs[i]);
	printf("\n");

	memset(tkn,'\0',45);
	memcpy(tkn,decryptedBlock+24,44);
	tkn[44]='\0';
	if(strcmp(argv[1],cid) == 0 && strcmp(argv[2],s) == 0)
		printf("Client and Server ID matched with sent arguments\n");
	else
	{
		printf("Client and Server ID NOT matched with sent arguments\n");
		exit(5);
	}

	printf("Encrypted Token: \n");
	for (i=0; i<44; i++)
		printf("%03d ", (int)tkn[i]);
	printf("\n");
	//printf("Sample trial 1..\n");
	//x = strlen(nullassigner(nReply->R.C))+strlen(nullassigner(nReply->R.S))+strlen(nullassigner(nReply->R.Key));
	//printf("Sample trial 2..\n");
	//char t[24];
	unsigned char eb[100];
	unsigned int ebl;
//	memset(t,'\0',24);
//	memcpy(t,nReply->R.token,24);
//	memset(request.ST,'\0',24);
//	memcpy(request.ST,t,24);
	//printf ("token legth after setting sp..%d\n",x);
    int inpNo;
	char inpStr[100];
	repSTR *rStr;
	printf("Enter the service No. you want: 1 for ALPHA or 2 for NUMERIC\n");
	scanf("%d",&inpNo);
	printf("Enter the input string for the above service\n");
	scanf("%s",inpStr);
	strLen=strlen(inpStr);
	memcpy(request.AS.aSTR,inpStr,100);
	request.length=140;

	R_EncryptPEMBlock(eb,&ebl,inpStr,100,kcs,iv);
	memset(tkn,'\0',45);
	memcpy(tkn,decryptedBlock+24,44);
	tkn[44]='\0';

	memset(arr,'\0',200);
	strncat(arr,argv[1],8);
	strncat(arr,argv[2],8);
	strncat(arr,eb,140);

//	printf("Encrypted Token middle\n");
//	for (i=0; i<44; i++)
//		printf("%03d ", (int)tkn[i]);
//	printf("\n");

	strncat(arr,tkn,44);

//	printf("Encrypted Array\n");
//	for (i=0; i<200; i++)
//		printf("%03d ", (int)arr[i]);
//	printf("\n");

	memset(ereq.enc_message_array,'\0',200);
	memcpy(ereq.enc_message_array,arr,200);
	ereq.enc_Len = strlen(ereq.enc_message_array);

	printf("Encrypted Argument\n");
	for (i=0; i<140; i++)
		printf("%03d ", (int)eb[i]);
	printf("\n");

	printf("Encrypted Token\n");
	for (i=0; i<44; i++)
		printf("%03d ", (int)tkn[i]);
	printf("\n");

	printf("Encrypted Array length %d\n",ereq.enc_Len);

	if(inpNo==1)
	{
		printf("ALPHA service: Non Alphabet characters will be removed \n");
		if((er = alpha_1(ereq,c)) == NULL)
		{
			clnt_perror(c,"Call Failed \n");
			exit(3);
		}

	}
	else if(inpNo==2)
	{
		printf("NUMERIC service: Non Digit characters will be removed \n");
		if((er = numeric_1(ereq,c)) == NULL)
			{
				clnt_perror(c,"Call Failed \n");
				exit(3);
			}

	}
	else {
		printf("Please enter either 1 or 2 \n");
		exit(4);
	}
	printf("Encrypted Reply string\n");
	for (i=0; i<er->enc_Len; i++)
			printf("%03d ", (int)er->enc_message_array[i]);
		printf("\n");
	memset(db,'\0',160);
	printf("Session Key: \n");
	for (i=0; i<8; i++)
		printf("%03d ", (int)newkcs[i]);
	printf("\n");
	R_DecryptPEMBlock(db, &dbl, er->enc_message_array,er->enc_Len,newkcs,iv);
	unsigned char cx[9], sx[9],xmsg[101];
	memset(cx,'\0',9);
	memcpy(cx,db,8);
	cx[8]='\0';
	memset(sx,'\0',9);
	memcpy(sx,db+8,8);
	sx[8]='\0';
	memset(xmsg,'\0',100);
	memcpy(xmsg,db+16,100);
	xmsg[100]='\0';
	//printf("dbl..%d\n",dbl);
	printf("Received Client %s \n",cx);
	printf("Received Server %s \n",sx);
	if( (strncmp(argv[1],cx,8) == 0) && (strncmp(argv[2],sx,8) == 0) )
	{
		printf("Plain text Reply: %s\n",xmsg);
	}
	else{
		printf("Client or Server not matched \n");
		exit(6);
	}
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
