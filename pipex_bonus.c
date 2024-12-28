/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:52:29 by ablabib           #+#    #+#             */
/*   Updated: 2024/12/27 14:35:09 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	here_doc(char **argv, char **env)
{
	int	fd[2][2];
	int	pid[2];
	int	i;

	if (pipe(fd[0]) == -1 || pipe(fd[1]) == -1)
		here_doc_fail();
	write(1, "here_doc> ", 10);
	while (get_next_line(argv[2], fd) == 1)
		write(1, "here_doc> ", 10);
	close(fd[0][1]);
	i = 0;
	while (i < 2)
	{
		pid[i] = fork();
		if (pid[i] < 0)
			here_doc_fork_fail(fd);
		if (pid[i] == 0)
			here_doc_exec_cmd(argv, fd, env, i);
		i++;
	}
	close_pipes(2, fd);
	i = -1;
	while (++i < 2)
		waitpid(pid[i], NULL, 0);
	exit(0);
}

static void	execute_all_cmds(t_pipex *pipex)
{
	int	i;
	int	pid;

	i = 0;
	while (i < pipex->cmd_num)
	{
		pid = fork();
		if (pid == -1)
			failed_to(pipex);
		if (pid == 0)
			child_process(pipex, i);
		i++;
	}
	close_all_pipes(pipex);
	i = -1;
	while (++i < pipex->cmd_num)
		wait(NULL);
}

static void	get_init(t_pipex *pipex, char *cmds[])
{
	int	i;

	pipex->cmd_args = (char **)malloc(sizeof(char *) * (pipex->cmd_num + 1));
	if (!pipex->cmd_args)
		failed_to(pipex);
	pipex->cmd_args[pipex->cmd_num] = NULL;
	i = 0;
	while (i < pipex->cmd_num)
	{
		pipex->cmd_args[i] = ft_strdup(cmds[i + 2]);
		if (!pipex->cmd_args[i])
			failed_to(pipex);
		i++;
	}
}

static void	create_pipes(t_pipex *pipex)
{
	int	i;

	pipex->pipefd = malloc(sizeof(int [2]) * pipex->pipes_num);
	if (!pipex->pipefd)
		return ;
	i = 0;
	while (i < pipex->pipes_num)
	{
		if (pipe(pipex->pipefd[i]) == -1)
			failed_to(pipex);
		i++;
	}
}

int	main(int argc, char *argv[], char *env[])
{
	t_pipex	pipex;

	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		{
			if (argc > 5)
				here_doc(argv, env);
			else
				usage();
		}
		pipex.argc = argc;
		pipex.argv = argv;
		pipex.env = env;
		pipex.cmd_num = argc - 3;
		pipex.pipes_num = argc - 4;
		get_init(&pipex, argv);
		create_pipes(&pipex);
		execute_all_cmds(&pipex);
		ft_free(pipex.cmd_args);
		close_all_pipes(&pipex);
		free(pipex.pipefd);
	}
	else
		usage();
}
