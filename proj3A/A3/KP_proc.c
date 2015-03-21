/* KP_proc.c */
#include <rpc/rpc.h>     /* standard RPC include file */
#include "C1K.h"
#include <stdio.h>
#include <stdlib.h>

int KP_validator(clientReq req);
void keyGen(netReply *nReply);
void token_builder(netReply *nReply,char *cid,char *sid);
void replyBuilder(netReply *nReply);

netReply* requestsessionkey_1(clientReq req)
{
	char cid[9],sid[9];
	memset(cid,'\0',8);
	memcpy(cid,req.C,8);
	memset(sid,'\0',8);
	memcpy(sid,req.S,8);
	cid[8]='\0';
	sid[8]='\0';
	printf("SERVER_CK: Received Message \n");
	printf("Client= %s\n",cid);
	printf("Server= %s\n",sid);

	static struct netReply nReply;
	memset(nReply.R.C,'\0',8);
	memset(nReply.R.S,'\0',8);
	memcpy(nReply.R.C,req.C,8);
	memcpy(nReply.R.S,req.S,8);

	if (KP_validator(req) == 1)
	{
		keyGen(&nReply);
		token_builder(&nReply,cid,sid);
		replyBuilder(&nReply);
	}
	else
	{
		nReply.replyLen = -1;
	}
	return &nReply;

}


int KP_validator(clientReq req)
{
	return 1;

}

void keyGen(netReply *nReply)
{
	char tmp[9]="11112222";
	tmp[8]='\0';
	printf("Session key = %s\n",tmp);
	memcpy(nReply->R.Key,tmp,8);
}

void token_builder(netReply *nReply,char *cid,char *sid)
{
	char tkn[25];
	memset(tkn,'\0',25);
//	memcpy(tkn,cid,strlen(cid));
//	memcpy(tkn+8,sid,strlen(sid));
//	memcpy(tkn+16,nReply->R.Key,8);
    //tkn[24]='\0';

	strcat(tkn,cid);
	strcat(tkn,sid);
	strcat(tkn,nReply->R.Key);
	int x= strlen(cid)+strlen(sid)+8;
	tkn[x]='\0';

	printf("Token - %s\n",tkn);
//	memcpy(nReply->R.token,tkn,24);
	memset(nReply->R.token,'\0',x);
	memcpy(nReply->R.token,tkn,x);
//	printf("x length..%d\n",x);
//	printf("token length..%d\n",strlen(nReply->R.token));
}

void replyBuilder(netReply *nReply)
{
	nReply->replyLen = sizeof(Reply);
	//int s= nReply->replyLen;
//	printf("length..%d\n",nReply->replyLen);
}
