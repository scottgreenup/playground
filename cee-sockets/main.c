#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

/*
 * A game server requires 
 *
 */

void run_server(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)  {
        perror("ERROR opening socket");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
    }
    listen(sockfd,5);
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    int newsockfd = accept(
        sockfd, 
        (struct sockaddr *) &cli_addr, 
        &clilen
    );
    if (newsockfd < 0) {
        perror("ERROR on accept");
    }
    char buffer[256] = {0};
    int n = read(newsockfd,buffer,255);
    if (n < 0) { 
        perror("ERROR reading from socket");
    }

    printf("Here is the message: %s\n",buffer);

    write(newsockfd,"I got your message",18);

    if (n < 0) {
        perror("ERROR writing to socket");
    }

    close(newsockfd);
    close(sockfd);
}

void run_client(char* hostname, int port) {

    printf("hostname: %s; port: %d\n", hostname, port);

    char buffer[256];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
    }

    struct hostent* server;
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }


    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    memcpy(
        &serv_addr.sin_addr.s_addr,
        server->h_addr,
        server->h_length
    );

    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
    }

    printf("Please enter the message: ");
    memset(&buffer, 0, 256);
    fgets(buffer, 255, stdin);

    if (write(sockfd, buffer, strlen(buffer)) < 0) {
         perror("ERROR writing to socket");
    }

    memset(&buffer, 0, 256);
    if (read(sockfd, buffer, 255) < 0) {
         perror("ERROR reading from socket");
    }

    printf("%s\n",buffer);
    close(sockfd);
}

/*

Usage: [mode] [OPTIONS...]
    
    mode:
        server - run as a server
        client - run as a client

    server options:
        [port]

    client options:
        hostname port

*/
int main(int argc, char* argv[]) {

    static int server_flag = 0;
    static int client_flag = 0;
    static int port_flag = 0;
    static int host_flag = 0;

    int port;
    char hostname[255];

    while (1) {
        static struct option long_options[] = {
            /* options that set a flag */
            {"server",   no_argument,       &server_flag, 's'},
            {"client",   no_argument,       &client_flag, 'c'},
            
            /* these options do not set a flag. */
            {"port",     required_argument, &port_flag, 'p'},
            {"hostname", required_argument, &host_flag, 'h'},
            {0, 0, 0, 0}

        };

        int option_index = 0;

        int c = getopt_long(
            argc,
            argv,
            "scp:h:",
            long_options,
            &option_index
        );

        if (c == -1) {
            break;
        }

        switch (c) {
        case 0:
            if (long_options[option_index].flag != 0) {
                break;
            }
            /*
            // long_options[option_index].name // the name "server" etc
            // optarg // the argument with the flag
            */
            break;
        case 's':
            server_flag = 1;
            break;
        case 'c':
            client_flag = 1;
            break;
        case 'p':
            port = atoi(optarg);
            port_flag = 1;
            break;
        case 'h':
            strcpy(hostname, optarg);
            host_flag = 1;
            break;
        default:
            exit(1);
        }
    }

    if (optind < argc) {
        printf("Unknown flags:\n");
        while (optind < argc) {
            printf("  %s\n", argv[optind++]);
        }
        exit(1);
    }


    if (server_flag && port_flag) {
        run_server(port);
    } else if (client_flag && port_flag && host_flag) {
        run_client(hostname, port);
    } else {
        printf("Missing arguments\n");
        exit(1);
    }

    return 0; 
}

