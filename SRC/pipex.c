/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miandrad <miandrad@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:09:22 by miandrad          #+#    #+#             */
/*   Updated: 2023/03/07 16:11:20 by miandrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*check_cmd(char *cmd1, char **env)
{
	int		i;
	char	**paths;
	char	*path;

	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 6))
			break ;
		i++;
	}
	paths = ft_split(&env[i][5], ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], cmd1);
		if (!access(path, X_OK))
			break ;
		free(path);
		path = NULL;
		i++;
	}
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	return (path);
}

int	main(int ac, char **av, char **env)
{
	int		i;
	int		id;
	int		fd[2];
	int		fdf[2];
	char	*path1;
	char	*path2;
	char	**cmd1;
	char	**cmd2;
	char	*cmd1p;
	char	*cmd2p;

	if (ac != 5)
		exit (0);
	if (av == NULL || av[1] == NULL)
		exit (0);
	av[1] = ft_strjoin("./", av[1]);
	av[4] = ft_strjoin("./", av[4]);
	cmd1 = ft_split(av[2], ' ');
	cmd2 = ft_split(av[3], ' ');
	cmd1p = ft_strjoin("/", cmd1[0]);
	cmd2p = ft_strjoin("/", cmd2[0]);
	fdf[0] = open(av[1], O_RDONLY);
	fdf[1] = open(av[4], O_WRONLY | O_TRUNC | O_CREAT);
	if (fdf[0] == -1 || fdf[1] == -1)
		return (1);
	path1 = check_cmd(cmd1p, env);
	if (path1 == NULL)
		perror("First command invalid");
	path2 = check_cmd(cmd2p, env);
	if (path2 == NULL)
		perror("Second command invalid");
	if (pipe(fd) == -1)
		exit (2);
	id = fork();
	if (id < 0)
		exit (2);
	if (id == 0)
	{
		dup2(fdf[0], 0);
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		close(fdf[0]);
		execve(path1, cmd1, NULL);
	}
	close(fdf[0]);
	close(fd[1]);
	free(path1);
	id = fork();
	if (id < 0)
		exit (0);
	if (id == 0)
	{
		dup2(fdf[1], 1);
		dup2(fd[0], 0);
		close(fd[0]);
		close(fdf[1]);
		execve(path2, cmd2, NULL);
	}
	close(fd[0]);
	close(fdf[1]);
	wait(NULL);
	free(path2);
	free(av[1]);
	free(av[4]);
	free(cmd1p);
	free(cmd2p);
	i = 0;
	while (cmd1[i])
	{
		free(cmd1[i]);
		i++;
	}
	free(cmd1);
	i = 0;
	while (cmd2[i])
	{
		free(cmd2[i]);
		i++;
	}
	free(cmd2);
}
