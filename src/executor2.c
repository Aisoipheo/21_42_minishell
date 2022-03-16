/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:14:34 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 22:13:01 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

int	ft_rebuildenvp(t_info *info)
{
	ft_free_char2dem(info->envp, -1);
	info->envp = ft_compose_envp(info->envp_list);
	if (NULL == info->envp)
		return (ft_error(-1, "minishell: ft_execve: compose_envp", 1, 0));
	info->envp_f = 0;
	return (0);
}

int	ft_execve(t_ll_elem *cmd, t_info *info, t_fd *fd)
{
	char		*path;
	char		*filepath;
	char		**args;
	pid_t		pid;

	pid = fork();
	if (pid == -1)
		ft_error(-1, "minishell: ft_execve: fork", 1, 0);
	if (pid > 0)
		return (pid);
	if (remap_fds(fd->fds[0], fd->fds[1]))
		exit(1);
	close(fd->pfd[0]);
	path = llist_getval(info->envp_list, "PATH");
	if (ft_acces(cmd, path, &filepath))
		exit(g_exit);
	if (create_argv(cmd, &args, filepath))
		exit(g_exit);
	if (execve(filepath, args, info->envp) == -1)
		exit(ft_error(127, "minishell: execve", 1, 0));
	return (1);
}

int	ft_execbuiltin(int idx, t_ll_elem *cmd, t_info *info, t_fd *fd)
{
	int		pid;

	pid = fork();
	if (pid == -1)
		ft_error(-1, "minishell: ft_execve: fork", 1, 0);
	if (pid > 0)
		return (pid);
	if (remap_fds(fd->fds[0], fd->fds[1]))
		exit(1);
	close(fd->pfd[0]);
	exit((*info->f_ptrs[idx])(cmd->key, info));
}

int	ft_execcommon(t_ll_elem *cmd, t_info *info, t_fd *fd, int mode)
{
	int	i;

	if (((t_llist *)cmd->key)->size == 0)
		return (0);
	i = check_if_builtins(cmd, info);
	if (info->envp_f && ft_rebuildenvp(info) == -1)
		return (-1);
	if (i == 7)
		return (ft_execve(cmd, info, fd));
	if (mode)
		return (ft_execbuiltin(i, cmd, info, fd));
	return (ft_callbuiltin(i, cmd, info, fd));
}

pid_t	executor(t_group *cmds, t_info *info)
{
	signal(SIGQUIT, handler_in_executor);
	signal(SIGINT, handler_in_executor);
	if (PRS_PIPELINE & cmds->type)
		return (pipeline(cmds, info));
	if (((t_cmd_info *)cmds->cmds->head->val)->flags & CMD_SUBSHELL)
		return (ft_subshell(cmds, info));
	return (ft_common(cmds, info));
}
