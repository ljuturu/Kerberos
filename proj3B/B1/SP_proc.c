/* SP_proc.c */
#include <rpc/rpc.h>     /* standard RPC include file */
#include "C2S.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <md5.h>
#include "Skey.h"

int SP_validator(char *cid, char *s, char *ct, char *st);
void replyBuilder(repSTR *rStr, enc_rep *e_rep, char *kcs, char *cid, char *s);
void print_details(serverREQ as);

enc_rep* alpha_1(enc_req er)
{
	char a[100],x[100],arr[200],iv1[8],db[24],dbm[100];
	int i,len,j,dbl,dblm,z;
	repSTR rep;
	static enc_rep e_rep;
	memset(arr,'\0',200);
	memset(iv1,'\0',8);
	memcpy(arr,er.enc_message_array,er.enc_Len);
//	printf("Encrypted array:\n");
//	for (i=0; i<200; i++)
//		printf("%03d ", (int)arr[i]);
//	printf("\n\n");
	serverREQ as;
	char cid[9], s[9],msg[141], tkn[45],ct[9],st[9];
	memset(cid,'\0',9);
	memcpy(cid,arr,8);
	cid[8]='\0';
	memset(s,'\0',9);
	memcpy(s,arr+8,8);
	s[8]='\0';
	memset(msg,'\0',141);
	memcpy(msg,arr+16,140);
	msg[140]='\0';
	printf("Output from Alpha service:\n");
	printf("Client Id: %s\n",cid);
	printf("Encrypted Argument:\n");
	for (i=0; i<140; i++)
		printf("%03d ", (int)msg[i]);
	printf("\n");
	memset(tkn,'\0',45);
	memcpy(tkn,arr+156,44);
	tkn[44]='\0';
	printf("Encrypted Token:\n");
	for (i=0; i<44; i++)
		printf("%03d ", (int)tkn[i]);
	printf("\n");

	R_DecryptPEMBlock(db, &dbl, tkn,44,key,iv1);
//	printf("db...%s \n",db);
//	printf("db l...%d \n",dbl);
	unsigned char kcs[9];
	memset(ct,'\0',9);
	memcpy(ct,db,8);
	ct[8]='\0';

	memset(st,'\0',9);
	memcpy(st,db+8,8);
	st[8]='\0';

	memset(kcs,'\0',9);
	memcpy(kcs,db+16,8);
	kcs[8]='\0';
	printf("Session key \n");
	for (i=0; i<8; i++)
		printf("%03d ", (int)kcs[i]);
	printf("\n");

	if(SP_validator(cid,s,ct,st)==1)
	{
		//len=strlen(as.AS.aSTR);
		R_DecryptPEMBlock(dbm, &dblm, msg,140,kcs,iv1);
		printf("Plain text input %s \n",dbm);
		//printf("dblm...%d\n",strlen(dbm));
		z = strlen(dbm);
		memset(a,'\0',100);
		memcpy(a,dbm,strlen(dbm));
		a[z]='\0';
		j=0;
		for (i=0;i<z;i++)
		{
			if( (a[i]>='a' && a[i]<='z') || (a[i]>='A' && a[i]<='Z') )
			{
				x[j]=a[i];
				j++;
			}
		}
		/* prepare reply */
		//x[j]='\0';

		memset(rep.rSTR,'\0',100);
		memcpy(rep.rSTR,x,j);
		//print_details(as);
		//printf("length of reply string..%d\n",j);
		x[j]='\0';
		//a[dblm]='\0';
		//printf("Message Processed: %s\n",x);
		printf("output String: %s\n",x);
		//printf("Processed Reply: %s\n",rep.rSTR);
		replyBuilder(&rep,&e_rep,kcs,cid,s);
		return &e_rep;
	}
	else
	{
		e_rep.enc_Len = -1;
		memcpy(e_rep.enc_message_array,"Client or Server Id not found",40);
		return &e_rep;
	}
}

enc_rep* numeric_1(enc_req er)
{
	char a[100],x[100],arr[200],iv2[8],db[24],dbm[100];
	int i,len,j,dbl,dblm,z;
	repSTR rep;
	static enc_rep e_rep;
	memset(arr,'\0',200);
	memset(iv2,'\0',8);
	memcpy(arr,er.enc_message_array,er.enc_Len);
//	printf("Encrypted array:\n");
//	for (i=0; i<200; i++)
//		printf("%03d ", (int)arr[i]);
//	printf("\n\n");
	char cid[9], s[9],msg[141], tkn[45],ct[9],st[9];
	memset(cid,'\0',9);
	memcpy(cid,arr,8);
	cid[8]='\0';
	memset(s,'\0',9);
	memcpy(s,arr+8,8);
	s[8]='\0';
	memset(msg,'\0',141);
	memcpy(msg,arr+16,140);
	msg[140]='\0';
	printf("Output from Numeric service:\n");
	printf("Client Id: %s\n",cid);
	printf("Encrypted Argument:\n");
	for (i=0; i<140; i++)
		printf("%03d ", (int)msg[i]);
	printf("\n");
	memset(tkn,'\0',45);
	memcpy(tkn,arr+156,44);
	tkn[44]='\0';
	printf("Encrypted Token:\n");
	for (i=0; i<44; i++)
		printf("%03d ", (int)tkn[i]);
	printf("\n");
	R_DecryptPEMBlock(db, &dbl, tkn,44,key,iv2);
	unsigned char kcs[9];
	memset(ct,'\0',9);
	memcpy(ct,db,8);
	ct[8]='\0';

	memset(st,'\0',9);
	memcpy(st,db+8,8);
	st[8]='\0';

	memset(kcs,'\0',9);
	memcpy(kcs,db+16,8);
	kcs[8]='\0';
	printf("Session key: \n");
	for (i=0; i<8; i++)
		printf("%03d ", (int)kcs[i]);
	printf("\n");
	if(SP_validator(cid,s,ct,st)==1)
	{
		//len=strlen(as.AS.aSTR);
		R_DecryptPEMBlock(dbm, &dblm, msg,140,kcs,iv2);
		printf("Plain text input %s \n",dbm);
		z = strlen(dbm);
		memset(a,'\0',100);
		memcpy(a,dbm,strlen(dbm));
		a[z]='\0';
		j=0;
		for (i=0;i<z;i++)
		{
			if(a[i]>='0' && a[i]<='9')
			{
				x[j]=a[i];
				j++;
			}
		}
		//x[j]='\0';
		/* prepare reply */

		memset(rep.rSTR,'\0',100);
		memcpy(rep.rSTR,x,j);
		//print_details(as);
		//printf("length of repl ystr..%d\n",j);
		x[j]='\0';
		//a[len]='\0';
		//printf("Message Processed: %s\n",x);
		printf("Output String: %s\n",x);
		printf("Processed Reply: %s\n",rep.rSTR);
		replyBuilder(&rep,&e_rep,kcs,cid,s);
		return &e_rep;
	}
	else
	{
		e_rep.enc_Len = -1;
		memcpy(e_rep.enc_message_array,"Client or Server Id not found",40);
		return &e_rep;
	}
}

int SP_validator(char *cid, char *s, char *ct, char *st)
{
	if(strcmp(cid,ct)==0 && strcmp(s,st) == 0)
	{
		printf("Output from validator\n");
		printf("Client Id: %s \n Server Id: %s\n",ct,st);
		printf("Client Id and Server Id matched\n");
		return 1;
	}
	else
		return -1;
}

void print_details(serverREQ as)
{
	//struct serverREQ *sr;
	char cl[9],svr[9],tk[25];
	int n = strlen(as.ST);
	memset(cl,'\0',8);
	memcpy(cl,as.C1,8);
	memset(svr,'\0',8);
	memcpy(svr,as.S1,8);
	cl[8]='\0';
	svr[8]='\0';
	printf("SERVER_CS: Received Message \n");
	printf("Client= %s\n",cl);
	printf("Server= %s\n",svr);

	memset(tk,'\0',24);
	memcpy(tk,as.ST,24);
	tk[24]='\0';
	printf("Token= %s\n",tk);

}
void replyBuilder(repSTR *rStr, enc_rep *e_rep, char *kcs, char *cid, char *s)
{
	char iv[8];
	memset(iv,'\0',8);
	int i;
	unsigned char strenc[117];
	memset(strenc,'\0',117);
	strncpy(strenc,cid,8);
	strncat(strenc,s,8);
	strncat(strenc,rStr->rSTR,100);
	strenc[116]='\0';
	R_EncryptPEMBlock(e_rep->enc_message_array,&e_rep->enc_Len,strenc,116,kcs,iv);
	printf("Encrypted Reply:\n");
	for (i=0; i<e_rep->enc_Len; i++)
		printf("%03d ", (int)e_rep->enc_message_array[i]);
	printf("\n");
	rStr->rLEN = strlen(rStr->rSTR);
	//printf("reply string len..%d \n",rStr->rLEN);
}
