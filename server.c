#include "server.h"

char	*Hello[13]={
	"           \033[1;33m(\033[0m                  ,&&&.\n",
	"             \033[0;33m)\033[0m                .,.&&\n",
	"            \033[1;33m(  (\033[0m              \\=__/\n",
	"                \033[0;33m)\033[0m             ,'-'.\n",
	"          \033[1;33m(\033[0m    \033[0;33m(\033[0m  ,,      _.__|/ /|\n",
	"           \033[0;33m)\033[0m /\\ -((------((_|___/ |\n",
	"         \033[1;33m(\033[0m  // | \033[0;33m(\033[0m`'      ((  `'--|\n",
	"       _ -.;_/ \\\\--._      \\\\ \\\\-._/.\n",
	"      (_;-// | \\ \\-'.\\    <_,\\_\\`--'|\n",
	"      ( `.__ _  ___,')      <_,-'__,'\n",
	"       `'(_ )_)(_)_)'\n",
	STR_WELCOME, NULL
};

void			welcome(int client, t_client_list *client_list)
{
	int 	i;

	for (i = 0; Hello[i] != NULL; i++)
	{
		if (send(client, Hello[i], strlen(Hello[i]), 0) < 0)
			server_err("Send error", &client_list);
	}
}

void			server_err(char *str, t_client_list **client_list)
{
	perror(str);
	delete_list(client_list);
	exit(-1);
}

int 			setnonblock(int fd) {
   int fdflags;

   if ((fdflags = fcntl(fd, F_GETFL, 0)) == -1)
      return -1;
   fdflags |= O_NONBLOCK;
   if (fcntl(fd, F_SETFL, fdflags) == -1)
      return -1;
   return 0;
}

int 			handel_message_from_client(int client, t_client_list **client_list)
{
	int 	len;
	char	buf[BUF_SIZE];

	bzero(buf, BUF_SIZE);
	if ((len = recv(client, buf, BUF_SIZE, 0)) < 0)
		server_err("Recv error", client_list);
	if (len == 0)
	{
		if (close(client) < 0)
			server_err("Close error", client_list);
		*client_list = delete_client(client_list, client);
		printf("\033[1;31mClient with fd: %d closed! Current numbers clients = %d\n\033[0m", client, size_list(*client_list));
	}
	else
	{
		printf("\033[1;33mClient #%d>> %s\033[0m", client, buf);
		if (send(client, buf, BUF_SIZE, 0) < 0 )
			server_err("Send error", client_list);
	}
	return (len);
}

int 		 	main(void)
{
	int 						i;
	int							listener;
	int 						epfd;
	int 						client, epoll_events_count;
	clock_t						tStart;
	socklen_t					socklen;
	t_client_list 				*client_list = NULL;
	struct sockaddr_in 			addr, their_addr;
	static struct epoll_event 	ev, events[EPOLL_SIZE];

	socklen = sizeof(addr);
	addr.sin_family = PF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ev.events = EPOLLIN | EPOLLET;

	if ((listener = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		server_err("Socket error", &client_list);
	printf("Main listener(fd=%d) created!\n",listener);
	if (setnonblock(listener) == -1)
		server_err("Setnonblock error", &client_list);
	if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		server_err("Bind error", &client_list);
	printf("Listener binded to %s\n", inet_ntoa(addr.sin_addr));
	if (listen(listener, 1) < 0)
		server_err("Listen error", &client_list);
	printf("Start to listen!\n");
	if ((epfd = epoll_create(EPOLL_SIZE)) < 0)
		server_err("Epoll create error", &client_list);
	printf("Epoll(fd=%d) created!\n", epfd);
	ev.data.fd = listener;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, listener, &ev) < 0)
		server_err("Epoll ctl error", &client_list);
	printf("Main listener(%d) added to epoll!\n", epfd);
	while (1)
	{
		if ((epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, EPOLL_RUN_TIMEOUT)) < 0)
			server_err("Epoll wait error", &client_list);
		printf("Epoll events count: %d\n", epoll_events_count);
		tStart = clock();
		for (i = 0; i < epoll_events_count; i++)
		{
			// EPOLLIN event for listener(new client connection)
			if (events[i].data.fd == listener)
			{
				if ((client = accept(listener, (struct sockaddr *) &their_addr, &socklen)) < 0)
					server_err("Accept error", &client_list);
				printf("\033[0;32mConnection from: %s:%d, socket assigned: %d \033[0m\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), client);
				setnonblock(client);
				ev.data.fd = client;
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, client, &ev) < 0)
					server_err("Epoll ctl error", &client_list);
				push_back_client(&client_list, client, inet_ntoa(their_addr.sin_addr));
				printf("Add new client(fd = %d) and now client_list.size = %d\n", client, size_list(client_list));
				welcome(client, client_list);
			}
			else
			{
				// EPOLLIN event for others(new incoming message from client)
				if (handel_message_from_client(events[i].data.fd, &client_list) < 0)
					server_err("Handle message from client", &client_list);
			}
		}
		// print epoll events handling statistics
		printf("Statistics: %d events handled at: %.6f second(s)\n", epoll_events_count, (double)(clock() - tStart)/CLOCKS_PER_SEC);
	}
	close(listener);
	close(epfd);
	exit(0);
}