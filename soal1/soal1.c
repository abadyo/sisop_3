#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <dirent.h>
#include <stdint.h>

int statusR = 0;
int statusA = 0;

/* ---- Base64 Encoding/Decoding Table --- */
char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* decodeblock - decode 4 '6-bit' characters into 3 8-bit binary bytes */
void decodeblock(unsigned char in[], char *clrstr) {
  unsigned char out[4];
  out[0] = in[0] << 2 | in[1] >> 4;
  out[1] = in[1] << 4 | in[2] >> 2;
  out[2] = in[2] << 6 | in[3] >> 0;
  out[3] = '\0';
  strncat(clrstr, out, sizeof(out));
}

void b64_decode(char *b64src, char *clrdst) {
  int c, phase, i;
  unsigned char in[4];
  char *p;

  clrdst[0] = '\0';
  phase = 0; i=0;
  while(b64src[i]) {
    c = (int) b64src[i];
    if(c == '=') {
      decodeblock(in, clrdst); 
      break;
    }
    p = strchr(b64, c);
    if(p) {
      in[phase] = p - b64;
      phase = (phase + 1) % 4;
      if(phase == 0) {
        decodeblock(in, clrdst);
        in[0]=in[1]=in[2]=in[3]=0;
      }
    }
    i++;
  }
}

/* encodeblock - encode 3 8-bit binary bytes as 4 '6-bit' characters */
void encodeblock( unsigned char in[], char b64str[], int len ) {
    unsigned char out[5];
    out[0] = b64[ in[0] >> 2 ];
    out[1] = b64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? b64[ ((in[1] & 0x0f) << 2) |
             ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? b64[ in[2] & 0x3f ] : '=');
    out[4] = '\0';
    strncat(b64str, out, sizeof(out));
}

/* encode - base64 encode a stream, adding padding if needed */
void b64_encode(char *clrstr, char *b64dst) {
  unsigned char in[3];
  int i, len = 0;
  int j = 0;

  b64dst[0] = '\0';
  while(clrstr[j]) {
    len = 0;
    for(i=0; i<3; i++) {
     in[i] = (unsigned char) clrstr[j];
     if(clrstr[j]) {
        len++; j++;
      }
      else in[i] = 0;
    }
    if( len ) {
      encodeblock( in, b64dst, len );
    }
  }
}




void garputunggu(char bash[], char *arg[]){
    int status;
    pid_t child;
    child = fork();
    if(child == 0){
        execv(bash, arg);
    }
    else{
        ((wait(&status))>0);
    }
}

void *garputunggu_unzip( void *ptr )
{
    char *message;
    message = (char *) ptr;
    int status;
    pid_t child;
    if(strcmp(message, "quote.zip") == 0) {
        while(statusA != 1){}
        child = fork();
        if(child == 0){
            execlp("unzip", "unzip", message, "-d", "quote", NULL);
        }
        else{
            ((wait(&status))>0);
        }
        statusA = 2;
    } else if (strcmp(message, "music.zip") == 0){
        while(statusA != 2) {}
        child = fork();
        if(child == 0){
            execlp("unzip", "unzip", message, "-d", "music", NULL);
        }
        else{
            ((wait(&status))>0);
        }
    } else {
        while(statusR != 1){};
        char passw[100];
        char *nama_user;
        nama_user=(char *)malloc(10*sizeof(char));
        nama_user=getlogin();
        snprintf(passw, sizeof(passw), "mihinomenest%s", nama_user);
        child = fork();
        if(child == 0){
            execlp("unzip", "unzip", "-P", passw, message, NULL);
        }
        else{
            ((wait(&status))>0);
        }
        statusR = 2;
    }
}

void zip_garputunggu(char passw[], char namaz[], char namaf[]) {
    int status;
    pid_t child;
    child = fork();
    if(child == 0){
        execlp("zip", "zip", "-P", passw, "-r", namaz, namaf, NULL);
    }
    else{
        ((wait(&status))>0);
    }
}

void *garputunggu_mkdir( void *ptr )
{
    char *message;
    message = (char *) ptr;
    int status;
    pid_t child;
    child = fork();
    if(child == 0){
        execlp("mkdir", "mkdir", message, NULL);
    }
    else{
        ((wait(&status))>0);
    }
    statusA = 1;
}

void *garputunggu_hapus_dir( void *ptr )
{
    char *message;
    message = (char *) ptr;
    int status;
    pid_t child;
    child = fork();
    if(child == 0){
        execlp("rm", "rm", "-r", message, NULL);
    }
    else{
        ((wait(&status))>0);
    }
    statusR = 1;
}

void *garputunggu_hapus_file( void *ptr )
{
    while(statusR != 3) {}
    char *message;
    message = (char *) ptr;
    int status;
    pid_t child;
    child = fork();
    if(child == 0){
        execlp("rm", "rm", message, NULL);
    }
    else{
        ((wait(&status))>0);
    }
}

void *garputunggu_file( void *ptr )
{
    char *message;
    message = (char *) ptr;

    if(strcmp(message, "hasil") == 0) {
        while(statusR != 2) {}
        char nulis[100];
        snprintf(nulis, sizeof(nulis), "./%s/no.txt", message);
        FILE *tambahan = fopen(nulis, "w");
        fprintf(tambahan, "No");
        fclose(tambahan);
        statusR = 3;
    } else {
        DIR *filebuka;
        struct dirent *ep;

        char nulis[100];
        snprintf(nulis, sizeof(nulis), "./%s/%s.txt", message, message);

        FILE *translate = fopen(nulis, "w");

        filebuka = opendir(message);
        if(filebuka != NULL) {
            while(ep = readdir(filebuka)) {
                if(strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0) {
                    char line[200];
                    char awal[300];
                    snprintf(awal, sizeof(awal), "./%s/%s", message, ep->d_name);
                    FILE *terbuka = fopen(awal, "r");
                    while(fgets(line, sizeof(line), terbuka)) {
                        // long panjang_line = strlen(line);
                        // char * Ltrans = base64_decode(line, panjang_line, &panjang_line);
                        char hasilT[100];
                        b64_decode(line, hasilT);
                        fprintf(translate, "%s\n", hasilT);
                    }
                    fclose(terbuka);
                }
            }
            (void) closedir (filebuka);
        } else  perror ("Couldn't open the directory");

        fclose(translate);
    }

}

void *garputunggu_move( void *ptr )
{
    char *message;
    message = (char *) ptr;
    int status;
    pid_t child;
    if(strcmp(message, "quote") == 0) {
        child = fork();
        if(child == 0){
            char awal[100];
            snprintf(awal, sizeof(awal), "./%s/quote.txt", message);
            execlp("mv", "mv", awal, "./hasil", NULL);
        }
        else{
            ((wait(&status))>0);
        }
    } else {
        child = fork();
        if(child == 0){
            char awal[100];
            snprintf(awal, sizeof(awal), "./%s/music.txt", message);
            execlp("mv", "mv", awal, "./hasil", NULL);
        }
        else{
            ((wait(&status))>0);
        }
    }
}


int main()
{
    char *nama_user;
    nama_user=(char *)malloc(10*sizeof(char));
    nama_user=getlogin();


    pthread_t threadawal[4];
    char *mkdirQ = "quote";
    char *mkdirM = "music";
    char *unzipQ = "quote.zip";
    char *unzipM = "music.zip";
    int  mkdir1, mkdir2, unzip1, unzip2;;

    mkdir1 = pthread_create( &threadawal[1], NULL, garputunggu_mkdir, (void*) mkdirQ); //membuat thread pertama
    mkdir2 = pthread_create( &threadawal[2], NULL, garputunggu_mkdir, (void*) mkdirM);//membuat thread kedua
    unzip1 = pthread_create( &threadawal[3], NULL, garputunggu_unzip, (void*) unzipQ); //membuat thread pertama
    unzip2 = pthread_create( &threadawal[4], NULL, garputunggu_unzip, (void*) unzipM);//membuat thread kedua

    pthread_join( threadawal[1], NULL);
    pthread_join( threadawal[2], NULL); 
    pthread_join( threadawal[3], NULL);
    pthread_join( threadawal[4], NULL); 


    pthread_t threadFile[2];//inisialisasi awal
    char *fileQ = "quote";
    char *fileM = "music";
    int  file1, file2;

    file1 = pthread_create( &threadFile[1], NULL, garputunggu_file, (void*) fileQ); //membuat thread pertama
    file2 = pthread_create( &threadFile[2], NULL, garputunggu_file, (void*) fileM);//membuat thread kedua

    pthread_join( threadFile[1], NULL);
    pthread_join( threadFile[2], NULL); 

    char *buathasil[] = {"mkdir", "hasil", NULL};
    garputunggu("/bin/mkdir", buathasil);

    pthread_t threadMove[2];//inisialisasi awal
    char *moveQ = "quote";
    char *moveM = "music";
    int  move1, move2;

    move1 = pthread_create( &threadMove[1], NULL, garputunggu_move, (void*) moveQ); //membuat thread pertama
    move2 = pthread_create( &threadMove[2], NULL, garputunggu_move, (void*) moveM);//membuat thread kedua

    pthread_join( threadMove[1], NULL);
    pthread_join( threadMove[2], NULL); 

    char name[100];
    snprintf(name, sizeof(name), "mihinomenest%s", nama_user);
    zip_garputunggu(name, "hasil.zip", "hasil");

    pthread_t ngulang[10];
    char * repothapus = "hasil";
    char * repotzip = "hasil.zip";
    char * repotfile = "hasil";
    int repot1, repot2, repot3, repot4;

    repot1 = pthread_create( &ngulang[1], NULL, garputunggu_hapus_dir, (void*) repothapus); //membuat thread pertama
    repot2 = pthread_create( &ngulang[2], NULL, garputunggu_unzip, (void*) repotzip);//membuat thread kedua
    repot3 = pthread_create( &ngulang[3], NULL, garputunggu_file, (void*) repotfile);
    repot4 = pthread_create( &ngulang[4], NULL, garputunggu_hapus_file, (void*) repotzip);

    pthread_join( ngulang[1], NULL);
    pthread_join( ngulang[2], NULL); 
    pthread_join( ngulang[3], NULL); 
    pthread_join( ngulang[4], NULL); 

    char name2[100];
    snprintf(name2, sizeof(name2), "mihinomenest%s", nama_user);
    zip_garputunggu(name2, "hasil.zip", "hasil");

    exit(EXIT_SUCCESS);
}



