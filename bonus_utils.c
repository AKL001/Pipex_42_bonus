/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablabib <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:47:40 by ablabib           #+#    #+#             */
/*   Updated: 2024/12/27 14:32:51 by ablabib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_free(char **get_free)
{
	int	i;

	i = 0;
	while (get_free[i])
	{
		free(get_free[i]);
		i++;
	}
	free(get_free);
	return ;
}

char	*get_path(char **paths, char *cmd)
{
	int		i;
	char	*path;
	char	*part_path;

	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (path && access(path, F_OK | X_OK) == 0)
			break ;
		free(path);
		path = NULL;
		i++;
	}
	return (path);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	i = -1;
	while (envp[++i] && ft_strnstr(envp[i], "PATH", 4) == 0)
		;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	path = get_path(paths, cmd);
	ft_free(paths);
	return (path);
}

void	exec_path(char *full_cmd, char **env)
{
	char	**cmd;
	char	*path;

	if (!*full_cmd)
		exit(1);
	cmd = ft_split(full_cmd, ' ');
	if (!cmd)
		return ;
	if (ft_strncmp(full_cmd, "./", 2) == 0 || ft_strncmp(full_cmd, "../", 3) == 0
		|| ft_strncmp(full_cmd, "/", 1) == 0)
		if (execve(cmd[0], cmd, env) == -1)
			exec_path_failed(cmd);
	path = find_path(cmd[0], env);
	if (path == NULL)
	{
		ft_free(cmd);
		write(2, "command not found :\n", 20);
		exit(EXIT_FAILURE);
	}
	if (execve(path, cmd, env) == -1)
	{
		free(path);
		ft_free(cmd);
		return ;
	}
}
