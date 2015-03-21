/* C2S.x */

struct argSTR {
	unsigned char aSTR[100];
	}; 

struct repSTR {
	int rLEN;
	unsigned char rSTR[100];
	unsigned char C[8];
	unsigned char S[8];
	}; 
	
struct serverREQ {
	int length;
	unsigned char C1[8];
	unsigned char S1[8];
	unsigned char ST[44];
	struct argSTR AS;
	}; 

struct enc_req { 
 	int enc_Len; 
 	unsigned char enc_message_array[200]; 
 }; 
 
 struct enc_rep { 
 	int enc_Len; 
 	unsigned char enc_message_array[200];  
 }; 
 
program CP_PROG {
	version CP_VERS {
		enc_rep ALPHA(enc_req r) = 1;
		enc_rep NUMERIC(enc_req r) = 2;
		} = 1; /* version number */
	} = 0x32711986; /*0x32711986;  program number should be large & unique */

