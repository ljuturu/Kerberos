/* SP_proc.c */
#include <rpc/rpc.h>     /* standard RPC include file */
#include "C2S.h"
#include <stdio.h>
#include <stdlib.h>

int SP_validator();
void replyBuilder(repSTR *rStr);
void print_details(serverREQ as);

repSTR* alpha_1(serverREQ as)
{
	char a[100],x[100];
	int i,len,j;
	static struct repSTR rep;
	if(SP_validator()==1)
	{
		len=strlen(as.AS.aSTR);
		memset(a,'\0',100);
		memcpy(a,as.AS.aSTR,len);

		j=0;
		for (i=0;i<len;i++)
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
		print_details(as);
		//printf("length of reply string..%d\n",j);
		x[j]='\0';
		a[len]='\0';
		//printf("Message Processed: %s\n",x);
		printf("Input String: %s\n",a);
		printf("Processed Reply: %s\n",rep.rSTR);
		replyBuilder(&rep);
		return &rep;
	}
}

repSTR* numeric_1(serverREQ as)
{
	char a[100],x[100];
	int i,len,j;
	static struct repSTR rep;
	if(SP_validator()==1)
	{
		len=strlen(as.AS.aSTR);
		memset(a,'\0',100);
		memcpy(a,as.AS.aSTR,len);

		j=0;
		for (i=0;i<len;i++)
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
		print_details(as);
		//printf("length of repl ystr..%d\n",j);
		x[j]='\0';
		a[len]='\0';
		//printf("Message Processed: %s\n",x);
		printf("Input String: %s\n",a);
		printf("Processed Reply: %s\n",rep.rSTR);
		replyBuilder(&rep);
		return &rep;
	}
}

int SP_validator()
{
	return 1;
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
void replyBuilder(repSTR *rStr)
{
	rStr->rLEN = strlen(rStr->rSTR);
	//printf("reply string len..%d \n",rStr->rLEN);
}
