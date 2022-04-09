#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>

pthread_t tid[105];
char listfiles[105][1005];

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

void copy_files(char src[ ], char dest[ ]){
    FILE *source, *target;
    int i;
    source = fopen(src, "rb"); 

    if( source == NULL ) { printf("Press any key to exit...\n");} //exit(EXIT_FAILURE); 

    fseek(source, 0, SEEK_END);
    int length = ftell(source);

    fseek(source, 0, SEEK_SET);
    target = fopen(dest, "wb"); 

    if( target == NULL ) { fclose(source); } //exit(EXIT_FAILURE);

    for(i = 0; i < length; i++){
    fputc(fgetc(source), target);
    }

    printf("File %s copied successfully.\n\n", src); 
    fclose(source); 
    fclose(target);
}

void *organize(void *ptr){
    char * filename;
    filename = (char *) ptr;

    printf("processing %s...\n", filename);

    if(filename){
        char source_path[1005];
        char dir_target[505];
        char str[105];
        strcpy(str, filename);
        char target_path[1005];
        sprintf(source_path, "workspaces/%s", filename);


        if(filename[0] == '.'){
            
            sprintf(target_path, "hartakarun/hidden/%s", filename);
            strcpy(dir_target, "hidden");

        } else{
            
            strrev(str);
            char *token = strtok(str, ".");
            strrev(token);
            strcpy(dir_target, token);

            if(strcmp(dir_target, filename) == 0) strcpy(dir_target, "Unknown");
            else if(strcmp(dir_target, "gz") == 0) sprintf(dir_target, "tar.gz");
            else{
                int l = strlen(dir_target), i;
                for(i = 0;i < l; i++) dir_target[i] = tolower(dir_target[i]);
            }

            sprintf(target_path, "hartakarun/%s/%s", dir_target, filename);

        }

        printf("%s will go to %s\n", filename, dir_target);
        char dump[1005];
        sprintf(dump, "hartakarun/%s", dir_target);
        mkdir(dump, 0777);

        copy_files(source_path, target_path);
    }
}

int main(){
    mkdir("hartakarun", 0777);
    DIR *d = opendir("workspaces");
    struct dirent *dir;
    int i=0, err;

    //menyetarakan yang berada di dalam sub direktori
    if(d){
        while((dir = readdir(d)) != NULL){
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue;
            char * filename = dir->d_name;
            err=pthread_create(&(tid[i]),NULL,organize,(void*) filename); //membuat thread
            if(err!=0) //cek error
            {
                printf("\n can't create thread : [%s]",strerror(err));
            }
            else
            {
                printf("\n create thread %s success\n", filename);
            }
            i++;

        }
    } closedir(d);

    int j = 0;
    for(j = 0; j < i; j++) pthread_join(tid[j], NULL);
}
