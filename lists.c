#include "server.h"

void			delete_list(t_client_list **client_list)
{
	t_client_list *delete;

	while (*client_list != NULL)
	{
		delete = *client_list;
		*client_list = (*client_list)->next;
		if (delete->ip != NULL)
			free(delete->ip);
		free(delete);
	}
}

t_client_list	*delete_client(t_client_list **client_list, int client)
{
	if ((*client_list) == NULL)
		return (NULL);
	if ((*client_list)->client == client)
	{
		t_client_list 	*tmp_next;

		tmp_next = (*client_list)->next;
		if ((*client_list)->ip)
			free((*client_list)->ip);
		free((*client_list));
		return(tmp_next);
	}
	(*client_list)->next = delete_client(&(*client_list)->next, client);
	return((*client_list));
}

int 			size_list(t_client_list *client_list)
{
	t_client_list 	*tmp;
	int 			size = 0;

	tmp = client_list;
	while (tmp != NULL)
	{
		tmp = tmp->next;
		size++;
	}
	return (size);
}

t_client_list	*new_list(int client, char *ip, t_client_list **client_list)
{
	t_client_list	*new;

	new = (t_client_list*)malloc(sizeof(t_client_list));
	if (new == NULL)
		server_err("Malloc error!", client_list);
	new->ip = strdup(ip);
	if (new->ip == NULL)
		server_err("Strdup error", client_list);
	new->client = client;
	new->next = NULL;
	return (new);
}

void			push_back_client(t_client_list **client_list, int client, char *ip)
{
	t_client_list 	*tmp;

	if (*client_list == NULL)
		*client_list = new_list(client, ip, client_list);
	else
	{
		tmp = *client_list;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_list(client, ip, client_list);
	}
}