/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miandrad <miandrad@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:09:22 by miandrad          #+#    #+#             */
/*   Updated: 2023/02/28 18:31:05 by miandrad         ###   ########.fr       */
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
	return (path);
}

int	main(int ac, char **av, char **env)
{
	int		id;
	int		fd[2];
	int		fdf;
	char	*path;
	char	**cmd1;
	char	**cmd2;

	if (ac != 4)
		return (0);
	if (av == NULL)
		return (0);
	path = ft_strjoin("./", av[1]);
	fdf = open(av[1], O_RDONLY);
	free(path);
	cmd1 = ft_split(av[2], ' ');
	cmd2 = ft_split(av[3], ' ');
	path = check_cmd(cmd1[0], env);

	if (pipe(fd) == -1)
		return (0);
	id = fork();
	if (id < 0)
		return (0);
	if (id == 0)
	{
		close(fd[0]);
		dup2(fdf, 0);
		dup2(fd[1], 1);
		execve(av[0], &av[2], NULL);
		close(fd[1]);
	}
	if (pipe(fd) == -1)
		return (0);
	id = fork();
	if (id < 0)
		return (0);
	if (id == 0)
	{
		close(fd[1]);
		dup2(fd[0], 0);
		dup2(fdf, 1);
		execve(av[0], &av[4], NULL);
		close(fd[0]);
	}
}
