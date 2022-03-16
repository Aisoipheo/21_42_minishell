/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 20:36:53 by gmckinle          #+#    #+#             */
/*   Updated: 2022/03/16 22:02:56 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"
#include "parser.h"

static int	ft_fdmanip1(t_fd *fd, t_ll_elem *cmd, t_group *cmds)
{
	fd->fds[0] = get_in_fd(cmd->val);
	if (fd->fds[0] == -1)
		return (ft_error(-1, "mininshell: pipe: get in fd", 1, 0));
	fd->fds[1] = get_out_fd(cmd->val);
	if (fd->fds[1] == -1)
		return (ft_error(-1, "minishell: pipe: get out fd", 1, 0));
	if (cmd != cmds->cmds->head && fd->fds[0] == STDIN_FILENO)
		fd->fds[0] = fd->pfd[0];
	return (0);
}

static void	ft_fdmanip2(t_fd *fd, t_ll_elem *cmd, int tc)
{
	if (tc != -1)
		close(tc);
	if (cmd->next)
		close(fd->pfd[1]);
	if (fd->fds[0] != fd->pfd[0] && fd->fds[0] != STDIN_FILENO)
		close(fd->fds[0]);
	if (fd->fds[1] != fd->pfd[1] && fd->fds[1] != STDOUT_FILENO)
		close(fd->fds[1]);
}

//cmd->val == t_cmd_info	| Информация о команде
//cmd->key == t_llist		| Аргументы команды
pid_t	pipeline(t_group *cmds, t_info *info)
{
	t_fd		fd;
	int			pid;
	int			to_close;
	t_ll_elem	*cmd;
	int			ret;

	cmd = cmds->cmds->head;
	fd.pfd[0] = -1;
	while (cmd)
	{
		ret = ft_fdmanip1(&fd, cmd, cmds);
		to_close = fd.pfd[0];
		if (cmd->next && pipe(fd.pfd) == -1)
			return (ft_error(-1, "minishell: pipe: pipe", 1, 0));
		if (cmd->next && fd.fds[1] == STDOUT_FILENO)
			fd.fds[1] = fd.pfd[1];
		if (((t_cmd_info *)cmd->val)->flags & CMD_SUBSHELL && ret == 0)
			pid = ft_execsubshell(cmd, info, &fd);
		else if (ret == 0)
			pid = ft_execcommon(cmd, info, &fd, 1);
		ft_fdmanip2(&fd, cmd, to_close);
		cmd = cmd->next;
	}
	return (pid);
}
