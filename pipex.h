/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miandrad <miandrad@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:09:11 by miandrad          #+#    #+#             */
/*   Updated: 2023/03/17 16:31:38 by miandrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "ft_printf/ft_printf.h"
# include "get_next_line_100/get_next_line.h"
# include <fcntl.h>
# include <sys/wait.h>

typedef struct s_fd
{
	int	fdf[2];
	int	fd[2];
}t_fd;

typedef struct s_cmd
{
	int		id1;
	int		id2;
	char	*path1;
	char	*path2;
	char	**cmd1;
	char	**cmd2;
	char	*cmd1p;
	char	*cmd2p;
}t_cmd;

void	frees(t_cmd *vars, char **av);
char	*check_cmd(char *cmd1, char **env);

#endif