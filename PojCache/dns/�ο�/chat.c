/*
 *	$Source: /mit/jik/sipbsrc/src/chat/RCS/chat.c,v $
 *	$Header: /mit/jik/sipbsrc/src/chat/RCS/chat.c,v 1.18 1992/03/03 03:07:31 jik Exp $
 */

#ifndef lint
static char *rcsid_chat_c = "$Header: /mit/jik/sipbsrc/src/chat/RCS/chat.c,v 1.18 1992/03/03 03:07:31 jik Exp $";
#endif lint

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <strings.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <curses.h>
#include <pwd.h>
#ifdef _AIX
#include <sys/select.h>
#endif

#include "messagecodes.h"
#include "game_data.h"
#include "user.h"
#include "chat.h"

typedef struct xwin {
	WINDOW *x_win;
	int x_nlines;
	int x_ncols;
	int x_line;
	int x_col;
	char kill;
	char cerase;
	char werase;
} xwin_t;

int die();

static unsigned long nlen;		/* for network byte order */

char *game_data_file = GAME_DATA_FILE;

main()
{
    extern int errno;
    struct game_data gamedat;
    int server, game, len;
    struct passwd *pw;
    char *name, *hostname[MAX_NAME_LENGTH], namebuf[MAX_NAME_LENGTH];
    if (get_game_data(&gamedat, "chat") < 0) {
	fprintf(stderr, "chat: couldn't get game data\n");
	exit(1);
    }

    server = get_socket_to_gameserver_for_game("chat");
    if (server < 0) {
	switch (server) {
	    case -2:
	    	fprintf(stderr, "chat: this machine is not supported\n");
	    	break;
	    case -3:
	    	fprintf(stderr, "chat: couldn't connect to gameserver\n");
	        break;
	    default:
	    	fprintf(stderr, "chat: couldn't get socket to gameserver\n");
	}
	exit(1);
    }

    /* He'll expect us to send him our name. */
    pw = getpwuid(getuid());
    if (gethostname(hostname,MAX_NAME_LENGTH) < 0){
	fprintf(stderr, "chat: couldn't get hostname\n");
	exit(1);
    }
    printf("Your name? [%s@%s]: ",pw->pw_name, hostname);
    gets(namebuf);
    if (strcmp(namebuf, "Chatmaster") == 0) {
	printf("Very funny.\7\7\7\7\7\7\7\7\n");
	exit(0);
    }
    if (strlen(namebuf) == 0)
	 sprintf(namebuf,"%s@%s",pw->pw_name,hostname);
    name = namebuf;
    len = strlen(name);

    if ((game = find_game(server, gamedat.type)) < 0) {
	fprintf(stderr, "chat: couldn't join or create game\n");
	exit(1);
    }
    nlen = htonl(len);
    write(game, &nlen, sizeof(nlen));
    write(game, name, len);
    play(game, name);
}

play(game, name)
int game;
char *name;
{
    WINDOW *input_window;
    xwin_t output_window;

    register int i;
    fd_set readfds, selectmask;
    int len, maxfds, x, y;
    char chr;
    char inbuf[MAX_BUF_LENGTH], outbuf[MAX_BUF_LENGTH], *startbuf, *ptr;

    bzero(&output_window, sizeof(output_window));

    initscr();
    savetty();
    crmode();
    noecho();

    input_window = subwin(stdscr, 4, COLS, 0, 0);
    scrollok(input_window, TRUE);
    wmove(input_window, 0, 0);

    output_window.x_nlines = LINES - 5;
    output_window.x_ncols = COLS;
    output_window.x_win = subwin(stdscr,output_window.x_nlines, output_window.x_ncols, 5, 0);
    scrollok(output_window.x_win, FALSE);


    move(4, 0);
    for (i = 0; i < COLS; i++) addch('=');
    refresh();

    wrefresh(input_window);
    wrefresh(output_window.x_win);
    wmove(output_window.x_win, 0, 0);


    strcpy(inbuf, "\n");
    strcat(inbuf, name);
    strcat(inbuf, ": ");
    startbuf = inbuf + strlen(inbuf);
    ptr = startbuf;

    FD_ZERO(&selectmask);
    FD_SET(game, &selectmask);
    FD_SET(0, &selectmask);
    maxfds = game + 1;
    signal(SIGINT, die);
    while (1) {
	wrefresh(input_window);
	readfds = selectmask;
	while (select(maxfds, &readfds, NULL, NULL, NULL) < 0) {
	    wrefresh(input_window);
	    readfds = selectmask;
	}
	if (FD_ISSET(game, &readfds)) {
	    if (safe_read(game, &nlen, sizeof(nlen)) < 0) {
		perror("chat: chatmaster died 1");
		resetty();
		exit(1);
	    }
	    len = ntohl(nlen);
	    if (safe_read(game, outbuf, len) < 0) {
		    perror("chat: chatmaster died 2");
		    resetty();
		    exit(1);
	    }
	    outbuf[len] = '\0';
	    display(&output_window, outbuf, len);
	}
	if (FD_ISSET(0, &readfds)) {
	    read(0, &chr, sizeof(chr));
	    switch (chr) {
		case '\n':
		case '\r':
		    waddch(input_window, '\n');
		    if (ptr > startbuf) { /* ignore blank lines */
			len = ptr - inbuf;
			nlen = htonl(len);
			write(game, &nlen, sizeof(nlen));
			write(game, inbuf, len);
			ptr = startbuf;
		    }
		    wclrtoeol(input_window);
		    break;
		case '\177':
		case '\b':
		    if (ptr > startbuf) {
			ptr--;
			getyx(input_window, y, x);
			if (x < (len = strlen(unctrl(*ptr)))) {
			    if (y == 0) {
				char *tmp;
				
				wmove(input_window, 0, 0);
				for (tmp = startbuf; tmp < ptr; tmp++) {
				    waddstr(input_window, unctrl(*tmp));
				}
				getyx(input_window, y, x);
			    }
			    else {
				y--;
				x += input_window->_maxx - len;
			    }
			}
			else {
			    x -= len;
			}
				
			wmove(input_window, y, x);
			wclrtobot(input_window);
			wmove(input_window, y, x);
		    }
		    break;
		/* ^U */
		case '\025':
		    ptr = startbuf;
		    getyx(input_window, y, x);
		    wmove(input_window, y, 0);
		    wclrtobot(input_window);
		    wmove(input_window, y, 0);
		    break;
		/* ^L */
		case '\f':
		    wrefresh(curscr);
		    break;
		/* ^C */
		case '\3':
		    echo();
 		    noraw();
		    endwin();
		    resetty();
		    exit(0);
	    	/* ^A */
		case '\1':
		    getyx(input_window, y, x);
		    wmove(input_window, y, 0);
		    wclrtobot(input_window);
		    wmove(input_window, y, 0);
		    printf("Your new name?");
		    echo();
		    /*raw();*/
		    gets(outbuf);
		    noecho();
		    crmode();
		    if (strcmp(outbuf, "Chatmaster") == 0) {
			printf("Very funny.\7\7\7\7\7\7\7");
			wrefresh(curscr);
			break;
		    }
		    len = htonl(MC_NEW_NAME);
		    write(game, &len, sizeof(len));
		    len = strlen(outbuf);
		    nlen = htonl(len);
		    write(game, &nlen, sizeof(nlen));
		    write(game, outbuf, len);
		    strcpy(inbuf, "\n");
		    strcat(inbuf, outbuf);
		    strcat(inbuf, ": ");
		    startbuf = inbuf + strlen(inbuf);
		    ptr = startbuf;
		    wrefresh(curscr);

		    break;
		/* ^G */
		case '\7':
		    break;
		/* ^W */
		case '\027':
		    len = htonl(MC_QUERY_GAME);
		    write(game, &len, sizeof(len));
		    break;
		default:
		    *(ptr++) = chr;
		    waddstr(input_window, unctrl(chr));
		    break;
	    }
	}
    }
}

die()
{
	noraw();
	echo();
	resetty();
	endwin();
	exit(0);
}

display(win, text, size)
     register xwin_t *win;
     char *text;
     int size;
{
	register int i;
	char cch;
	
	for (i = 0; i < size; i++) {
		
		if (*text == '\n') {
			xscroll(win, 0);
			text++;
			continue;
		}
		
		if (win->x_col == COLS-1) {
			/* check for wraparound */
			xscroll(win, 0);
		}
		
		if (*text < ' ' && *text != '\t' && *text != '\7') {
			waddch(win->x_win, '^');
			getyx(win->x_win, win->x_line, win->x_col);
			
			if (win->x_col == COLS-1) {
				/* check for wraparound */
				xscroll(win, 0);
			}
			
			cch = (*text & 63) + 64;
			waddch(win->x_win, cch);
		} else {
			waddch(win->x_win, *text);
		}
		
		getyx(win->x_win, win->x_line, win->x_col);
		text++;

	}
	wrefresh(win->x_win);
}

xscroll(win, flag)
     register xwin_t *win;
     int flag;
{
	if (flag == -1) {
		wmove(win->x_win, 0, 0);
		win->x_line = 0;
		win->x_col = 0;
		return;
	}
	win->x_line = (win->x_line + 1) % win->x_nlines;
	win->x_col = 0;
	wmove(win->x_win, win->x_line, win->x_col);
	wclrtoeol(win->x_win);
	wmove(win->x_win, (win->x_line + 1) % win->x_nlines, win->x_col);
	wclrtoeol(win->x_win);
	wmove(win->x_win, win->x_line, win->x_col);
}
