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
    char name[1025];
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

void insert(int fd, int playing)
{
    struct player *new, **p;
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

    for (p = &playerlist; *p; p = &(*p)->next);
    new->next = *p;
    *p = new;
}

void printall()
{
    struct player *p;
    for (p = playerlist; p; p = p->next)
        printf("%d: %d\n", p->fd, p->playing);
    printf("[end]\n");
}

int is_space(char *s) {
  while (*s != '\0') {
    if (!isspace(*s))
      return 0;
    s++;
  }
  return 1;
}

int is_valid(char *s) {
    struct player *p;
    // Empty line, invalid name
    if (is_space(s)) return 2;
    // Check players for no duplicate names
    for (p = playerlist; p; p = p->next) {
        if (strcmp(p->name, s) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    struct player *p, *next;
    fd_set readfds;
    char msg[MAXNAME + 50];
    int server_socket, new_socket;
    int max_fd, size, activity, valread;
    struct sockaddr_in r;
    char buf[1025];
    char welcome[50] = "Welcome to Mancala. What is your name?\n";
    char message[1001];
    int val, turn = -1;
    int lastchar;

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

        for (p = playerlist; p; p = p->next) {
            // add child sockets to set
            if (p->fd > 0) FD_SET(p->fd, &readfds);
            // highest file descriptor number, needed for select
            if (p->fd > max_fd) max_fd = p->fd;
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
            if (send(new_socket, welcome, strlen(welcome), 0) != strlen(welcome))
                perror("send");
            // Create new player and add to end of linked list
            // Currently not playing, has to input their name but is connected
            if (playerlist == NULL)
                turn = new_socket;
            insert(new_socket, 0);
            continue;
        }
        // IO operation on other socket
        printall();
        for (p = playerlist; p; p = p->next) {
            if (FD_ISSET(p->fd, &readfds)) {
                //printall();
                if ((valread = read(p->fd, buf, 1024)) == 0) {
                    //Somebody disconnected , get his details and print
                    getpeername(p->fd , (struct sockaddr*)&r , (socklen_t*)&size);
                    printf("disconnecting client ip %s\n" , inet_ntoa(r.sin_addr));
                    strcpy(message, p->name);
                    strcat(message, " has left the game.\n");
                    broadcast(message);
                    //Close the socket and mark as 0 in list for reuse
                    next = p->next;
                    // Change players move
                    if (turn == p->fd) {
                        if (next != NULL) {
                            turn = next->fd;
                        } else if (playerlist != NULL) {
                            turn = playerlist->fd;
                        } else {   // Probably don't need this line but whatever
                            turn = -1;
                            printf("No one's turn\n");
                        }
                    }
                    close(p->fd);
                    delete(p->fd);
                }
                buf[valread] = '\0';
                // printf("BUF is %s\n", buf);
                // Player isn't playing and needs to put in a name first
                if (!(p->playing)) {
                    // Make newline terminating zero byte. Don't want two newlines
                    lastchar = buf[valread-1];
                    buf[valread-1] = '\0';
                    val = is_valid(buf);
                    printf("val = %d\n", val);
                    // Duplicate name
                    if (val == 1) {
                        printf("rejecting duplicate name %s from %s\n", buf, inet_ntoa(r.sin_addr));
                        send(p->fd, "Sorry, someone else already has that name. Please choose another\n\n", 65, 0);
                        break;
                    // Empty name
                    } else if (val == 2){
                        printf("rejecting empty name\n");
                        send(p->fd, "What is your name?\n", 19, 0);
                        break;
                    // Valid name
                    } else if (lastchar == '\r' || lastchar == '\n') {
                        p->playing = 1;
                        strcpy(p->name, buf);
                        strcpy(message, buf);
                        strcat(message, " has joined the game\n");
                        printf("%s's name is set to %s\n", inet_ntoa(r.sin_addr), p->name);
                        broadcast(message);
                    }
                // Player is playing, actual game content here
                } else {


                    //printf("My FD is %d\n", p->fd);
                }
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
    struct player *p;
    for (p = playerlist; p; p = p->next)
        send(p->fd, s, strlen(s), 0);
}

void broadcast_playing(char *s) {
    struct player *p;
    for (p = playerlist; p; p = p->next) {
        if (p->playing == 0) continue;
        send(p->fd, s, strlen(s), 0);
    }
}

char * print_board() {

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
