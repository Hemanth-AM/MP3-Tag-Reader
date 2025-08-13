#ifndef MP3VIEW_H
#define MP3VIEW_H

typedef struct _mp3view
{
    //file name
    char *file_name;
    //mp3 file pinter
    FILE *fptr;

    int size;
    char *Tpe1_detial; // Artist name
    char *Tit2_detial; // Title / songe name
    char *Talb_detial; // Albumb
    char *Tyer_detial; // year
    char *tcon_detial; // content type
    char *tcom_detial; // composer
    char *tcomm_detial; //comment

    
}MP3FILEINFO;

//perform view opration
int sor_fil_viw(MP3FILEINFO *mp3);

//To get the frame id and the size of the content
int get_size_framid(MP3FILEINFO *mp3,unsigned char *size_bytes,char *framid);

//To get the frame id content temp
int get_fram_content(MP3FILEINFO *mp3,char *temp);

//To display the contants of the .mp3 file
void  display_details(MP3FILEINFO *mp3);

//To save the file
int save_file(MP3FILEINFO *mp3);

#endif