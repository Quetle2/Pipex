/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miandrad <miandrad@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:09:22 by miandrad          #+#    #+#             */
/*   Updated: 2023/03/08 15:22:00 by miandrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	frees(t_cmd *vars, char **av, int flag)
{
	int		i;

	if (flag != 4 && flag != 5)
		free(vars->path2);
	if (flag != 5)
		free(vars->path1);
	free(av[1]);
	free(av[4]);
	free(vars->cmd1p);
	free(vars->cmd2p);
	i = 0;
	while (vars->cmd1[i])
	{
		free(vars->cmd1[i]);
		i++;
	}
	free(vars->cmd1);
	i = 0;
	while (vars->cmd2[i])
	{
		free(vars->cmd2[i]);
		i++;
	}
	free(vars->cmd2);
	exit(flag);
}

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
	int		id;
	int		fd[2];
	int		fdf[2];
	t_cmd	vars;

	if (ac != 5)
		exit (0);
	if (av == NULL || av[1] == NULL)
		exit (0);
	av[1] = ft_strjoin("./", av[1]);
	av[4] = ft_strjoin("./", av[4]);
	vars.cmd1 = ft_split(av[2], ' ');
	vars.cmd2 = ft_split(av[3], ' ');
	vars.cmd1p = ft_strjoin("/", vars.cmd1[0]);
	vars.cmd2p = ft_strjoin("/", vars.cmd2[0]);
	vars.path1 = check_cmd(vars.cmd1p, env);
	if (vars.path1 == NULL)
		frees(&vars, av, 5);
	vars.path2 = check_cmd(vars.cmd2p, env);
	if (vars.path2 == NULL)
		frees(&vars, av, 4);
	fdf[0] = open(av[1], O_RDONLY);
	fdf[1] = open(av[4], O_WRONLY | O_TRUNC | O_CREAT);
	if (fdf[0] == -1 || fdf[1] == -1)
		frees(&vars, av, 3);
	if (pipe(fd) == -1)
		frees(&vars, av, 2);
	id = fork();
	if (id < 0)
		frees(&vars, av, 1);
	if (id == 0)
	{
		dup2(fdf[0], 0);
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		close(fdf[0]);
		execve(vars.path1, vars.cmd1, env);
	}
	close(fdf[0]);
	close(fd[1]);
	id = fork();
	if (id < 0)
		frees(&vars, av, 1);
	if (id == 0)
	{
		dup2(fdf[1], 1);
		dup2(fd[0], 0);
		close(fd[0]);
		close(fdf[1]);
		execve(vars.path2, vars.cmd2, env);
	}
	close(fd[0]);
	close(fdf[1]);
	wait(NULL);
	frees(&vars, av, 0);
}
