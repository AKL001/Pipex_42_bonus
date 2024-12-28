/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:50:20 by ablabib           #+#    #+#             */
/*   Updated: 2024/12/23 13:50:23 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	ft_free_buffer(char *buffer, int i)
{
	free(buffer);
	return (i);
}

char	*ft_read_until_newline(int fd)
{
	char	*buffer;
	char	c;
	int		i;
	int		bytes_read;

	buffer = malloc(1024 * sizeof(char));
	i = 0;
	if (!buffer)
		return (NULL);
	bytes_read = read(fd, &c, 1);
	while (bytes_read > 0)
	{
		buffer[i++] = c;
		if (c == '\n')
			break ;
		bytes_read = read(fd, &c, 1);
	}
	buffer[i] = '\0';
	if (bytes_read <= 0 && i == 0)
	{
		free(buffer);
		return (NULL);
	}
	return (buffer);
}

int	get_next_line(char *delimiter, int fd[2][2])
{
	char	*buffer;
	char	*delim;

	delim = ft_strjoin(delimiter, "\n");
	if (!delim)
		return (0);
	buffer = ft_read_until_newline(0);
	if (!buffer)
		return (free(delim), 0);
	if (ft_strncmp(buffer, delim, ft_strlen(delim)) == 0)
		return (free(delim), ft_free_buffer(buffer, 0));
	write(fd[0][1], buffer, ft_strlen(buffer));
	return (free(delim), ft_free_buffer(buffer, 1));
}

void	close_pipes(int i, int fd[2][2])
{
	if (i == 0)
	{
		close(fd[0][1]);
		close(fd[1][0]);
	}
	else if (i == 1)
	{
		close(fd[0][0]);
		close(fd[1][1]);
	}
	else
	{
		close(fd[0][0]);
		close(fd[0][1]);
		close(fd[1][1]);
		close(fd[1][0]);
	}
}

void	here_doc_exec_cmd(char *av[], int fd[2][2], char *env[], int i)
{
	int	outfile;

	if (i == 0)
	{
		close_pipes(0, fd);
		dup2(fd[0][0], STDIN_FILENO);
		dup2(fd[1][1], STDOUT_FILENO);
		close_pipes(1, fd);
		exec_path(av[3], env);
	}
	else
	{
		outfile = open(av[5], O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (outfile == -1)
			here_doc_fail();
		dup2(outfile, STDOUT_FILENO);
		close(outfile);
		dup2(fd[1][0], STDIN_FILENO);
		close_pipes(2, fd);
		exec_path(av[4], env);
	}
}
