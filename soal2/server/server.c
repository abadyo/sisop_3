#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#define PORT 8080

void write_file(char *nama, int sock) {
    int n;
    char buffer[1024];
    FILE *fp = fopen(nama, "a+");
    if(fp == NULL) printf("error no file\n");

    while(1) {
        n = recv(sock, buffer, 1024, 0);
        if(n <= 0) {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, 1024);
    }
    return;
}

bool is_mixed(char* str) {

    int   i;
    bool  found_lower = false, found_upper = false;

    for (int i = 0; str[i] != '\0'; i++) {
        found_lower = found_lower || (str[i] >= 'a' && str[i] <= 'z');
        found_upper = found_upper || (str[i] >= 'A' && str[i] <= 'Z');

        if (found_lower && found_upper) break;
    }

    return (found_lower && found_upper);

}


int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }


    printf("Connected to port %d\n", PORT);

    if(access( "problem.tsv", F_OK ) != 0) {
        FILE *tsv = fopen("problem.tsv", "a+");
        fprintf(tsv, "Judul\tAuthor\n");
        fclose(tsv);
    }
    
    while(1) {
        char username[1024] = {0};
        char password[1024] = {0};
        char terdaftar[3000];
        
        
        read( new_socket , buffer, 1024);
        if(strcmp(buffer, "Masuk") == 0) {
            printf("Proses Masuk dijalankan\n");
            send(new_socket , "M" , 100 , 0 );

            read( new_socket , username, 1024);
            // printf("Username: %s", username);
            
            read( new_socket , password, 1024);
            // printf("Password: %s", password);
            
            FILE *file_terdaftar = fopen("akun.txt", "r");
            char line[200];
            while(fgets(line, sizeof(line), file_terdaftar)) {
                char *pisah = strtok(line, ":");
                printf("%s\n", line);
                if(strcmp(pisah, username) == 0) {
                    pisah = strtok(NULL, ":");
                    printf("%s\n", pisah);
                    pisah[strcspn(pisah, "\n")] = 0;
                    if(strcmp(pisah, password) == 0) {
                        send(new_socket , "success" , 100 , 0 );
                        break;
                    } else  {
                        send(new_socket , "error" , 100 , 0 );
                        break;
                    }
                } 
                }
            
            fclose(file_terdaftar);
            read( new_socket , buffer, 1024);

            char judul[100];
            char deskripsi[100];
            char input[100];
            char output[100];
            if(strcmp(buffer, "add") == 0) {
                send(new_socket , "doAdd" , 100 , 0 );
                
                // read perintah add
                read( new_socket , judul, 1024);
                printf("%s\n", judul);
                mkdir(judul, 0777);
                char buat_d[1000];
                snprintf(buat_d, sizeof(buat_d), "%s/description.txt", judul);
                printf("%s", buat_d);
                write_file(buat_d, new_socket);
            }

        } else if(strcmp(buffer, "Daftar") == 0) {
            printf("Proses Daftar dijalankan\n");
            send(new_socket , "D" , 100 , 0 );

            read( new_socket , username, 1024);
            // printf("Username: %s", username);
            
            read( new_socket , password, 1024);
            // printf("Password: %s", password);

            FILE *file_terdaftar = fopen("akun.txt", "a+");
            char line[200];
            int check = 0;
            while(fgets(line, sizeof(line), file_terdaftar)) {
                // char terpilih[1000];
                char *pisah = strtok(line, ":");
                // printf("%s \n", pisah);
                if(!is_mixed(password) || strlen(password) < 6) {
                    send(new_socket , "denied_pass_term" , 100 , 0 );
                    check = 1;
                    break;
                }
                if(strcmp(pisah, username) == 0) {
                    send(new_socket , "denied_exist" , 100 , 0 );
                    check = 1;
                    break;
                }
                
            }
            if(check == 0) {
                fprintf(file_terdaftar, "%s:%s\n", username, password);
                send(new_socket , "allow" , 100 , 0 );
            }
            fclose(file_terdaftar);
            

        } else {
            printf("Input Salah!\n");
        }
        // printf("%s", terdaftar);
        // send(new_socket , hello , strlen(hello) , 0 );
        // printf("Hello message sent\n");
    }
    
    return 0;
}
