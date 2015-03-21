/* KP_proc.c */
#include <rpc/rpc.h>     /* standard RPC include file */
#include "C1K.h"
#include <stdio.h>
#include <stdlib.h>
#include <md5.h>
#include <string.h>

char * KP_validator(clientReq req, char h);
void keyGen(netReply *nReply);
void token_builder(netReply *nReply,char *cid,char *sid, char *skey);
void replyBuilder(netReply *nReply, char *ckey, enc_reply *erep);

enc_reply* requestsessionkey_1(clientReq req)
{
	char cid[9],sid[9];
	char *ckey = malloc(sizeof(char) * 8);
	char *skey =  malloc(sizeof(char) * 8);
	memset(cid,'\0',8);
	memcpy(cid,req.C,8);
	memset(sid,'\0',8);
	memcpy(sid,req.S,8);
	cid[8]='\0';
	sid[8]='\0';
	printf("Output from Session Key Service \n Arguments:\n");
	//printf("SERVER_CK: Received Message \n");
	printf("Client ID: %s\n",cid);
	printf("Server ID: %s\n",sid);

	static struct enc_reply encReply;
	struct netReply nReply;
	ckey=KP_validator(req, 'c');
	if (ckey == NULL)
	{
		encReply.enc_Len = -1;
		printf("Client Id not found \n");
		memset(encReply.enc_message_array,'\0',200);
		memcpy(encReply.enc_message_array,"Client Id not found",strlen("Client Id not found"));
		return &encReply;
	}
	skey=KP_validator(req, 's');
	if (skey == NULL)
	{
		encReply.enc_Len = -1;
		printf("Server Id not found \n");
		memset(encReply.enc_message_array,'\0',200);
		memcpy(encReply.enc_message_array,"Server Id not found",strlen("Server Id not found"));
		return &encReply;
	}
	printf("Client ID verified \n");
	printf("Server ID verified \n");
	memset(nReply.R.C,'\0',8);
	memset(nReply.R.S,'\0',8);
	memcpy(nReply.R.C,req.C,8);
	memcpy(nReply.R.S,req.S,8);
	keyGen(&nReply);
	memset(sid,'\0',8);
	memcpy(sid,req.S,8);
//	printf("Client before passing: %s\n",cid);
//	printf("Server before passing: %s\n",sid);
	token_builder(&nReply,cid,sid,skey);
	replyBuilder(&nReply,ckey,&encReply);

	return &encReply;

}


char* KP_validator(clientReq req, char h)
{
	char id[8], fk[16];
	char *sec_key= malloc(8*sizeof(char));
	int v=0,size=0;
	FILE * fp;
	memset(id,'\0',8);
	memset(sec_key,'\0',8);
	if (h == 'c')
		memcpy(id,req.C,8);
	else if(h == 's')
		memcpy(id,req.S,8);
	//printf("for %c\n",h);
	fp=fopen("DB.key","r");
 	while((size=fread(fk,1,16,fp)) > 0)
 	{
 		if(strncmp(fk,id,8)==0)
	 	{
			v=1;
 			memcpy(sec_key,fk+8,8);
 			//printf(" entry found \n");
	 		break;
	 	}
 		//printf("not found \n");
	}
	fclose(fp);
	//printf("before return\n");
	//printf("before return sec_key...%s\n",sec_key);
	//printf("v..%d \n",v);
	if (v==1) return sec_key;
	else return NULL;

}

void keyGen(netReply *nReply)
{
	int i;
	FILE *dt, *popen();
	unsigned char text[128], randText[128];
	dt = popen("date; ps -e", "r");
	fread(text, 128, 1, dt);
	md5_calc(randText, text, 128);
	memset(nReply->R.Key,'\0',8);
	memcpy(nReply->R.Key,randText,8);
	pclose(dt);
	printf("Generated Session Key:\n");
	for (i=0; i<8; i++)
	   printf("%03d ", (int)nReply->R.Key[i]);
	printf("\n");
}

void token_builder(netReply *nReply,char *cid,char *sid, char *skey)
{
	char tkn[25];
	int i;
	unsigned char iv[8], encryptedBlock[100];
	unsigned int encryptedBlockLen;
	memset(tkn,'\0',25);
	memset(iv, '\0', 8);
//	memcpy(tkn,cid,strlen(cid));
//	memcpy(tkn+8,sid,strlen(sid));
//	memcpy(tkn+16,nReply->R.Key,8);
    //tkn[24]='\0';
//	printf("cid..%s \n",cid);
//	printf("sid..%s \n",sid);
	strcat(tkn,cid);
	strcat(tkn,sid);
	//printf("tkn..with cs..%s",tkn);
	strcat(tkn,nReply->R.Key);
	int x= strlen(cid)+strlen(sid)+strlen(nReply->R.Key);
	//printf("x..%d \n",x);
	tkn[x]='\0';
	printf("PlainText Token: %s\n",tkn);
	R_EncryptPEMBlock(encryptedBlock,&encryptedBlockLen,tkn,24,skey,iv);
	printf("Encrypted Token len=%d\n", encryptedBlockLen);
	printf("Encrypted Token: \n");
	for (i=0; i < 44; i++)
	   printf("%03d ", (int)encryptedBlock[i]);
	printf("\n\n");
//	memcpy(nReply->R.token,tkn,24);
	//printf("here..12");
	memset(nReply->R.token,'\0',44);
	//printf("here..11");
	memcpy(nReply->R.token,encryptedBlock,encryptedBlockLen);
	//printf("here..");
//	printf("x length..%d\n",x);
//	printf("token length..%d\n",strlen(nReply->R.token));
}

void replyBuilder(netReply *nReply, char *ckey, enc_reply *erep)
{
	char rep[100];
	int x, i;
	unsigned char iv[8], encryptedBlock[200];
	unsigned int encryptedBlockLen;
	memset(rep,'\0',100);
	memset(iv, '\0', 8);

	strncat(rep,nReply->R.C,8);
		strncat(rep,nReply->R.S,8);
		strncat(rep,nReply->R.Key,8);
		strncat(rep,nReply->R.token,44);
	//printf("in reply builder\n");
	x= strlen(nReply->R.C)+strlen(nReply->R.S)+strlen(nReply->R.Key)+strlen(nReply->R.token);
	rep[x]='\0';
	printf("PlainText Reply: %s\n",rep);
	R_EncryptPEMBlock(encryptedBlock,&encryptedBlockLen,rep,68,ckey,iv);
	printf("Encrypted Reply len=%d\n", encryptedBlockLen);
	printf("Encrypted Reply: \n");
	for (i=0; i<encryptedBlockLen; i++)
	   printf("%03d ", (int)encryptedBlock[i]);
	printf("\n");
	erep->enc_Len = encryptedBlockLen;
	memset(erep->enc_message_array,'\0',200);
	memcpy(erep->enc_message_array,encryptedBlock,200);
	//int s= nReply->replyLen;
//	printf("length..%d\n",nReply->replyLen);
}
