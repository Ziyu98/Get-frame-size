#include<stdio.h>
#include<stdlib.h>
int get_position(FILE *fp,char *str1, char *str2, unsigned *posi);
int main()
{
    char startstr1[4]={0x00,0x00,0x00,0x01};
    char startstr2[3]={0x00,0x00,0x01};
    unsigned position_len;  //length of position array
    unsigned position[15000];  //store the position of each start code
    int *framesize,frame_num,i;
    FILE *fp,*fpres;
    fp=fopen("for_birds.h264","r");
    fpres=fopen("result.txt","w");
    if(!fpres)
    printf("null file!\n");
    position_len=get_position(fp,startstr1,startstr2,position);
    frame_num=position_len;
    framesize=malloc(sizeof(int)*frame_num);

    for (i=1;i<frame_num;i++)
    {
        framesize[i-1]=position[i]-position[i-1];
    }
    fseek(fp,0L,SEEK_END);
    for(i=1;i<frame_num;i++){
    fprintf(fpres,"%d\t",framesize[i-1]);
    printf("size of frame [%d] = %d \n",i-1,framesize[i-1]);}

}

int get_position(FILE *fp,char *str1, char *str2, unsigned *posi)
{
    if(!fp||!str1||!str2) return 0;
    unsigned pos=0,pos_end=0;
    unsigned pos_len=0;
    int strlen1=4,strlen2=3;
    char *buf1=malloc(sizeof(char)*strlen1);
    if(!buf1) return 0;
    fseek(fp,0L,SEEK_END);
    pos_end=ftell(fp)-4;
    int res1=0;
    while(pos<=pos_end)
    {
        fseek(fp,pos,SEEK_SET);
        res1=fread(buf1,sizeof(char),strlen1,fp);
        if(res1!=strlen1)break;
        if(memcmp(str1,buf1,strlen1)==0)  //the position of "00 00 00 01"
        {
            posi[pos_len]=pos;
            pos_len++;
        }
        else if((memcmp(str1,buf1,strlen1)!=0)&&(memcmp(str2,(buf1+sizeof(char)),strlen2)==0))
        {
            posi[pos_len]=pos+1;    //position of 00 00 01
            pos_len++;
        }
        pos++;
    }

    fseek(fp,0L,SEEK_SET);
    free(buf1);
    return pos_len;
}

