#include<stdio.h>
#include<stdlib.h>
#include "Information.h"
#include "gettype.cpp"
int get_position(FILE *fp,char *str1, char *str2, unsigned *posi,unsigned *offs,NALU_t *nalu);
int get_nalu(FILE *fp, unsigned *posi, NALU_t *nalu,int size);
int main()
{
    int b=0,p=0,ii=0,sei=0,sps=0,pps=0;
    char startstr1[4]={0x00,0x00,0x00,0x01};
    char startstr2[3]={0x00,0x00,0x01};
    unsigned position_len;
    unsigned position[10000];//store the start position of each nalu(begin with the start conde)
    unsigned offset[10000];//store the offset of each nalu(begin with the header, in order to compare with h264_analyze)
    NALU_t nalu[10000];
    //nalu=malloc(sizeof(NALU_t));
    int *framesize,frame_num,i;
    int type[10000];
    FILE *fp,*fpres;
    fp=fopen("for_birds.h264","r");
    fpres=fopen("result.txt","w");
    if(!fpres)
    printf("null file!\n");
    position_len=get_position(fp,startstr1,startstr2,position,offset,nalu);
    frame_num=position_len;
    framesize=malloc(sizeof(int)*frame_num);
    //GetFrameType(nalu);
    for (i=1;i<=frame_num;i++)
    {
        framesize[i-1]=position[i]-position[i-1];
    }
    get_nalu(fp,position,nalu,frame_num);
    for (i=0;i<frame_num;i++)
    {
        GetFrameType(&nalu[i]);
        if(nalu[i].Frametype==16)
        p++;
        if(nalu[i].Frametype==15)
        ii++;
        if(nalu[i].Frametype==17)
        b++;
        if(nalu[i].Frametype==6)
        sei++;
        if(nalu[i].Frametype==7)
        sps++;
        if(nalu[i].Frametype==8)
        pps++;
        printf("offset =%d,size=%d\n",offset[i],nalu[i].len);
    }
    

}

int get_position(FILE *fp,char *str1, char *str2, unsigned *posi,unsigned *offs,NALU_t *nalu)
{
    if(!fp||!str1||!str2) return 0;
    unsigned pos=0,pos_end=0;
    unsigned pos_len=0;
    int strlen1=4,strlen2=3;
    char *buf1=malloc(sizeof(char)*strlen1);
    if(!buf1) return 0;
    fseek(fp,0L,SEEK_END);
    pos_end=ftell(fp)-4;
    int res1=0,res2=0;
    while(pos<=pos_end)
    {
        fseek(fp,pos,SEEK_SET);
        res1=fread(buf1,sizeof(char),(strlen1+1),fp);
        if(res1!=(strlen1+1))break;
        if(memcmp(str1,buf1,strlen1)==0)
        {
            nalu[pos_len].forbidden_bit=0;
            nalu[pos_len].nal_unit_type=buf1[strlen1]&0x1f;
            nalu[pos_len].startcodeprefix_len=4;

            offs[pos_len]=pos+4;
            posi[pos_len]=pos;
            pos_len++;
        }
        else if((memcmp(str1,buf1,strlen1)!=0)&&(memcmp(str2,(buf1+sizeof(char)),strlen2)==0))
        {
            nalu[pos_len].forbidden_bit=0;
            nalu[pos_len].nal_unit_type=buf1[strlen1]&0x1f;
            nalu[pos_len].startcodeprefix_len=3;
            offs[pos_len]=pos+4;
            posi[pos_len]=pos+1;
            pos_len++;
        } 
        pos++;
    }
    posi[pos_len]=pos_end+4;  //virtual position for last frame size

    fseek(fp,0L,SEEK_SET);
    free(buf1);
    return pos_len;
}

int get_nalu(FILE *fp, unsigned *posi, NALU_t *nalu,int size)
{
    int i=0;
    char *buf=malloc(10000);
    for (i=0;i<size;i++)
    {
        if(nalu[i].startcodeprefix_len==4)
        {
            nalu[i].len=posi[i+1]-posi[i]-4;
            nalu[i].buf=malloc(nalu[i].len);
            fread(buf,sizeof(char),(posi[i+1]-posi[i]),fp);
            memcpy(nalu[i].buf,buf+4,nalu[i].len);
        }
        else if(nalu[i].startcodeprefix_len==3)
        {
            nalu[i].len=posi[i+1]-posi[i]-3;
            nalu[i].buf=malloc(nalu[i].len);
            fread(buf,sizeof(char),(posi[i+1]-posi[i]),fp);
            memcpy(nalu[i].buf,buf+3,nalu[i].len);
        }
    }
    fseek(fp,0L,SEEK_SET);
}
