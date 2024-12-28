/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:48:20 by ablabib           #+#    #+#             */
/*   Updated: 2024/12/23 13:48:21 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	handle_first_cmd(t_pipex *pipex, int i);
void	handle_last_cmd(t_pipex *pipex, int i);
void	child_process(t_pipex *pipex, int i);

void	handle_first_cmd(t_pipex *pipex, int i)
{
	if (i == 0)
	{
		pipex->infile = open(pipex->argv[1], O_RDONLY);
		if (pipex->infile == -1)
			failed_to(pipex);
		if (dup2(pipex->infile, STDIN_FILENO) == -1)
			failed_to(pipex);
		close(pipex->infile);
	}
	else if (dup2(pipex->pipefd[i - 1][0], STDIN_FILENO) == -1)
		failed_to(pipex);
}

void	handle_last_cmd(t_pipex *pipex, int i)
{
	if (i == pipex->cmd_num - 1)
	{
		pipex->outfile = open(pipex->argv[pipex->argc - 1],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if ((pipex->outfile) == -1)
			failed_to(pipex);
		if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
			failed_to(pipex);
		close(pipex->outfile);
	}
	else if (dup2(pipex->pipefd[i][1], STDOUT_FILENO) == -1)
		failed_to(pipex);
}

void	child_process(t_pipex *pipex, int i)
{
	handle_first_cmd(pipex, i);
	handle_last_cmd(pipex, i);
	close_all_pipes(pipex);
	exec_path(pipex->cmd_args[i], pipex->env);
}
