#include<stdio.h>
#include"typdef.h"
#include"mp3view.h"
#include"mp3edit.h"
#include<string.h>

#define present 0
MP3FILEINFO mp3f;
MP3FILE_EDIT mp3e;
char choice;
int main(int argc , char **argv)
{
   
//to check cla after ./a.out any argument present or not
if(argc > 2 )
{

 //to check cla -v is present or not if present performe view operation
 if((strcmp(argv[1],"-v")) == present)
 {

     printf("Selected view option\n");

    //check mp3 extention present or not
    if((strstr(argv[2],".mp3")) != NULL )
    {
       char ID[3] = {0};
       int Version[3] = {0};

       //assigning source file name
       mp3f.file_name = argv[2];
       //open source file

       mp3f.fptr = fopen(mp3f.file_name,"r");

       //if file is not found then show the error messege
       if(mp3f.fptr == NULL)
       {
         perror("fopen");
         fprintf(stderr, "ERROR: Unable to open file %s\n", mp3f.file_name);
        }

       //check Id present or not 
       fread(ID,1,3,mp3f.fptr);
       ID[3] = '\0';

      if((strcmp(ID,"ID3")) != 0)
      {
        printf("File Identifier Is Not Found\n");
       return -1;
    
      }

      //check version
      fread(Version,1,2,mp3f.fptr);

      if(Version[0] == 3 && Version[1] == 0)
      {
        printf("Version is ID3v2.3.0\n");
      }
     else
     {
        printf("%d %d",Version[0],Version[1]);
        return -1;
      }
      if(sor_fil_viw(&mp3f) == p_failure)
     {
       printf("File view operstion is faliure");
      }
      else
      {
        printf("Would you like to save these tags to a CSV file? (Y/N): ");
        scanf("%c",&choice);
        if(choice == 'Y' || choice == 'y')
        {
           // Code to save tags to CSV
           if(save_file(&mp3f) == p_failure)
           {
               printf("Failed to save tags to CSV file\n");
           }
           else
           {
               printf("Tags saved successfully to CSV file.:)\n");
           }
        }
            // printf("File view operstion is faliure successful");
       } 

   }

   //not present show error
   else
    {
      printf("----------------------------------------------------------------------\n\n");
      printf("Error: Invalid source file\n");
      printf("source file Extention Mustby .mp3\n\n");
      printf("----------------------------------------------------------------------\n");
    }

  }

  //to check cla -e is present or not if present performe edit operation
  else if((strcmp(argv[1],"-e")) == present)
{
  //printf("selected edit option\n");
  char ID[4] = {0};
       int Version[3] = {0};

       //assigning source file name
       mp3e.file_name = argv[4];
       //open source file

       mp3e.fptr = fopen(mp3e.file_name,"r");

       //if file is not found then show the error messege
       if(mp3e.fptr == NULL)
       {
         perror("fopen");
         fprintf(stderr, "ERROR: Unable to open file %s\n", mp3e.file_name);
        }

       //check Id present or not 
       fread(ID,1,3,mp3e.fptr);
       ID[3] = '\0';

      if((strcmp(ID,"ID3")) != 0)
      {
        printf("File Identifier Is Not Found\n");
       return -1;
    
      }

      //check version
      fread(Version,1,2,mp3e.fptr);

      if(Version[0] == 3 && Version[1] == 0)
      {
       // printf("Version is ID3v2.3.0\n");
      }
     else
     {
        printf("%d %d",Version[0],Version[1]);
       return -1;
      }
       
      //check edit title 
      if(((strcmp(argv[2],"-t") == 0) || (strcmp(argv[2],"-a") == 0) || (strcmp(argv[2],"-A") == 0) || (strcmp(argv[2],"-y") == 0) || ((strcmp(argv[2],"-m") == 0)) || ((strcmp(argv[2],"-t") == 0))))
      {
        //printf("edit title is found\n");

       //performe edit operation
       if(edit_operation(&mp3e,argv) == p_failure)
       {
         printf("edit operation is falure");
         return p_failure; 
       }

      }
      else 
      {
        printf("----------------------------------------------------------------------\n\n");
        printf("Error: Edit title\n");
        printf("To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n\n");
        printf("----------------------------------------------------------------------\n");
      }

}
  else if((strcmp(argv[1],"--help")) == present)
  {

    printf("---------------------------------HELP MENU---------------------------------\n\n");
    printf("1. -v -> to view mp3 file contents\n");
    printf("2. -e -> to edit mp3 file contents\n");
    printf("         2.1. -t -> to edit song title\n");
    printf("         2.2. -a -> to edit artist name\n");
    printf("         2.3. -A -> to edit album name\n");
    printf("         2.4. -y -> to edit year\n");
    printf("         2.5. -m -> to edit content\n");
    printf("         2.6. -c -> to edit comment\n\n");
    printf("---------------------------------------------------------------------------\n\n");
  }

  // both -e and -v both not present in respective postion then show the error 
  else
  {
    printf("----------------------------------------------------------------------\n");
    printf("ERROR: ./a.out : INVALID ARGUMENTS\nUSAGE :\n");
    printf("To view please pass like: ./a.out -v mp3filename\nTo edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
    printf("To get help pass like : ./a.out --help\n");
    printf("----------------------------------------------------------------------\n");
    return -1;
  }
}
else
{
  printf("----------------------------------------------------------------------\n");
  printf("ERROR: ./a.out : INVALID ARGUMENTS\nUSAGE :\n");
  printf("To view please pass like: ./a.out -v mp3filename\nTo edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
  printf("To get help pass like : ./a.out --help\n");
  printf("----------------------------------------------------------------------\n");
  return -1;
}
}