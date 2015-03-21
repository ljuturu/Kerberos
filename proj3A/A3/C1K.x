struct clientReq { 
 	int len; 
 	unsigned char C[8]; 
 	unsigned char S[8]; 
 }; 


struct Token {
	unsigned char T[24];
	};

struct Reply {
	unsigned char C[8];
	unsigned char S[8];
	unsigned char Key[8];
	unsigned char token[24];
	};

struct netReply {
	int replyLen;
	struct Reply R;
	};

program CK_PROG {
	version CK_VERS {
		netReply requestSessionKey (clientReq) = 1;
		} = 1; /* version number */
	} = 0x32791986; /*0x32791986;  program number should be large & unique */

