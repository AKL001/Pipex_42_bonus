/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:04:13 by ablabib           #+#    #+#             */
/*   Updated: 2024/12/27 14:06:20 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	here_doc_fail(void)
{
	perror("error");
	exit(1);
}

void	usage(void)
{
	ft_putstr_fd("Ex: ./pipex_bonus <file1> <cmd1> <cmd2> <...> <file2>\n", 1);
	ft_putstr_fd("./pipex_bonus here_doc <LIMITER> <cmd> <cmd1> <...> <file>\n",
		1);
	exit(EXIT_SUCCESS);
}

void	failed_to(t_pipex *pipex)
{
	perror("Error");
	cleanup_pipex(pipex);
	exit(EXIT_FAILURE);
}

void	close_all_pipes(t_pipex *pipex)
{
	int	j;

	j = -1;
	while (++j < pipex->pipes_num)
	{
		close(pipex->pipefd[j][0]);
		close(pipex->pipefd[j][1]);
	}
}
