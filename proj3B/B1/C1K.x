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
	unsigned char token[44];
	};

struct netReply {
	int replyLen;
	struct Reply R;
	};

struct enc_reply { 
 	int enc_Len; 
 	unsigned char enc_message_array[200];  
 }; 
 
program CK_PROG {
	version CK_VERS {
		enc_reply requestSessionKey (clientReq) = 1;
		} = 1; /* version number */
	} = 0x32791986; /*0x32791986;  program number should be large & unique */

