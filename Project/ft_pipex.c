/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: souaammo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:19:20 by souaammo          #+#    #+#             */
/*   Updated: 2024/12/15 16:19:23 by souaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_pipex.h"

void	ft_pipex(int fd_in, char *cmd, int fd_out)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_msg_error("Fork error\n", NULL, 6);
	if (pid == 0)
	{
		if (dup2(fd_in, 0) == -1)
			ft_msg_error("Dup2 error on input\n", NULL, 7);
		if (dup2(fd_out, 1) == -1)
			ft_msg_error("Dup2 error on output\n", NULL, 7);
		ft_run_cmd(cmd);
		exit(7);
	}
	else
		wait(NULL);
}

int	ft_openfile(int n, char *name, int temp)
{
	int	fd;

	if (n == 1)
		fd = open(name, O_RDONLY);
	else if (n == 2)
		fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		if (temp != -1)
			close(temp);
		ft_msg_error("Error open file\n", NULL, 1);
	}
	return (fd);
}

void	ft_here_doc(char *stop, int fd)
{
	char	*line;

	while (1)
	{
		write(1, "here_doc> ", 10);
		line = get_next_line(0);
		if (!line)
			ft_msg_error("Error reading line\n", NULL, 1);
		if (ft_strcmp(line, stop) == 1337)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
}

void	ft_main(int ac, char **av, int i)
{
	int	fd_in;
	int	fd_out;
	int	pipe_fd[2];

	fd_in = ft_openfile(1, av[1], -1);
	while (i < ac - 2)
	{
		if (pipe(pipe_fd) == -1)
			ft_msg_error("Pipe error\n", NULL, 5);
		ft_pipex(fd_in, av[i], pipe_fd[1]);
		close(pipe_fd[1]);
		fd_in = pipe_fd[0];
		i++;
	}
	fd_out = ft_openfile(3, av[ac - 1], -1);
	ft_pipex(fd_in, av[i], fd_out);
	ft_close(fd_in, fd_out);
}

int	main(int ac, char **av, char **env)
{
	int	fd_in;
	int	i;

	i = ft_check_args(ac, av);
	ft_check_env(ac, av, env, i);
	if (i == 3)
	{
		fd_in = ft_openfile(2, av[1], -1);
		ft_here_doc(av[2], fd_in);
		i = 2;
		while (i < ac - 1)
		{
			av[i] = av[i + 1];
			i++;
		}
		ac--;
	}
	i = 2;
	ft_main(ac, av, i);
	exit(0);
}
