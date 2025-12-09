#include<string.h>
#include "mp3.h"

int main(int argc,char *argv[])
{
    song s;
    if(argc<2)
    {
        printf("Insufficient arguments\n");
        get_help(argv[0]);
    }
    else if(argc==3 && strcmp(argv[1],"-v")==0)
    {
        view_mp3(argv[2]);

    }
    else if(argc==5 && strcmp(argv[1],"-e")==0)
    {
        edit_mp3(argv[2],argv[3],argv[4],&s);
    }
    else if(strcmp(argv[1],"-h")==0)
    {
        get_help(argv[0]);
    }
    else
    {
        printf("Invalid arguements.....TRY AGAIN.......\n");
        get_help(argv[0]);
    }
    return 0;
}