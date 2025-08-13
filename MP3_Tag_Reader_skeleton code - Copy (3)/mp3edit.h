#ifndef MP3EDIT_H
#define MP3EDIT_H

#include<stdio.h>


typedef struct _mp3edit
{
    ///file name
    char *file_name;
    //mp3 file pinter
    FILE *fptr;

    /* Temporary MP3 file Info */
    FILE* fptr_temp;

	/* Updated value Info passed in the Command Line */
    char frame_Id [5];
    char frame_Id_value [100];
    uint frame_Id_size;
    
}MP3FILE_EDIT;

// Function to edit MP3 file tags based on command line arguments
int edit_operation(MP3FILE_EDIT *tag_de,char **argv);

// Function to execute frame copy operation
int exect_frame_copy(MP3FILE_EDIT *tag_de, char *fram, int size, unsigned char *size_bytes, char *flag);

// Function to get frame content from MP3 file
int add_new_content(MP3FILE_EDIT *tag_de, char **argv, char *fram, int old_size, char *flag);

// Function to copy remaining data from source to temporary file
void copy_remaining_data(MP3FILE_EDIT *tag_de);

// Function to replace original MP3 file with the temporary file
void replace_source_with_temp(MP3FILE_EDIT *tag_de);

// Function to display the edited details
void display(MP3FILE_EDIT *tag_de, char **argv);


#endif