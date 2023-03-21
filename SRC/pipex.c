/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miandrad <miandrad@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:09:22 by miandrad          #+#    #+#             */
/*   Updated: 2023/03/21 17:53:21 by miandrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	first_process(t_cmd *vars, char **av, t_fd *fd, char **env)
{
	vars->path1 = check_cmd(vars->cmd1p, env);
	fd->fdf[0] = open(av[1], O_RDONLY);
	if (fd->fdf[0] == -1)
	{
		perror(&av[1][2]);
		return ;
	}
	if (vars->path1 == NULL)
		ft_printf("%s: command not found\n", &vars->cmd1p[1]);
	vars->id1 = fork();
	if (vars->id1 < 0)
		return ;
	if (vars->id1 == 0)
	{
		dup2(fd->fdf[0], 0);
		dup2(fd->fd[1], 1);
		close(fd->fd[1]);
		close(fd->fd[0]);
		close(fd->fdf[0]);
		if (vars->path1 != NULL)
			execve(vars->path1, vars->cmd1, env);
		exit(2);
	}
	close(fd->fdf[0]);
	close(fd->fd[1]);
}

void	second_process(t_cmd *vars, char **av, t_fd *fd, char **env)
{
	vars->path2 = check_cmd(vars->cmd2p, env);
	fd->fdf[1] = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0000644);
	if (fd->fdf[1] == -1)
		return ;
	if (vars->path2 == NULL)
		ft_printf("%s: command not found\n", &vars->cmd2p[1]);
	vars->id2 = fork();
	if (vars->id2 < 0)
		return ;
	if (vars->id2 == 0)
	{
		dup2(fd->fdf[1], 1);
		dup2(fd->fd[0], 0);
		close(fd->fd[0]);
		close(fd->fd[1]);
		close(fd->fdf[1]);
		if (vars->path2)
			execve(vars->path2, vars->cmd2, env);
		exit(2);
	}
	close(fd->fdf[1]);
	close(fd->fd[0]);
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
	while (env[i++])
		if (ft_strnstr(env[i], "PATH=", 6))
			break ;
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
		free(paths[i++]);
	free(paths);
	return (path);
}

int	main(int ac, char **av, char **env)
{
	t_fd	fd;
	t_cmd	vars;

	if (ac != 5)
		exit (0);
	if (ac != 5)
		ft_printf("Invalid number of arguments");
	if (av == NULL || av[1] == NULL)
		exit (1);
	av[1] = ft_strjoin("./", av[1]);
	av[4] = ft_strjoin("./", av[4]);
	vars.cmd1 = ft_split(av[2], ' ');
	vars.cmd2 = ft_split(av[3], ' ');
	vars.cmd1p = ft_strjoin("/", vars.cmd1[0]);
	vars.cmd2p = ft_strjoin("/", vars.cmd2[0]);
	if (pipe(fd.fd) != -1)
	{
		first_process(&vars, av, &fd, env);
		second_process(&vars, av, &fd, env);
		close(fd.fd[0]);
		close(fd.fdf[1]);
		waitpid(vars.id1, NULL, 0);
	}
	frees(&vars, av);
}
