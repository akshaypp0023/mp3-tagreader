#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include "mp3.h"

int convert_size(unsigned char *bytes,int ver)
{
    int size=0;
    if(ver==3)
    {
        size = ((bytes[0] & 0xFF)<<24) | ((bytes[1] & 0xFF)<<16) | ((bytes[2] & 0xFF)<<8) | (bytes[3] & 0xFF);
    }
    else if(ver==4)
    {
        size = ((bytes[0] & 0x7F)<<21) | ((bytes[1] & 0x7F)<<14) | ((bytes[2] & 0x7F)<<7) | (bytes[3] & 0x7F);
    }
    return size;
}

void read_flags(song *s,FILE *fptr)
{
    fread(s->flag,1,2,fptr);
}
void read_size(song *s,FILE *fptr,int ver)
{
    unsigned char buff[4];
    fread(buff,1,4,fptr);
    s->frame_size=convert_size(buff,ver);
}
void read_tag(song *s,FILE *fptr)
{
    fread(s->tag,1,4,fptr);
    s->tag[4]='\0';
}
void read_data(song *s,FILE *fptr)
{
    if(s->frame_size > 0 && s->frame_size < 1024)
    {
        fread(s->frame_data,1,s->frame_size,fptr);
        s->frame_data[s->frame_size]='\0';
    }
    else
    {
        s->frame_data[0]='\0';
        fseek(fptr,s->frame_size,SEEK_CUR);
    }

}

void print_frame(song *s,FILE *fp)
{
    if(s->frame_size==0)
       return ;
    unsigned char encoding = s->frame_data[0];   //encoding byte 
    char *data=s->frame_data+1;  //data starting byte

    if(strcmp(s->tag,"COMM")==0)
    {
        //data=data+5;
        char lang[4]={0};//In COMM tag first
        memcpy(lang,data,4);
        data=data+3;
        while((data-s->frame_data) < s->frame_size && *data !='\0')
        data++;
        //data++;
        printf("Language: %s\n",lang);
        printf("Comment: ");
        for(int i=0;(data-s->frame_data)+i<s->frame_size;i++)
        {
            if(data[i]!='\0')
            printf("%c",data[i]);

        }
        printf("\n");
        return;
    }
    if(encoding == 0x00)   //UTF-8
    {
        printf(" %s\n",data);
    }
    else if(encoding == 0x03)
    {
        printf(" %s\n",data);
    }
    else if(encoding == 0x01 || encoding == 0x02)   //UTF-16
    {
        int start=0;
        if(s->frame_size>3 && (unsigned char)data[0] == 0xFF && (unsigned char)data[1]==0xFE)
           start=2;
        for(int i=start;i<s->frame_size-1;i+=2)
        {
            if(data[i]!=0 && data[i]<128)
               printf("%c",data[i]);
        }
        printf("\n");
    }
}

void write_size(unsigned char *size_bytes,int size,unsigned char ver)
{
    if(ver==3)
    {
        size_bytes[0]=(size>>24) & 0xFF;
        size_bytes[1]=(size>>16) & 0xFF;
        size_bytes[2]=(size>>8) & 0xFF;
        size_bytes[3]=size & 0xFF;
    }
    else if(ver==4)
    {
        size_bytes[0]=(size>>21) & 0x7F;
        size_bytes[1]=(size>>14) & 0x7F;
        size_bytes[2]=(size>>7) & 0x7F;
        size_bytes[3]=size & 0x7F;
    }

}
void view_mp3(char *file)
{
    FILE* fptr = fopen(file, "rb");
    if(!fptr){ printf("File not found\n"); return; }
    printf("---------------MP3 TAG READER--------------\n");
   char header[3];
    fread(header,1,3,fptr);
    if(strncmp(header,"ID3",3)!=0)
    {
        printf("ID3 TAG NOT FOUND\n");
        fclose(fptr);
        return ;
    }
    song s;
    unsigned char ver;
    fread(&ver,1,1,fptr);    //one byte of version 03 or 04
    fseek(fptr,2,SEEK_CUR);   //skips flag and revision byte

    unsigned char total_bytes[4];
    fread(total_bytes,1,4,fptr);
    int total_size=convert_size(total_bytes,ver);  //size of everything after header

    long int pos=ftell(fptr);
    while(ftell(fptr) < pos+total_size)
    {
        read_tag(&s,fptr);
        if(!isalpha(s.tag[0]))
         break;             //end of frames
        read_size(&s,fptr,ver);
        read_flags(&s,fptr);
        read_data(&s,fptr);
        if(strcmp(s.tag,"TPE1")==0) printf("%s-Artist: ",s.tag);
        else if(strcmp(s.tag,"TIT2")==0) printf("%s-Title: ",s.tag);
        else if(strcmp(s.tag,"TALB")==0) printf("%s-Album: ",s.tag);
        else if(strcmp(s.tag,"TYER")==0) printf("%s-Year: ",s.tag);
        else if(strcmp(s.tag,"TCON")==0) printf("%s-Content type: ",s.tag);
        else if(strcmp(s.tag,"TCOM")==0) printf("%s-Composer: ",s.tag);
        else if(strcmp(s.tag,"COMM")==0) printf("%s-Comments: ",s.tag);
        else continue;


 print_frame(&s,fptr);
    }
    fclose(fptr);
    printf("============================\n");

}
void edit_mp3(char* file,char* option,char* new_data,song* s)
{
       char ch;
    if(strcmp(option,"-t")==0)  ch='t';
    else if(strcmp(option,"-a")==0)  ch='a';
    else if(strcmp(option,"-r")==0)  ch='r';
    else if(strcmp(option,"-g")==0)  ch='g';
    else if(strcmp(option,"-y")==0)  ch='y';
    else if(strcmp(option,"-c")==0)  ch='c';
    else
    {
        printf("Invalid edit option\n");
        return;
    }
    FILE *fp1=fopen(file,"rb");
    if(!fp1)
    {
        printf("File opening error\n");
        return ;
    }
    char buff[3];
    fread(buff,1,3,fp1);
    if(strncmp(buff,"ID3",3)!=0)
    {
        printf("Invalid MP3 file\n");
        fclose(fp1);
        return;
    }
    unsigned char ver,rev,flag;
    fread(&ver,1,1,fp1);
    fread(&rev,1,1,fp1);
    fread(&flag,1,1,fp1);

    unsigned char size_bytes[4];
    fread(size_bytes,1,4,fp1);
    int new_tag_size=convert_size(size_bytes,ver);

    FILE *fp2=fopen("temp.mp3","wb+");
    if(!fp2)
    {
        printf("File creation error\n");
        fclose(fp1);
        return ;
    }

    unsigned char header[10];
    fseek(fp1,0,SEEK_SET);
    fread(header,1,10,fp1);
    fwrite(header,1,10,fp2);

    long int start_pos = ftell(fp1);
    int newlen = strlen(new_data);

    while(ftell(fp1)<(start_pos+new_tag_size))
    {
        read_tag(s,fp1);
        if(s->tag[0]==0)
           break;
        read_size(s,fp1,ver);
        read_flags(s,fp1);
        long data_start=ftell(fp1);
        int found=0;
        if(ch=='t'  && strcmp(s->tag,"TIT2")==0)  found=1;
        else if(ch=='a'  &&  strcmp(s->tag,"TALB")==0)  found=1;
        else if(ch=='r'  &&  strcmp(s->tag,"TPE1")==0)  found=1;
        else if(ch=='g'  &&  strcmp(s->tag,"TCON")==0)  found=1;
        else if(ch=='y'  &&  strcmp(s->tag,"TYER")==0)  found=1;
        else if(ch=='c'  &&  strcmp(s->tag,"TCOM")==0)  found=1;

        if(found)
        {
            fwrite(s->tag,1,4,fp2);
            int newsize=newlen+1;
            unsigned char newsizebytes[4];
            write_size(newsizebytes,newsize,ver);
            fwrite(newsizebytes,1,4,fp2);
            fwrite(&(s->flag[0]),1,1,fp2);
            fwrite(&(s->flag[1]),1,1,fp2);

            unsigned char encoding = 0x00;
            fwrite(&encoding,1,1,fp2);
            fwrite(new_data,1,newlen,fp2);

            fseek(fp1,s->frame_size,SEEK_CUR);  //skip old data

        }
        else
        {
            fseek(fp1,data_start-10,SEEK_SET);
            unsigned char *framedata = (unsigned char *)malloc(10+s->frame_size);
            fread(framedata,1,10+s->frame_size,fp1);
            fwrite(framedata,1,10+s->frame_size,fp2);
            free(framedata);
        }
    }
    unsigned char buffer[4096];
    size_t bytesread;
    while((bytesread=fread(buffer,1,sizeof(buffer),fp1))>0)
    {
        fwrite(buffer,1,bytesread,fp2);
    }
    fclose(fp1);
    fclose(fp2);
    printf("Tag edited successfully\n");
    remove(file);
    rename("temp.mp3", file);

}
void get_help(char *argv)
{
    printf("--------------------MAIN MENU-----------------------------------\n");
    printf("1.VIEW\n2.EDIT\n3.HELP\n");
    printf("-----------------------VIEW SYNTAX-------------------------------\n");
    printf("./a.out -v <filename.mp3>\n");
    printf("------------------------EDIT SYNTAX--------------------------------\n");
    printf("./a.out -e <filename.mp3> <-edittag> <newname>\n");
    printf("Edit Tags:\nt->Title\na->Album\nr->Artist\ng->Content Type(Genre)\ny->Year\nc->Composer\n ");
    printf("-------------------------HELP SYNTAX-------------------------------\n");
    printf("./a.out -h\n");
}