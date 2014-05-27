/*
 *	$Source: /afs/sipb.mit.edu/project/sipbsrc/src/chat/RCS/chatmaster.c,v $
 *	$Header: /afs/sipb.mit.edu/project/sipbsrc/src/chat/RCS/chatmaster.c,v 1.11 92/03/03 02:57:28 jik Exp $
 */

#ifndef lint
static char *rcsid_chatmaster_c = "$Header: /afs/sipb.mit.edu/project/sipbsrc/src/chat/RCS/chatmaster.c,v 1.11 92/03/03 02:57:28 jik Exp $";
#endif lint

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#ifdef _AIX
#include <sys/select.h>
#endif

#include "messagecodes.h"
#include "chat.h"
#include "game_data.h"

/* There is no real reason we should limit the number of players,
   (except that we'll be tieing up the net), so make it big. */
#define MAX_PLAYERS 100
#define BUFLEN 4096

char *game_data_file = GAME_DATA_FILE;

struct player {
    int socket;
    char name[BUFLEN];
};

int num_of_players;
struct player players[MAX_PLAYERS];

static unsigned long nlen;

initialize_game()
{
    extern int num_of_players;

    num_of_players = 0;
}


int new_player_is_ok()
{
    extern int num_of_players;

    return(num_of_players < MAX_PLAYERS);
}


new_player(sock,M_max_fd,M_select_mask)
int sock, M_max_fd;
fd_set M_select_mask;
{
    extern struct player players[];
    extern int num_of_players,votes,novotes;

    int len,readfds,num,fd,sock1;
    static char buf[BUFLEN];
    register int i;
    register char *s;

    /* The player program is required to send us the player's name right
       away.  Like all other messages, it consists of an integer length
       followed by that many characters. */

    if (safe_read(sock, &nlen, sizeof(nlen)) < 0) {
	get_rid_of_player(sock);
	return;
    }
    len = ntohl(nlen);
    if (safe_read(sock, buf, len) < 0) {
	get_rid_of_player(sock);
	return;
    }
    buf[len] = '\0';

    sock1 = sock;

    /* Copy the name into a more permenant location. */
    s = (char *)malloc(len + 1);
    strcpy(s, buf);

    players[num_of_players].socket = sock;
    strcpy(players[num_of_players].name, s);


    /* Let the player know who else is talking. */
    switch (num_of_players) {
	case 0:
	    strcpy(buf, "Chatmaster: You are alone.\n");
	    break;
	case 1:
	    sprintf(buf, "Chatmaster: You are joining %s.\n", players[0].name);
	    break;
	case 2:
	    sprintf(buf, "Chatmaster: You are joining %s and %s.\n",
	        players[0].name, players[1].name);
	    break;
	default:
	    strcpy(buf, "Chatmaster: You are joining ");
	    for (i = 1; i < num_of_players; i++) {
		strcat(buf, players[i].name);
		strcat(buf, ", ");
	    }
	    strcat(buf, "and ");
	    strcat(buf, players[0].name);
	    strcat(buf, ".\n");
	    break;
    }
    len = strlen(buf);
    nlen = htonl(len);
    write(sock1, &nlen, sizeof(nlen));
    write(sock1, buf, len);

    /* Construct the message to send to the other players. */
    sprintf(buf, "\nChatmaster: %s is joining the conversation.", s);
    len = strlen(buf);
    nlen = htonl(len);

    /* Send it. */
    for (i = 0; i < num_of_players; i++) {
	sock = players[i].socket;
	write(sock, &nlen, sizeof(nlen));
	write(sock, buf, len);
    }

    /* Make the new player "official". */
    num_of_players++;
}


int query_response(response)
char **response;
{
    extern int num_of_players;
    extern struct player players[];
    register int i;
    static char buf[BUFLEN];
    register char *ptr1, *ptr2;

    ptr1 = buf;
    for (i = 0; i < num_of_players; i++) {
	for (ptr2 = players[i].name; *ptr2; ptr2++) {
	    *ptr1++ = *ptr2;
	}
	*ptr1++ = ' ';
    }
    *response = buf;
    return(ptr1 - buf);
}


listen_to_player(sock)
int sock;
{
    extern struct player players[];
    extern int num_of_players,novotes,votes;

    int len;
    static char buf[BUFLEN], msg[BUFLEN];
    register int i,j;

    if (safe_read(sock, &nlen, sizeof(nlen)) < 0) {
	get_rid_of_player(sock);
	return;
    }
    len = ntohl(nlen);
    switch (len) {
    	case MC_QUERY_GAME:
	    sprintf(buf, "\nChatmaster: You are connected to:");
	    for (i=0; i < num_of_players; i++) {
	    	strcat(buf, " ");
		strcat(buf,players[i].name);
	    }
	    len = strlen(buf);
	    nlen = htonl(len);
	    write(sock, &nlen, sizeof(nlen));
	    write(sock, buf, len);
	    break;
	case MC_NEW_NAME:
	    if (safe_read(sock, &nlen, sizeof(nlen)) < 0) {
		get_rid_of_player(sock);
		return;
	    }
	    len = ntohl(nlen);
	    if (safe_read(sock, buf, len) < 0) {
		get_rid_of_player(sock);
		return;
	    }   
	    buf[len] = '\0';
	    for (i=0; i < num_of_players; i++) 
		if (players[i].socket == sock) {
		    sprintf(msg, "\nChatmaster: \7%s's new name is %s", players[i].name,buf);
		    /* note we cannot just assign the pointer since
		       buf is reused */
		    strcpy(players[i].name, buf);
		    len = strlen(msg);
		    nlen = htonl(len);
		    for (j = 0; j < num_of_players; j++) {
			write(players[j].socket, &nlen, sizeof(nlen));
			write(players[j].socket, msg, len);
		    }
		break;
		}
	    break;
	default:
	    if (safe_read(sock, buf, len) < 0) {
		get_rid_of_player(sock);
		return;
	    }   
	    for (i = 0; i < num_of_players; i++) {
	    	write(players[i].socket, &nlen, sizeof(nlen));
		write(players[i].socket, buf, len);
	    }
	}
}


get_rid_of_player(sock)
int sock;
{
    extern int num_of_players;
    extern struct player players[];

    static char buf[BUFLEN];
    register int i;
    int len;

    for (i = 0; i < num_of_players; i++) {
        if (players[i].socket == sock) {
	    /* Construct the message to send to the other players. */
	    sprintf(buf, "\nChatmaster: %s has left the conversation.", players[i].name);
	    len = strlen(buf);
	    nlen = htonl(len);

	    /* Deallocate the resources used for the player. */
	    free(players[i].name);
	    num_of_players--;
            players[i] = players[num_of_players];
	    remove_player(sock);

	    for (i = 0; i < num_of_players; i++) {
		sock = players[i].socket;
		write(sock, &nlen, sizeof(nlen));
		write(sock, buf, len);
	    }
	    break;
        }
    }
}
