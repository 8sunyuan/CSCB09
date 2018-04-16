#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>



int port = 3000;
int listenfd;

#define MAXNAME 80  /* maximum permitted name size, not including \0 */

#define NPITS 6  /* number of pits on a side, not including the end pit */

struct player {
    int fd;
    int pits[NPITS+1];
    int playing;
    char *name;
    struct player *next;
} *playerlist = NULL;

extern void parseargs(int argc, char **argv);
extern void makelistener();

extern int compute_average_pebbles();
extern int game_is_over();  /* boolean */
extern void broadcast(char *s);

void delete(int fd)
{
    struct player **pp;

    /* find the (struct item *) which points to the item to be deleted */
    for (pp = &playerlist; *pp && (*pp)->fd != fd; pp = &(*pp)->next)
        ;
    if (*pp && (*pp)->fd == fd) {
        struct player *old = *pp;
        *pp = (*pp)->next;
        free(old);
    }
}

void insert(int fd, int playing, struct player *last)
{
    struct player *new;
    int i, pebbles = compute_average_pebbles();
    /* create the new item */
    if ((new = malloc(sizeof(struct player))) == NULL) {
        fprintf(stderr, "out of memory!\n");  /* unlikely */
        exit(1);
    }
    new->fd = fd;
    new->playing = playing;
    for (i = 0; i < NPITS; i++)
        new->pits[i] = pebbles;

    if (playerlist) {
        /* link it in */
        new->next = playerlist;
        last->next = new;
    } else {
        playerlist = new;
        new->next = new;
    }
}

int main(int argc, char **argv)
{
    struct player *p;
    struct player *last;
    fd_set readfds;
    char msg[MAXNAME + 50];
    int server_socket, new_socket;
    int max_fd, size, activity, valread;
    struct sockaddr_in r;
    char buf[1025];
    char message[50] = "Welcome to Mancala.  What is your name?\n";

    parseargs(argc, argv);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    	perror("socket");
    	exit(1);
    }

    memset(&r, '\0', sizeof r);
    r.sin_family = AF_INET;
    r.sin_addr.s_addr = INADDR_ANY;
    r.sin_port = htons(port);
    if (bind(server_socket, (struct sockaddr *)&r, sizeof r)) {
    	perror("bind");
    	exit(1);
    };

    if (listen(server_socket, 5)) {
    	perror("listen");
    	exit(1);
    }

    while (!game_is_over()) {
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);
        max_fd = server_socket;
        size = sizeof r;
        p = playerlist;
        last = playerlist;

        if (p) {
            if (p->fd > 0) FD_SET(p->fd, &readfds);
            if (p->fd > max_fd) max_fd = p->fd;
            p = p->next;
            while (p != playerlist) {
                // add child sockets to set
                if (p->fd > 0) FD_SET(p->fd, &readfds);
                // highest file descriptor number, needed for select
                if (p->fd > max_fd) max_fd =  p->fd;
                last = p;
                p = p->next;
            }
        }

        activity = select(max_fd + 1 , &readfds , NULL , NULL , NULL);
        if ((activity < 0) && (errno != EINTR))
            printf("select error");

        // Incoming connection on the server socket
        if (FD_ISSET(server_socket, &readfds)) {
            if ((new_socket = accept(server_socket, (struct sockaddr*)&r, (socklen_t*)&size)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            // Print new connection on server
            printf("connection from %s\n", inet_ntoa(r.sin_addr));

            // Send welcome message to new player
            if (send(new_socket, message, strlen(message), 0) != strlen(message))
                perror("send");

            // Create new player and add to end of linked list
            // Currently not playing, has to input their name but is connected
            insert(new_socket, 0, last);
            continue;
        }

        // IO operation on other socket
        for (p = playerlist->next; p != last; p = p->next) {
            if (FD_ISSET(p->fd, &readfds)) {
                if ((valread = read(p->fd, buf, 1024)) == 0) {
                    //Somebody disconnected , get his details and print
                    getpeername(p->fd , (struct sockaddr*)&r , (socklen_t*)&size);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(r.sin_addr) , ntohs(r.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close(p->fd);
                } else {
                    buf[valread] = '\0';
                    send(p->fd, buf, strlen(buf), 0);
                }
                // Linkedlist shit

                // if !p->joined
                // read name and store in p->name;

                // else if p->joined
                    // If move = p->fd
                    // read index and must be valid from
                    // else print
                    // gtfo just to him

            }
        }

    }

    broadcast("Game over!\r\n");
    printf("Game over!\n");
        for (p = playerlist; p; p = p->next) {
    	int points, i;
    	for (points = i = 0; i <= NPITS; i++)
    	    points += p->pits[i];
    	printf("%s has %d points\r\n", p->name, points);
    	snprintf(msg, sizeof msg, "%s has %d points\r\n", p->name, points);
    	broadcast(msg);
    }

    return(0);
}

void broadcast(char *s) {






}


void parseargs(int argc, char **argv)
{
    int c, status = 0;
    while ((c = getopt(argc, argv, "p:")) != EOF) {
    	switch (c) {
    	case 'p':
    	    port = atoi(optarg);
    	    break;
    	default:
    	    status++;
    	}
    }

    if (status || optind != argc) {
	   fprintf(stderr, "usage: %s [-p port]\n", argv[0]);
	   exit(1);
    }
}


void makelistener()
{
    struct sockaddr_in r;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    	perror("socket");
    	exit(1);
    }

    memset(&r, '\0', sizeof r);
    r.sin_family = AF_INET;
    r.sin_addr.s_addr = INADDR_ANY;
    r.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr *)&r, sizeof r)) {
    	perror("bind");
    	exit(1);
    };

    if (listen(listenfd, 5)) {
    	perror("listen");
    	exit(1);
    }
}




int compute_average_pebbles()  /* call this BEFORE linking the new player in to the list */
{
    struct player *p;
    int i;

    if (playerlist == NULL)
	return(4);

    int nplayers = 0, npebbles = 0;
    for (p = playerlist; p; p = p->next) {
        nplayers++;
        for (i = 0; i < NPITS; i++)
            npebbles += p->pits[i];
    }
    return((npebbles - 1) / nplayers / NPITS + 1);  /* round up */
}


int game_is_over() /* boolean */
{
    struct player *p;
    int i;
    if (!playerlist)
	return(0);  /* we haven't even started yet! */
    for (p = playerlist; p; p = p->next) {
	    int is_all_empty = 1;
	    for (i = 0; i < NPITS; i++)
	        if (p->pits[i])
		        is_all_empty = 0;
	    if (is_all_empty)
	        return(1);
    }
    return(0);
}
