

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mp3view.h"
#include "typdef.h"

int store_content_ch(MP3FILEINFO *mp3, char *temp);

int sor_fil_viw(MP3FILEINFO *mp3)
{
    unsigned char size_bytes[4]; //to store framid content size
    char framid[5]; //to store framid id
    fseek(mp3->fptr, 10, SEEK_SET);  // Skip ID3 header

    int i = 0;
    while (i < 10) // it check the  first 10 frames
    {
        if (get_size_framid(mp3, size_bytes, framid) == p_success)
        {
            if (mp3->size <= 0) break;  // Invalid size, stop reading

            char *temp = malloc(mp3->size + 1); //allocated dinamic memory to store the fram id contant
            if (!temp) 
            {
                printf("Memory allocation failed\n");
                return p_failure;
            }

            if (get_fram_content(mp3, temp) == p_failure) 
            {
                free(temp);
                return p_failure;
            }

            /*Check encoding byte and skip it (0x00 or 0x03 is valid text encoding)
            because frome mp3 file first byte tells what kind of text formate is stored 
            Ex: 0x00 -> normal text , 0x03 UTF -8 this both are safe to print 
            */

            if (temp[0] == 0x00 || temp[0] == 0x03)
            {
                if (strcmp(framid, "TPE1") == 0)
                    mp3->Tpe1_detial = strdup(temp + 1);
                else if (strcmp(framid, "TIT2") == 0)
                    mp3->Tit2_detial = strdup(temp + 1);
                else if (strcmp(framid, "TALB") == 0)
                    mp3->Talb_detial = strdup(temp + 1);
                else if ((strcmp(framid, "TYER") == 0) || strcmp(framid, "TDRC") == 0)
                    mp3->Tyer_detial = strdup(temp + 1);
                else if (strcmp(framid, "TCON") == 0)
                    mp3->tcon_detial = strdup(temp + 1);
                else if ((strcmp(framid, "TCOM") == 0))
                    mp3->tcom_detial = strdup(temp + 1);
                else if ((strcmp(framid, "COMM") == 0))
                    mp3->tcomm_detial = strdup(temp + 1);
            }

            free(temp); //free the dynamic memory
        }
        else 
        {
            break; // Stop if reading a frame fails
        }

        i++;
    }

    display_details(mp3);
    
    return p_success;
}

/*
To get the frame id and the size of the content
Input        : structure pointer, size_bytes, framid variable
output       : frame id , size of data
return value : p_failure, e_failure
*/

int get_size_framid(MP3FILEINFO *mp3, unsigned char *size_bytes, char *framid)
{
    if (fread(framid, 1, 4, mp3->fptr) != 4)
        return p_failure;

    framid[4] = '\0';

    if (fread(size_bytes, 1, 4, mp3->fptr) != 4)
        return p_failure;

    /*
    why we need to code the size is mp3 files are often include byte pattern like
    0xFF , which can be be confused with mp3 frame headers  to privent thet the size 
    stored like msb bit  they make has 0 they convert 32 bit to 28 bit and then stored
    that is why we need to convert
    */
   
    // Decode synchsafe size
    mp3->size = (size_bytes[0] << 21) |
                (size_bytes[1] << 14) |
                (size_bytes[2] << 7) |
                (size_bytes[3]);

    // Skip frame flags
    fseek(mp3->fptr, 2, SEEK_CUR);

    return p_success;
}

/*
To get the frame id content temp
Input        : structure pointer, temp variable
output       : temp(contant)
return value : p_failure, e_failure
*/

int get_fram_content(MP3FILEINFO *mp3, char *temp)
{
    fread(temp, 1, mp3->size, mp3->fptr) ;
    

    temp[mp3->size] = '\0';  // Null-terminate for safety
    return p_success;
}

/*
To display the contants of the .mp3 file
Input        : structure pointer
output       : strcture members are displayed
*/
void  display_details(MP3FILEINFO *mp3)
{
     printf("---------------------------------- SELECTED VIEW DETAILS------------------------------------------------ \n\n\n");
     printf("---------------------------------------------------------------------------------------------------------\n");
     printf("                           MP3 TAG READER AND EDITOR FOR ID3v2\n");
     printf("---------------------------------------------------------------------------------------------------------\n");
    if (mp3->Tit2_detial) 
    printf("Title                :              %s\n", mp3->Tit2_detial);
    if (mp3->Tpe1_detial) 
    printf("Artist               :              %s\n", mp3->Tpe1_detial);
    if (mp3->Talb_detial) 
    printf("Album                :              %s\n", mp3->Talb_detial);
    if (mp3->Tyer_detial) 
    printf("Year                 :              %s\n", mp3->Tyer_detial);
    if (mp3->tcon_detial) 
    printf("Genre                :              %s\n", mp3->tcon_detial);
    if (mp3->tcom_detial) 
    printf("Composer             :              %s\n", mp3->tcom_detial);
    if (mp3->tcomm_detial) 
    printf("Comment              :              %s\n", mp3->tcomm_detial);
    printf("----------------------------------------------------------------------------------------------------------\n\n");
    printf("------------------------------------DETAILS DISPLAYED SUCCESSFULLY----------------------------------------\n\n");
}

int save_file(MP3FILEINFO *mp3)
{
    int not_present = 0;
    FILE *csv = fopen("mp3_tags.csv", "a+"); // Open for reading & appending
    if (!csv) {
        printf("Failed to open CSV file\n"); 
        return p_failure;
    }

    // Check if file is empty
    fseek(csv, 0, SEEK_END);
    if (ftell(csv) == 0) {
        not_present = 1;
    }

    // Write header if needed
    if (not_present) {
        fprintf(csv, "Title,Artist,Album,Year,Genre,Composer,Comment\n");
    }

    // Write tag details (replace NULL with N/A)
   fprintf(csv, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
    mp3->Tit2_detial ? mp3->Tit2_detial : "N/A",
    mp3->Tpe1_detial ? mp3->Tpe1_detial : "N/A",
    mp3->Talb_detial ? mp3->Talb_detial : "N/A",
    mp3->Tyer_detial ? mp3->Tyer_detial : "N/A",
    mp3->tcon_detial ? mp3->tcon_detial : "N/A",
    mp3->tcom_detial ? mp3->tcom_detial : "N/A",
    mp3->tcomm_detial ? mp3->tcomm_detial : "N/A"
);


    fclose(csv);
    return p_success;
}
