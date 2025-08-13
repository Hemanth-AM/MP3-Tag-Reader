#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mp3edit.h"
#include "typdef.h"

/*
    * Function to edit MP3 file tags based on command line arguments
    * Input: MP3FILE_EDIT structure pointer, command line arguments
    * Output: p_success or p_failure based on operation success
    * Return value: p_success or p_failure
*/
int edit_operation(MP3FILE_EDIT *tag_de, char **argv)
{
   

    unsigned char header[10];

    tag_de->fptr_temp = fopen("temp.mp3", "wb"); // open temp file in write mode
    if (tag_de->fptr_temp == NULL) {
        printf("temp file is not open\n");
        return p_failure;
    }

    fseek(tag_de->fptr_temp, 0L, SEEK_SET);
    fseek(tag_de->fptr, 0L, SEEK_SET);

    // Read 10 byte header from source file and write to temp
    fread(header, 1, 10, tag_de->fptr);
    fwrite(header, 1, 10, tag_de->fptr_temp);

 
int i =0;
    // Loop until all tag data is processed
    while (i < 20)
    {
        char fram[5];
        unsigned char size_bytes[4];
        char flag[2];
        int frame_size;

        // Read frame ID
        if (fread(fram, 1, 4, tag_de->fptr) != 4) break; // End of tag
        fram[4] = '\0';

        // Read size
        fread(size_bytes, 1, 4, tag_de->fptr);
        frame_size = (size_bytes[0] << 21) | (size_bytes[1] << 14) |
                     (size_bytes[2] << 7) | size_bytes[3];

        // Read flags
        fread(flag, 1, 2, tag_de->fptr);

        // Check if this is the frame we want to edit
        if ((strcmp(fram, "TPE1") == 0 && strstr(argv[2], "-a")) ||
            (strcmp(fram, "TIT2") == 0 && strcmp(argv[2], "-t") == 0) ||
            (strcmp(fram, "TALB") == 0 && strstr(argv[2], "-A")) ||
            (((strcmp(fram, "TYER") == 0) || (strcmp(fram, "TDRC") == 0)) && strstr(argv[2], "-y")) ||
            (strcmp(fram, "TCON") == 0 && strstr(argv[2], "-m")) ||
            (strcmp(fram, "TCOM") == 0 && strstr(argv[2], "-c")))
        {
            //printf("Editing %s frame\n", fram);
            if (add_new_content(tag_de, argv, fram, frame_size, flag) == p_failure) {
                printf("Adding new content is failure\n");
                return p_failure;
            }
        }
        else
        {
            // Copy unchanged frame
            if (exect_frame_copy(tag_de, fram, frame_size, size_bytes, flag) == p_failure) {
                printf("Frame copy operation failed\n");
                return p_failure;
            }
        }
        i++;
        

    }
      copy_remaining_data(tag_de);// Copy remaining data from source to temp file
      replace_source_with_temp(tag_de);// Replace original file with temporary file
      display(tag_de,argv);// Display the edited details

    return p_success;
}

/*
    * Function to execute frame copy operation
    * Input: MP3FILE_EDIT structure pointer, frame ID, size, size bytes, flags
    * Output: p_success or p_failure based on operation success
    * Return value: p_success or p_failure
 */
int exect_frame_copy(MP3FILE_EDIT *tag_de, char *fram, int size, unsigned char *size_bytes, char *flag)
{
    // Write frame ID
    fwrite(fram, 1, 4, tag_de->fptr_temp);

    // Write frame size
    fwrite(size_bytes, 1, 4, tag_de->fptr_temp);

    // Write flags
    fwrite(flag, 1, 2, tag_de->fptr_temp);

    // Copy frame data
    char *buffer = malloc(size);
    if (!buffer) return p_failure;

    fread(buffer, 1, size, tag_de->fptr);
    fwrite(buffer, 1, size, tag_de->fptr_temp);

    free(buffer);
    return p_success;
}

/*
    * Function to get frame content from MP3 file
    * Input: MP3FILEINFO structure pointer, temp variable to store content
    * Output: temp variable filled with frame content
    * Return value: p_success or p_failure based on operation success
 */
int add_new_content(MP3FILE_EDIT *tag_de, char **argv, char *fram, int old_size, char *flag)
{
    int new_size = strlen(argv[3]) + 1; // include encoding byte
    unsigned char size_bytes[4];

    // Frame ID
    fwrite(fram, 1, 4, tag_de->fptr_temp);

    // New size
    size_bytes[0] = (new_size >> 21) & 0x7F;
    size_bytes[1] = (new_size >> 14) & 0x7F;
    size_bytes[2] = (new_size >> 7)  & 0x7F;
    size_bytes[3] = new_size & 0x7F;
    fwrite(size_bytes, 1, 4, tag_de->fptr_temp);

    // Flags
    fwrite(flag, 1, 2, tag_de->fptr_temp);

    // Encoding + text
    unsigned char encoding = 0x00; // ISO-8859-1
    fwrite(&encoding, 1, 1, tag_de->fptr_temp);
    fwrite(argv[3], 1, new_size - 1, tag_de->fptr_temp);

    // Skip old content
    fseek(tag_de->fptr, old_size, SEEK_CUR);

    return p_success;
}

/*
    * Function to copy remaining data from source to temporary file
    * Input: MP3FILE_EDIT structure pointer
    * Output: Remaining data copied to temporary file
    * Return value: None
 */
void copy_remaining_data(MP3FILE_EDIT *tag_de)
{
    

    char buffer[8192]; // 8KB buffer for efficiency
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), tag_de->fptr)) > 0)
    {
        fwrite(buffer, 1, bytes_read, tag_de->fptr_temp);
    }
     
   
}

/*
    * Function to replace original MP3 file with the temporary file
    * Input: MP3FILE_EDIT structure pointer
    * Output: Original file replaced with temporary file
    * Return value: None
 */
void replace_source_with_temp(MP3FILE_EDIT *tag_de)
{
    fclose(tag_de->fptr);
    fclose(tag_de->fptr_temp);

    // Remove original file
    remove(tag_de->file_name);

    // Rename temp file to original file name
    rename("temp.mp3", tag_de->file_name);

    //printf("Edit operation completed successfully.\n");
}   

/*
    * Function to display the edited details
    * Input: MP3FILE_EDIT structure pointer, command line arguments
    * Output: Display the changes made to the MP3 file
    * Return value: None
 */
void display(MP3FILE_EDIT *tag_de, char **argv)
{
    printf("---------------------------------- SELECTED VIEW DETAILS------------------------------------------------ \n\n\n");
    printf("---------------------SELECTED EDIT OPTION -------------------\n\n");
   if(strcmp(argv[2] ,"-t")== 0 )
   {
     printf("---------------CHANGE THE TITLE-------------------------\n\n");
    
   
    printf("Title     :     %s\n\n", argv[3]);
    printf("----------------YEAR CHANGED SUCCESSFULLY-----------------\n\n");
   }
   else if(strcmp(argv[2] ,"-a")== 0 )
   {
    printf("-----------------CHANGE THE ARTIST-----------------------------\n\n");
   
    printf("Artist     :     %s\n\n", argv[3]);
    printf("-----------------ARTIST CHANGED SUCCESSFULLY-------------------\n\n");
   }
   else if(strcmp(argv[2] ,"-A")== 0 )
   {
    printf("----------------CHANGE THE ALBUM-------------------------------\n\n");
   
    printf("Album     :     %s\n\n", argv[3]);
    printf("----------------ALBUM CHANGED SUCCESSFULLY-------------------\n\n");
   }
   else if(strcmp(argv[2] ,"-y")== 0 )
   {
    printf("---------------CHANGE THE YEAR-------------------------------\n\n");

    printf("Year     :     %s\n\n", argv[3]);

    printf("---------------YEAR CHANGED SUCCESSFULLY-------------------\n\n");
   }
   else if(strcmp(argv[2] ,"-m")== 0 )
   {
    printf("----------------CHANGE THE GENRE-------------------------------\n\n");

    printf("Genre     :     %s\n\n", argv[3]);
    printf("----------------GENRE CHANGED SUCCESSFULLY-------------------\n\n");
   }
   else if(strcmp(argv[2] ,"-c")== 0 )
   {
    printf("----------------CHANGE THE COMPOSER-------------------------------\n\n");

    printf("Composer   :     %s\n\n", argv[3]);

    printf("----------------COMPOSER CHANGED SUCCESSFULLY-------------------\n\n");
   }
    
   

}