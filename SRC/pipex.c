/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miandrad <miandrad@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:09:22 by miandrad          #+#    #+#             */
/*   Updated: 2023/03/16 14:54:44 by miandrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	first_process(t_cmd *vars, char **av, int *fdf, int *fd)
{
	int	id;

	fdf[0] = open(av[1], O_RDONLY);
	if (fdf[0] == -1)
		return ;
	id = fork();
	if (id < 0)
		return ;
	if (id == 0)
	{
		dup2(fdf[0], 0);
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		close(fdf[0]);
		ft_printf("aqui\n\0");
		if (vars->path1 != NULL)
			execve(vars->path1, vars->cmd1, NULL);
		exit(0);
	}
	close(fdf[0]);
	close(fd[1]);
}

void	second_process(t_cmd *vars, char **av, int *fdf, int *fd)
{
	int	id;

	fdf[1] = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0000644);
	if (fdf[1] == -1)
		return ;
	id = fork();
	if (id < 0)
		return ;
	if (id == 0)
	{
		dup2(fdf[1], 1);
		dup2(fd[0], 0);
		close(fd[0]);
		close(fdf[1]);
		ft_printf("aqui\n");
		if (vars->path2)
			execve(vars->path2, vars->cmd2, NULL);
		exit(0);
	}
	close(fdf[0]);
	close(fd[1]);
}

void	frees(t_cmd *vars, char **av)
{
	int		i;

	if (vars->path2 != NULL)
		free(vars->path2);
	if (vars->path1 != NULL)
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
	t_fd	fd;
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
	vars.path2 = check_cmd(vars.cmd2p, env);
	if (pipe(fd.fd) == -1)
	{
		frees(&vars, av);
		return (1);
	}
	first_process(&vars, av, fd.fdf, fd.fd);
	second_process(&vars, av, fd.fdf, fd.fd);
	dup2(fd.fdf[1], 1);
	dup2(fd.fd[0], 0);
	close(fd.fd[0]);
	close(fd.fdf[1]);
	wait(NULL);
	frees(&vars, av);
}
