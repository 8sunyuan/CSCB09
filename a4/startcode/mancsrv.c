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
    char name[MAXNAME];
    struct player *next;
} *playerlist = NULL;

extern void parseargs(int argc, char **argv);
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
    struct player *new;
    int i, pebbles = compute_average_pebbles();
    /* create the new item */
    if ((new = malloc(sizeof(struct player))) == NULL) {
        fprintf(stderr, "out of memory!\n");  /* unlikely */
        exit(1);
    }
    strcpy(new->name, "");
    new->fd = fd;
    new->playing = playing;
    // Set pebbles for each pit
    for (i = 0; i < NPITS; i++)
        new->pits[i] = pebbles;
    new->pits[NPITS] = 0;
    //Insert at head of list
    new->next = playerlist;
    playerlist = new;
}

void printall()
{
    struct player *p;
    for (p = playerlist; p; p = p->next)
        printf("NAME: %s (%d: %d)\n", p->name, p->fd, p->playing);
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

void print_board(int turn) {
    struct player *p = playerlist;
    char message[1025] = "";
    char move[MAXNAME + 20];
    // Get string for board
    for (p = playerlist; p; p = p->next) {
        if (p->playing) {
            sprintf(message, "%s%s:  ", message, p->name);
            for (int i = 0; i <= NPITS; i++) {
                if (i < NPITS)
                    sprintf(message, "%s[%d]", message, i);
                else
                    sprintf(message, "%s [end pit]", message);
                sprintf(message, "%s%d ", message, p->pits[i]);
            }
            strcat(message, "\n");
        }
        if (turn == p->fd)
            sprintf(move, "It is %s's turn\n", p->name);
    }
    // Send board message to players
    for (p = playerlist; p; p = p->next) {
        if (p->playing == 0) continue;
        send(p->fd, message, strlen(message), 0);
        if (turn != -1) {
            if (turn == p->fd)
                send(p->fd, "Your move\n", 11, 0);
            else
                send(p->fd, move, strlen(move), 0);
        }
    }
}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;
  // Write new null terminator
  *(end+1) = 0;

  return str;
}

int next_turn(struct player *p, int turn, int disconnect) {
    struct player *pp;
    pp = p;
    do
        pp = (pp->next) ? pp->next : playerlist;
    while(!pp->playing);
    // Went through whole list and no one else playing
    // if player p is disconnecting then it's no one's turn
    if (disconnect && pp->fd == turn) {
        printf("No one's turn\n");
        turn = -1;
    } else {
        turn = pp->fd;
    }
    return turn;
}



int main(int argc, char **argv)
{
    struct player *p, *q;
    fd_set readfds;
    char msg[MAXNAME + 50];
    int server_socket, new_socket;
    int max_fd, size, activity, valread;
    struct sockaddr_in r;
    char buf[1025];
    char welcome[50] = "Welcome to Mancala. What is your name?\n";
    char message[1001];
    int valid_name, pitnum, pebbles, turn = -1;
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

    if (listen(server_socket, 50)) {
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
            insert(new_socket, 0);
            continue;
        }

        // IO operation on other socket
        for (p = playerlist; p; p = p->next) {
            if (FD_ISSET(p->fd, &readfds)) {
                if ((valread = read(p->fd, buf, 1024)) == 0) {
                    //Somebody disconnected , get his details and print
                    getpeername(p->fd , (struct sockaddr*)&r , (socklen_t*)&size);
                    printf("disconnecting client ip %s\n" , inet_ntoa(r.sin_addr));
                    sprintf(message, "%s has left the game\n", p->name);
                    broadcast(message);
                    // Current turn is for player disconnecting. Change turn before
                    // removing player from game
                    if (p->fd == turn && ((turn = next_turn(p, turn, 1)) != -1)) {
                        printf("%d\n", turn);
                        for (q = playerlist; q; q = q->next) {
                            if (q->fd == turn)
                                sprintf(message, "It is %s's turn\n", q->name);
                        }
                        print_board(turn);
                        broadcast(message);
                    }
                    close(p->fd);
                    delete(p->fd);
                    break;
                }
                buf[valread] = '\0';
                if (!(p->playing)) { // Player isn't playing and needs to put in a name first
                    // Make newline terminating zero byte. Don't want two newlines
                    lastchar = buf[valread-1];
                    strcat(p->name, buf);
                    if (lastchar == '\r' || lastchar == '\n') {
                        char *newline;
                        if ((newline = strchr(p->name, lastchar)))
                            *newline = '\0';
                        valid_name = is_valid(buf);
                        // Duplicate name
                        if (valid_name == 1) {
                            printf("rejecting duplicate name %s from %s\n", buf, inet_ntoa(r.sin_addr));
                            send(p->fd, "Sorry, someone else already has that name. Please choose another\n\n", 65, 0);
                            memset(p->name, '\0', strlen(p->name));
                            break;
                        // Empty name
                        } else if (valid_name == 2){
                            printf("rejecting empty name\n");
                            send(p->fd, "What is your name?\n", 19, 0);
                            memset(p->name, '\0', strlen(p->name));
                            break;
                        // Valid name
                        } else {
                            // Player has not fully entered his name until the
                            // message ends with a newline
                            printf("%s's name is set to %s\n", inet_ntoa(r.sin_addr), p->name);
                            sprintf(message, "%s has joined the game\n", p->name);
                            broadcast(message);
                            p->playing = 1;
                            if (turn == -1)
                                turn = p->fd;
                            print_board(turn);
                        }
                    }
                } else { // Player is playing, actual game content here
                    // Not the players move
                    if (turn != p->fd) {
                        send(p->fd, "It is not your move.\n", 22, 0);
                        break;
                    }

                    if ((pitnum = strtol(buf, NULL, 10)) == 0 && buf[0] != '0')
                        break;

                    if (pitnum >= 0 && pitnum <= NPITS - 1) {
                        pebbles = p->pits[pitnum];
                        if (pebbles == 0) {
                            send(p->fd, "Your pit is empty. Try again\n", 30, 0);
                            break;
                        }
                        p->pits[pitnum] = 0;
                        pitnum++;
                        q = p;
                        // Distribute pebbles to adjacent pits
                        while (pebbles != 0) {
                            while (pitnum <= NPITS && q->playing) {
                                // Don't add to end pit unless its own players pit
                                q->pits[pitnum]++;
                                if (--pebbles == 0) break;
                                pitnum++;
                            }
                            // Ensure last pebble doesn't go into opponents last
                            if (q->fd != turn && pitnum == NPITS + 1) {
                                pebbles++;
                                q->pits[pitnum-1]--;
                            }
                            // Reset index at 0 and find adjacent player
                            if (pebbles != 0) {
                                pitnum = 0;
                                do {
                                    q = (q->next) ? q->next : playerlist;
                                } while(!q->playing);
                            }
                        }
                        // If last pebble landed in players own last pit
                        // don't change turns
                        if (pitnum != NPITS || q->fd != turn)
                            turn = next_turn(p, turn, 0);
                        print_board(turn);
                    } else {
                        sprintf(message, "Pit numbers go from 0 to %d\n", NPITS - 1);
                        send(p->fd, message, strlen(message), 0);
                    }
                }
            }
        }
    }

    broadcast("Game over!\r\n");
    printf("Game over!\n");
    print_board(-1);
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
