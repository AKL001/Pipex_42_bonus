/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:03:40 by ablabib           #+#    #+#             */
/*   Updated: 2024/12/27 14:03:41 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	cleanup_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	cleanup_pipes(int (*pipefd)[2], int pipes_num)
{
	int	i;

	if (!pipefd)
		return ;
	i = 0;
	while (i < pipes_num)
	{
		if (pipefd[i][0] >= 0)
			close(pipefd[i][0]);
		if (pipefd[i][1] >= 0)
			close(pipefd[i][1]);
		i++;
	}
	free(pipefd);
}

void	cleanup_pipex(t_pipex *pipex)
{
	if (pipex->infile >= 0)
		close(pipex->infile);
	if (pipex->outfile >= 0)
		close(pipex->outfile);
	if (pipex->pipefd)
		cleanup_pipes(pipex->pipefd, pipex->pipes_num);
	if (pipex->cmd_args)
		cleanup_array(pipex->cmd_args);
	pipex->pipefd = NULL;
	pipex->cmd_args = NULL;
	pipex->env = NULL;
	pipex->argv = NULL;
	pipex->infile = -1;
	pipex->outfile = -1;
}

void	here_doc_fork_fail(int pipe[2][2])
{
	perror("error");
	close_pipes(2, pipe);
	exit(1);
}
