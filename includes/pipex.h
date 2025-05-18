/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:55:12 by ablabib           #+#    #+#             */
/*   Updated: 2024/12/27 14:41:58 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		(*pipefd)[2];
	char	**cmd_args;
	char	**env;
	char	**argv;
	int		cmd_num;
	int		pipes_num;
	int		argc;
}			t_pipex;

/* bonus */
char		*find_path(char *cmd, char **envp);
void		exec_path(char *full_cmd, char **env);
void		close_pipes(int i, int fd[2][2]);
void		ft_free(char **get_free);
void		cleanup_pipex(t_pipex *pipex);
void		cleanup_pipes(int (*pipefd)[2], int pipes_num);
void		cleanup_array(char **array);
void		failed_to(t_pipex *pipex);

/* herdoc */
int			get_next_line(char *delimiter, int fd[2][2]);
void		here_doc_exec_cmd(char *av[], int fd[2][2], char *env[], int i);
void		close_all_pipes(t_pipex *pipex);
void		handle_first_cmd(t_pipex *pipex, int i);
void		handle_last_cmd(t_pipex *pipex, int i);
void		child_process(t_pipex *pipex, int i);
void		here_doc_fail(void);
void		here_doc_fork_fail(int pipe[2][2]);

/* others */
void		usage(void);
void		failed_pipe(void);
void		input_error(void);
void		exec_path_failed(char **cmd);

#endif
