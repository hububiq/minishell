/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:41:35 by mdziadko          #+#    #+#             */
/*   Updated: 2025/05/09 19:10:26 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*	GET NEXT LINE LOGIC
	
	if errors retrun NULL
	1. read and store () remainder
	2. extract line () from the remainder
	3. return line

	read_and_store ()
	1. read from the fd BUFFER_SIZE bytes
	2. append buf to the end of remainder
		2.1 clean buf
	3. check new line () in remainder
		3.1 if new line was found 
			=> return
		3.2 if new line was not found 
			=> repeat until new line is found or read == 0

	extract_line()
	1. find new line () within remainder
	if found
		1.1 extract first part to the line
		1.2 leave remainded part in the the remainder
	if wasn't found
		1.1 move remainder to the line
		1.2 free remainder
*/

int	read_and_store(int fd, char **remainder)
{
	char	*buf;
	int		bytes_read;
	char	*temp;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (-1);
	while (!ft_strchr(*remainder, '\n'))
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read == 0)
			break ;
		if (bytes_read < 0)
			return (free(buf), -1);
		buf[bytes_read] = '\0';
		temp = ft_strjoin(*remainder, buf);
		if (!temp)
			return (free(buf), -1);
		free(*remainder);
		*remainder = temp;
	}
	return (free(buf), 0);
}

void	extract_line(char **remainder, char **line)
{
	char	*new_line;
	char	*temp;
	size_t	len;

	new_line = ft_strchr(*remainder, '\n');
	if (new_line)
	{
		len = new_line - (*remainder) + 1;
		*line = ft_substr(*remainder, 0, len);
		temp = ft_strdup(new_line + 1);
	}
	else
	{
		*line = ft_strdup(*remainder);
		temp = NULL;
	}
	free(*remainder);
	*remainder = temp;
}

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!remainder)
		remainder = ft_strdup("");
	if (!remainder)
		return (NULL);
	if (read_and_store(fd, &remainder) == -1)
		return (free(remainder), NULL);
	if (!remainder || !(*remainder))
	{
		free(remainder);
		remainder = NULL;
		return (NULL);
	}
	extract_line(&remainder, &line);
	return (line);
}

/*
t_fd_list	*get_fd(t_fd_list **head, int fd)
{
	t_fd_list	*node;

	node = *head;
	while (node && node->fd != fd)
		node = node->next;
	if (node)
		return (node);
	node = malloc(sizeof(t_fd_list));
	if (!node)
		return (NULL);
	node->remainder = ft_strdup("");
	if (!node->remainder)
	{
		free(node);
		return (NULL);
	}
	node->fd = fd;
	node->eof = 0;
	node->next = *head;
	*head = node;
	return (node);
}

int	free_mem(char *mem, t_fd_list **head, int fd, int is_list)
{
	t_fd_list	**current;
	t_fd_list	*temp;

	if (mem)
		free(mem);
	if (!is_list)
		return (0);
	current = head;
	while (*current)
	{
		if ((*current)->fd == fd)
		{
			temp = (*current);
			*current = (*current)->next;
			free(temp->remainder);
			free(temp);
			break ;
		}
		current = &((*current)->next);
	}
	return (-1);
}

int	read_and_store(int fd, t_fd_list **head)
{
	char		*buf;
	int			bytes_read;
	char		*temp;
	t_fd_list	*current;

	current = get_fd(head, fd);
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf || !current)
		return (free_mem(buf, head, fd, 1));
	while (!ft_strchr(current->remainder, '\n'))
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			current->eof = 1;
			break ;
		}
		buf[bytes_read] = '\0';
		temp = ft_strjoin(current->remainder, buf);
		if (!temp)
			return (free_mem(buf, head, fd, 1));
		free(current->remainder);
		current->remainder = temp;
	}
	return (free_mem(buf, head, fd, 0));
}

void	extract_line(char **remainder, char **line)
{
	char	*new_line;
	char	*temp;
	size_t	len;

	new_line = ft_strchr(*remainder, '\n');
	if (new_line)
	{
		len = new_line - (*remainder) + 1;
		*line = ft_substr(*remainder, 0, len);
		temp = ft_strdup(new_line + 1);
		free(*remainder);
		*remainder = temp;
	}
	else
	{
		*line = ft_strdup(*remainder);
		free(*remainder);
		*remainder = NULL;
	}
}

char	*get_next_line(int fd)
{
	static t_fd_list	*head;
	t_fd_list			*current;
	char				*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	current = get_fd(&head, fd);
	if (!current)
		return (NULL);
	if (!current->remainder)
		current->remainder = ft_strdup("");
	if (!current->remainder)
		return (NULL);
	if (read_and_store(fd, &head) == -1)
		return (NULL);
	if (!current->remainder || !(*current->remainder))
	{
		if (current->eof == 1)
			free_mem(NULL, &head, fd, 1);
		return (NULL);
	}
	extract_line(&current->remainder, &line);
	return (line);
}

*/
