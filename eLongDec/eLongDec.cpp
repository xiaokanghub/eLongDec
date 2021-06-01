//// eLongDec.cpp : 定义控制台应用程序的入口点。
////
//
////#include "stdafx.h"
//#include "aes128enc.h"
//#include "stdlib.h"
//#include "stdio.h"
//
///*add round key,wo,wk:(word_t *),i:(int)*/
//#define add_round_key(wo,wk,i) \
//for((i)=0;(i)<4;(i)++){\
//    (wo)[i]^=(wk)[i];\
//}
//
///*shift row left,op:(byte_t*),k:(int)*/
//#define shift_rows_left(op,k)\
//{\
//        k=(op)[1];            \
//        (op)[1]=(op)[5];    \
//        (op)[5]=(op)[9];    \
//        (op)[9]=(op)[13];    \
//        (op)[13]=k;            \
//        k=(op)[2];            \
//        (op)[2]=(op)[10];    \
//        (op)[10]=k;            \
//        k=(op)[6];            \
//        (op)[6]=(op)[14];    \
//        (op)[14]=k;            \
//        k=(op)[15];            \
//        (op)[15]=(op)[11];    \
//        (op)[11]=(op)[7];    \
//        (op)[7]=(op)[3];    \
//        (op)[3]=k;            \
//}
//
//#define sbox_shift_rows_left(op,k)\
//{\
//        (op)[0]=sbox[(op)[0]];        \
//        (op)[4]=sbox[(op)[4]];        \
//        (op)[8]=sbox[(op)[8]];        \
//        (op)[12]=sbox[(op)[12]];    \
//        k=sbox[(op)[1]];            \
//        (op)[1]=sbox[(op)[5]];        \
//        (op)[5]=sbox[(op)[9]];        \
//        (op)[9]=sbox[(op)[13]];        \
//        (op)[13]=k;                    \
//        k=sbox[(op)[2]];            \
//        (op)[2]=sbox[(op)[10]];        \
//        (op)[10]=k;                    \
//        k=sbox[(op)[6]];            \
//        (op)[6]=sbox[(op)[14]];        \
//        (op)[14]=k;                    \
//        k=sbox[(op)[15]];            \
//        (op)[15]=sbox[(op)[11]];    \
//        (op)[11]=sbox[(op)[7]];        \
//        (op)[7]=sbox[(op)[3]];        \
//        (op)[3]=k;                    \
//}
//
//#define subword(wt,sbox)\
//{\
//    (wt).b[0]=(sbox)[(wt).b[0]];\
//    (wt).b[1]=(sbox)[(wt).b[1]];\
//    (wt).b[2]=(sbox)[(wt).b[2]];\
//    (wt).b[3]=(sbox)[(wt).b[3]];\
//}
//
//static void PKCS7_Pad(byte_t * buf,int buflen,int blocksize,byte_t * paddingBuf)
//{
//    int i;
//    int p=blocksize-buflen%blocksize;
//    for(i=0;i<buflen;i++)
//        paddingBuf[i]=buf[i];
//
//    for(i=buflen;i<blocksize;i++)
//        paddingBuf[i]=p;
//
//}
//
//void key_expansion(const bw4 * key,word_t * w)
//{
//    bw  temp;
//    int i,j;
//    for(i=0;i<4;i++)
//    {   
//        w[i]=key->w[i];
//    }   
//    for(i=4;i<44;i++)
//    {   
//        temp.w=w[i-1];
//        if(i%4==0)
//        {   
//            temp.w=ROTATE_RIGHT(temp.w,32,8);//this for little Endian (bigEndian:rotate right 24bit)
//            subword(temp,sbox);
//            temp.w^=RC[i/4-1];
//        }   
//        w[i]=w[i-4]^temp.w;
//    }   
//}
//
///*AES128 encryption*/
//int Aes128_Enc(bw4 * input,int inlen,const bw4 * key,int keylen,bw4 * output)
//{
//    if(!input || inlen!=16 || !key || keylen!=16 || !output)
//        return -1;    
//    int i=0,j,round=0;
//    byte_t k;
//    bw tr;
//    for(i=0;i<4;i++)
//        output->w[i]=input->w[i];
//    key_expansion(key,W);
//    //Add Round Key
//    add_round_key(output->w,W+round*4,i);
//    round++;
//
//    while(round<10){
//        //ShiftRows
//        shift_rows_left(output->b,k);
//
//        //subbytes & mixcolumns
//        for(i=0;i<4;i++)
//        {
//            output->w[i]=sm0[output->b[i*4]].w ^ 
//                         sm1[output->b[i*4+1]].w ^
//                         sm2[output->b[i*4+2]].w ^
//                         sm3[output->b[i*4+3]].w ^
//                         (W+round*4)[i];
//        }
//
//        round++;    
//    }
//
//    //subBytes & ShiftRows
//    sbox_shift_rows_left(output->b,k);
//
//    //AddRoundKey
//    add_round_key(output->w,W+round*4,i);
//
//    return 0;
//}
//
//int Aes128cbc_Enc(bw4 * state,int slen,const word_t * wkey,const bw4 * iv) 
//{
//    if(!state || slen!=16 || !wkey )
//        return -1;  
//    int i=0,j,round=0;
//    byte_t k;
//    bw tr;
//
//    for(i=0;i<4;i++) 
//        state->w[i]^=iv->w[i];
//
//    //Add Round Key 
//    add_round_key(state->w,wkey+round*4,i);
//    round++;
//
//    while(round<10){
//        //shift rows
//        shift_rows_left(state->b,k);
//
//        //subbytes & mixcolumns
//        for(i=0;i<4;i++)
//        {
//            state->w[i]= sm0[state->b[i*4]].w ^ 
//                         sm1[state->b[i*4+1]].w ^
//                         sm2[state->b[i*4+2]].w ^
//                         sm3[state->b[i*4+3]].w ^
//                         (wkey+round*4)[i];
//        }
//
//        round++;
//    }
//
//    //subBytes & ShiftRows
//    sbox_shift_rows_left(state->b,k);
//
//    //AddRoundKey
//    add_round_key(state->w,wkey+round*4,i);
//
//    return 0;
//}
//
//int Aes128cbc_Pkcs7_Enc(bw4 * input,int inlen,const word_t * wkey,bw4 * output,const bw4 * iv)
//{
//    int i=0,j,round = 0;
//    byte_t k;
//    bw tr;
//    if(inlen<16){
//        PKCS7_Pad(input->b,inlen,16,output->b);
//        for(i=0;i<4;i++) output->w[i]^=iv->w[i];
//    }else{
//        for(i=0;i<4;i++) output->w[i]=input->w[i]^iv->w[i];
//    }    
//
//    //Add Round Key
//    add_round_key(output->w,wkey+round*4,i);
//    round++;
//
//    while(round<10){
//        //ShiftRows
//        shift_rows_left(output->b,k);
//
//        for(i=0;i<4;i++)
//        {
//            output->w[i]=sm0[output->b[i*4]].w ^ 
//                         sm1[output->b[i*4+1]].w ^
//                         sm2[output->b[i*4+2]].w ^
//                         sm3[output->b[i*4+3]].w ^
//                         (wkey+round*4)[i];
//        }
//
//        round++;    
//    }
//
//    //subbytes & ShiftRows
//    sbox_shift_rows_left(output->b,k);
//
//    //AddRoundKey
//    add_round_key(output->w,wkey+round*4,i);
//    return 0;
//}
//
//void print4x(byte_t * box)
//{
//	int i;
//	for(i=0;i<16;i++)
//	{
//		//if(i%4==0) putchar('\n');
//		printf("%02x",box[i]);
//	}
//	putchar('\n');
//	putchar('\n');
//}
//
//
//
//int main()
//{
//	const bw4 IV[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//	const word_t KEY[16]={0x35,0x49,0x46,0x6F,0x72,0x64,0x54,0x49,0x4F,0x64,0x4F,0x62,0x30,0x4C,0x4C,0x43};
//	bw4 input[16]={0x34,0x34,0x31,0x38,0x32,0x36,0x31,0x39,0x38,0x33,0x31,0x31,0x32,0x38,0x31,0x37};
//	bw4 *S=(bw4 *)calloc(16+1,sizeof(byte_t));
//	Aes128cbc_Pkcs7_Enc(input,sizeof(input),KEY,S,IV);
//	if(S){
//		//reverse4x(S);
//		print4x((byte_t *)S);
//	}
//
//
//	return 0;
//}
//
