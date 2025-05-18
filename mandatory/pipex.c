/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:35:38 by ablabib           #+#    #+#             */
/*   Updated: 2024/12/27 14:35:40 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	failed_fork(int *fd)
{
	close(fd[1]);
	close(fd[0]);
	perror("error");
	exit(1);
}

void	failed_dup2(int *fd, int pipe, int infile)
{
	close(fd[pipe]);
	close(infile);
	perror("error failed dup2");
	exit(1);
}

void	exec_first_child(char **av, int *fd, char **env)
{
	int	infile;

	infile = open(av[1], O_RDONLY);
	if (infile == -1)
	{
		close(fd[1]);
		close(fd[0]);
		perror("failed to open fd");
		exit(1);
	}
	close(fd[0]);
	if (dup2(infile, STDIN_FILENO) == -1)
		failed_dup2(fd, 1, infile);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		failed_dup2(fd, 1, infile);
	close(fd[1]);
	close(infile);
	exec_path(av[2], env);
	perror("command not found:");
	exit(1);
}

void	exec_sec_child(char **av, int *fd, char **env)
{
	int	outfile;

	outfile = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (outfile == -1)
	{
		close(fd[1]);
		close(fd[0]);
		perror("failed to open fd");
		exit(1);
	}
	close(fd[1]);
	if (dup2(outfile, STDOUT_FILENO) == -1)
		failed_dup2(fd, 0, outfile);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		failed_dup2(fd, 0, outfile);
	close(fd[0]);
	close(outfile);
	exec_path(av[3], env);
	perror("exec_path failed");
	exit(1);
}

int	main(int ac, char *av[], char *env[])
{
	int	fd[2];
	int	pid1;
	int	pid2;

	if (ac == 5)
	{
		if (pipe(fd) == -1)
			failed_pipe();
		pid1 = fork();
		if (pid1 == -1)
			failed_fork(fd);
		if (pid1 == 0)
			exec_first_child(av, fd, env);
		pid2 = fork();
		if (pid2 == -1)
			failed_fork(fd);
		if (pid2 == 0)
			exec_sec_child(av, fd, env);
		close(fd[1]);
		close(fd[0]);
		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);
	}
	else
		input_error();
}
