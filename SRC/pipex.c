/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miandrad <miandrad@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:09:22 by miandrad          #+#    #+#             */
/*   Updated: 2023/03/01 14:30:05 by miandrad         ###   ########.fr       */
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
		{
			paths = ft_split(&env[i][5], ':');
			i = 0;
			while (paths[i])
			{
				path = ft_strjoin(paths[i], cmd1);
				if (access(path, X_OK))
					return (path);
				i++;
			}
		}
		i++;
	}
	return (NULL);
}

int	main(int ac, char **av, char **env)
{
	int		id;
	int		fd[2];
	int		fdf;
	char	*path;
	char	**cmd1;
	char	**cmd2;

	if (ac != 5)
		exit (0);
	if (av == NULL || av[1] == NULL)
		exit (0);
	av[1] = ft_strjoin("./", av[1]);
	av[4] = ft_strjoin("./", av[4]);
	cmd1 = ft_split(av[2], ' ');
	cmd2 = ft_split(av[3], ' ');
	cmd1[0] = ft_strjoin("/", cmd1[0]);
	cmd2[0] = ft_strjoin("/", cmd2[0]);
	fdf = open(av[1], O_RDONLY);
	path = check_cmd(cmd1[0], env);
	ft_printf("%s\n", path);
	if (pipe(fd) == -1)
		exit (0);
	id = fork();
	if (id < 0)
		exit (0);
	if (id == 0)
	{
		dup2(fdf, 0);
		dup2(fd[1], 1);
		execve(path, cmd1, env);
	}
	close(fdf);
	fdf = open(av[4], O_WRONLY);
	path = check_cmd(cmd2[0], env);
	fdf = open(av[1], O_RDONLY);
	id = fork();
	if (id < 0)
		exit (0);
	if (id == 0)
	{
		// ft_printf("escrever\n");
		close(fd[1]);
		dup2(fd[0], 0);
		dup2(fdf, 1);
		execve(path, cmd2, env);
		close(fd[0]);
	}
}
