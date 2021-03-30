#include<netinet/in.h>    
#include<stdio.h>    
#include<stdlib.h>    
#include<sys/socket.h>    
#include<sys/stat.h>    
#include<sys/types.h>    
#include<unistd.h>    
#include<pthread.h>
#include "server.h"
socklen_t addrlen;    
int bufsize = 1024;    
char *buffer;    
struct sockaddr_in address;    
pthread_t threads[2];
plant plants[4];

void* arduino_handler(void* socket_);
void* app_handler(void* socket_);
void update_water_timer(int plant, int interval);
void update_light_timer(int plant, int interval);

int main() {    
   int create_socket, new_socket;    
   buffer = malloc(bufsize); 
   for (int i = 0; i < 4; i++) {
      plants[i].light = -1;
      plants[i].water = -1;
   }
 
   if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0){    
      printf("The socket was created\n");
   }
    
   address.sin_family = AF_INET;    
   address.sin_addr.s_addr = INADDR_ANY;    
   address.sin_port = htons(15000);    
    
   if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == 0){    
      printf("Binding Socket\n");
   }
    
    
   while (1) {    
      if (listen(create_socket, 10) < 0) {    
         perror("server: listen");    
         exit(1);    
      }    
    
      if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0) {    
         perror("server: accept");    
         exit(1);    
      }    
    
      if (new_socket > 0){    
         printf("The Client is connected...\n");
         recv(new_socket, buffer, bufsize, 1);   
         if (buffer[0] == 'i') {
            pthread_create(&threads[0], NULL, app_handler, (void*)&new_socket);
         } else if (buffer[0] == 'a') {
            pthread_create(&threads[0], NULL, arduino_handler, (void*)&new_socket);
         }
      }
        
   }    
   close(create_socket);    
   return 0;    
}

void* arduino_handler(void* socket_) {
   int socket = *(int*)socket_;
   while (1) {
      recv(socket, buffer, bufsize, 5);
      if (buffer[0] == 'w') {
         plant p = plants[(int)buffer[1]];
         p.water = (int) buffer[2];
      } else if (buffer[0] == 'l') {
         plant p = plants[(int)buffer[1]];
         p.light = (int) buffer[2];
      } else if (buffer[0] == 'c') {

      }
   }
}

void* app_handler(void* socket_) {
   int socket = *(int*)socket_;
   while (1) {
      recv(socket, buffer, bufsize, 5);
   }
}

void update_water_timer(int plant, int interval) {

}
void update_light_timer(int plant, int interval) {

}