/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 20:36:53 by gmckinle          #+#    #+#             */
/*   Updated: 2022/03/12 16:06:58 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"
#include "parser.h"

static int	ft_fdmanip1(int pfd[2], int fds[2], t_ll_elem *cmd, t_group *cmds)
{
	fds[0] = get_in_fd(cmd->val, cmds->files);
	if (fds[0] == -1)
		return (ft_error(-1, "mininshell: pipe: get in fd", 1));
	fds[1] = get_out_fd(cmd->val);
	if (fds[1] == -1)
		return (ft_error(-1, "minishell: pipe: get out fd", 1));
	if (cmd != cmds->cmds->head && fds[0] == STDIN_FILENO)
		fds[0] = pfd[0];
	return (0);
}

static void	ft_fdmanip2(int pfd[2], int fds[2], t_ll_elem *cmd, int tc)
{
	if (tc != -1)
		close(tc);
	if (cmd->next)
		close(pfd[1]);
	if (fds[0] != pfd[0] && fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds[1] != pfd[1] && fds[1] != STDOUT_FILENO)
		close(fds[1]);
}

//cmd->val == t_cmd_info	| Информация о команде
//cmd->key == t_llist		| Аргументы команды
pid_t	pipeline(t_group *cmds, t_info *info)
{
	int			pfd[2];
	int			fds[2];
	int			pid;
	int			to_close;
	t_ll_elem	*cmd;

	cmd = cmds->cmds->head;
	pfd[0] = -1;
	while (cmd)
	{
		if (ft_fdmanip1(pfd, fds, cmd, cmds) == -1)
			return (-1);
		to_close = pfd[0];
		if (cmd->next && pipe(pfd) == -1)
			return (ft_error(-1, "minishell: pipe: pipe", 1));
		if (cmd->next && fds[1] == STDOUT_FILENO)
			fds[1] = pfd[1];
		if (((t_cmd_info *)cmd->val)->flags & CMD_SUBSHELL)
			pid = ft_execsubshell(cmd, info, fds);
		else
			pid = ft_execcommon(cmd, info, fds);
		ft_fdmanip2(pfd, fds, cmd, to_close);
		cmd = cmd->next;
	}
	return (pid);
}
