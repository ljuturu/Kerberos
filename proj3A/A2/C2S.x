/* C2S.x */

struct argSTR {
	unsigned char aSTR[100];
	}; 

struct repSTR {
	int rLEN;
	unsigned char rSTR[100];
	}; 
	
struct serverREQ {
	int length;
	unsigned char C1[8];
	unsigned char S1[8];
	unsigned char ST[24];
	struct argSTR AS;
	}; 

program CP_PROG {
	version CP_VERS {
		repSTR ALPHA(serverREQ r) = 1;
		repSTR NUMERIC(serverREQ r) = 2;
		} = 1; /* version number */
	} = 0x32711986; /*0x32711986;  program number should be large & unique */

