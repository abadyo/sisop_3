#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

void send_file(FILE *fp, int sock) {
    char data[1024];

    while(fgets(data, 1024, fp) != NULL) {
        if((send(sock, data, 100, 0)) == -1) {
            printf("error file transfer\n");
            exit(1);
        }
        bzero(data, 1024);
    }
}


int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    // char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // a
    while(1) {
        char awal1[100] = {0};
        char username[100];
        char password[100];
        printf("Masuk or Daftar?!\n");
        printf("1. Masuk\n2. Daftar\n>>> ");
        scanf("%s", awal1);
        send(sock , awal1 , 100, 0 );

        printf("Loading.....\n");
        read( sock , buffer, 1024);
        if(strcmp(buffer, "M") == 0) {
            printf("Hello\n");
            printf("Username: ");
            scanf("%s", username);
            send(sock , username , 100, 0 );

            printf("Password: ");
            scanf("%s", password);
            send(sock , password , 100, 0 );

            read( sock , buffer, 1024);
            if(strcmp(buffer, "success") == 0) {
                printf("\n\nWelcome\n");
                char perintah[100];
                char judul[100];
                char deskripsi[100];
                char input[100];
                char output[100];
                scanf("%s", perintah);
                send(sock , perintah , 100, 0 );
                read( sock , buffer, 1024);
                if(strcmp(buffer, "doAdd") == 0) {
                    printf("judul: ");
                    scanf("%s", judul);
                    send(sock , judul , 100, 0 );

                    printf("deskripsi(directory): ");
                    scanf("%s", deskripsi);

                    // printf("input(directory): ");
                    // scanf("%s", input);
                    // send(sock , input , 100, 0 );

                    // printf("output(directory): ");
                    // scanf("%s", output);
                    // send(sock , output , 100, 0 );

                    // judul

                    // deskripsi
                    FILE *file_d = fopen("tes.txt", "r");
                    char line[99];
                    sleep(2);
                    send_file(file_d, sock);

                    
                }

            } else {
                printf("Sorry, wrong username/password\n");
            }
        } else if(strcmp(buffer, "D") == 0) {
            printf("Username: ");
            scanf("%s", username);
            send(sock , username , 100, 0 );

            printf("Password: ");
            scanf("%s", password);
            send(sock , password , 100, 0 );
            
            read(sock, buffer, 1024);
            if(strcmp(buffer, "allow") == 0) {
                printf("Selamat datang!\n\n");
            } else if(strcmp(buffer, "denied_exist") == 0){
                printf("Gagal daftar: Sudah ada pengguna dengan username itu.\n\n");
            } else {
                printf("Gagal daftar: Password harus lebih dari 6 karakter dan Alphanumeric Upper/lower\n\n");
            }
        }
        

    }
   
    return 0;
}