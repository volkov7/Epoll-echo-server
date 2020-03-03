#ifndef SERVER_H
# define SERVER_H

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/epoll.h>
# include <fcntl.h>
# include <errno.h>
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

# define SERVER_PORT		6666
# define EPOLL_SIZE			256
# define BUF_SIZE			512
# define EPOLL_RUN_TIMEOUT	-1
# define STR_WELCOME 		"\t\tWelcome to VolkkeRServer!\n"

typedef struct 	s_client_list
{
	int 				 	client;
	char					*ip;
	struct s_client_list	*next;
}				t_client_list;

void			push_back_client(t_client_list **client_list, int client, char *ip);
int 			size_list(t_client_list *client_list);
t_client_list	*delete_client(t_client_list **client_list, int client);
void			delete_list(t_client_list **client_list);
void			server_err(char *str, t_client_list **client_list);

#endif