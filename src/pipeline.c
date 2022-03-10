/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 20:36:53 by gmckinle          #+#    #+#             */
/*   Updated: 2022/03/10 17:56:45 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"
#include "parser.h"

// /* Child process that create a fork and a pipe, put the output inside a pipe
//  and then close with the exec function. The main process will change his stdin
//  for the pipe file descriptor. */
// void	child_process(char *argv, char **envp)
// {
// 	pid_t	pid;
// 	int		fd[2];

// 	if (pipe(fd) == -1)
// 		error();
// 	pid = fork();
// 	if (pid == -1)
// 		error();
// 	if (pid == 0)
// 	{
// 		close(fd[0]);
// 		dup2(fd[1], STDOUT_FILENO);
// 		execute(argv, envp);
// 	}
// 	else
// 	{
// 		close(fd[1]);
// 		dup2(fd[0], STDIN_FILENO);
// 		waitpid(pid, NULL, 0);
// 	}
// }

// /* Function who make a child process that will read from the stdin with
//  get_next_line until it find the limiter word and then put the output inside a
//  pipe. The main process will change his stdin for the pipe file descriptor. */
// void	here_doc(char *limiter, int argc)
// {
// 	pid_t	reader;
// 	int		fd[2];
// 	char	*line;

// 	if (argc < 6)
// 		usage();
// 	if (pipe(fd) == -1)
// 		error();
// 	reader = fork();
// 	if (reader == 0)
// 	{
// 		close(fd[0]);
// 		while (get_next_line(&line))
// 		{
// 			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
// 				exit(EXIT_SUCCESS);
// 			write(fd[1], line, ft_strlen(line));
// 		}
// 	}
// 	else
// 	{
// 		close(fd[1]);
// 		dup2(fd[0], STDIN_FILENO);
// 		wait(NULL);
// 	}
// }

// /* Main function that run the childs process with the right file descriptor
//  or display an error message if arguments are wrong. It will run here_doc
//  function if the "here_doc" string is find in argv[1] */

// pid_t	pipeline(t_group *cmds, t_info *info)
// {
// 	(void)cmds;
// 	(void)info;
// 	debug_log("PIPELINE\n");
// 	// int	i;
// 	// int	filein;
// 	// int	fileout;

// 	// if (argc >= 5)
// 	// {
// 	// 	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
// 	// 	{
// 	// 		i = 3;
// 	// 		fileout = open_file(argv[argc - 1], 0);
// 	// 		create_heredoc(argv[2], argc);
// 	// 	}
// 	// 	else
// 	// 	{
// 	// 		i = 2;
// 	// 		fileout = open_file(argv[argc - 1], 1);
// 	// 		filein = open_file(argv[1], 2);
// 	// 		dup2(filein, STDIN_FILENO);
// 	// 	}
// 	// 	while (i < argc - 2)
// 	// 		child_process(argv[i++], envp);
// 	// 	dup2(fileout, STDOUT_FILENO);
// 	// 	execute(argv[argc - 2], envp);
// 	// }
// 	// usage();
// 	return (0);
// }


//cmd->val == t_cmd_info	| Информация о команде
//cmd->key == t_llist		| Аргументы команды
pid_t	pipeline(t_group *cmds, t_info *info)
{
	int			pfd[2];
	int			fds[2];
	int			pid;
	t_ll_elem	*cmd;

	cmd = cmds->cmds->head;
	pfd[0] = -1;
	while(cmd)
	{
		fds[0] = get_in_fd(cmd->val, cmds->files);
		if (fds[0] == -1)
			return (ft_error(-1, "mininshell: pipe: get in fd", 1));
		fds[1] = get_out_fd(cmd->val);
		if (cmd != cmds->cmds->head && fds[0] == STDIN_FILENO)
			fds[0] = pfd[0];
		if (fds[1] == -1)
			return (ft_error(-1, "minishell: pipe: get out fd", 1));
		if (cmd->next && pipe(pfd) == -1)
			return (ft_error(-1, "minishell: pipe: pipe", 1));
		if (cmd->next && fds[1] == STDOUT_FILENO)
			fds[1] = pfd[1];
		if (((t_cmd_info *)cmd->val)->flags & CMD_SUBSHELL)
			pid = ft_execsubshell(cmd, info, fds);
		else
			pid = ft_execcommon(cmd, info, fds);
		close(pfd[1]);
		if (fds[0] != pfd[0] && fds[0] != STDIN_FILENO)
			close(fds[0]);
		if (fds[1] != pfd[1] && fds[1] != STDOUT_FILENO)
			close(fds[1]);
		cmd = cmd->next;
	}
	if (pfd[0] != -1)
		close(pfd[0]);
	return (pid);
}
