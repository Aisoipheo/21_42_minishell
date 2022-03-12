/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:14:34 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/12 16:45:33 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

int	ft_rebuiltenvp(t_info *info)
{
	ft_free_char2dem(info->envp, -1);
	info->envp = ft_compose_envp(info->envp_list);
	if (NULL == info->envp)
		return (ft_error(-1, "minishell: ft_execve: compose_envp", 1));
	info->envp_f = 0;
	return (0);
}

int	ft_execve(t_ll_elem *cmd, t_info *info, int fds[2])
{
	char		*path;
	char		*filepath;
	char		**args;
	pid_t		pid;

	if (info->envp_f && ft_rebuiltenvp(info) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		ft_error(-1, "minishell: ft_execve: fork", 1);
	if (pid > 0)
	{
		debug_log("RET PID: %d\n", pid);
		return (pid);
	}
	debug_log("TRY REMAPFDS\n");
	if (remap_fds(fds[0], fds[1]))
		exit(1);
	debug_log("REMAPFDS OK\n");
	path = llist_getval(info->envp_list, "PATH");
	if (ft_acces(cmd, path, &filepath))
		exit(1);
	if (create_argv(cmd, &args, filepath))
		exit(1);
	debug_log("execve\n");
	debug_log("%s\n", filepath);
	if (execve(filepath, args, info->envp) == -1)
		exit(ft_error(1, "minishell: execve", 1));
	return (1);
}

int	ft_execbuiltin(int idx, t_ll_elem *cmd, t_info *info, int fds[2])
{
	int		stdcopy[2];
	int		ret;

	stdcopy[0] = dup(STDIN_FILENO);
	stdcopy[1] = dup(STDOUT_FILENO);
	if (remap_fds(fds[0], fds[1]))
	{
		close(stdcopy[0]);
		close(stdcopy[1]);
		return (ft_error(1, "minishell: execbuiltin: remap fds", 1));
	}
	ret = (*info->f_ptrs[idx])(cmd->key, info);
	dup2(stdcopy[0], STDIN_FILENO);
	dup2(stdcopy[1], STDOUT_FILENO);
	close(stdcopy[0]);
	close(stdcopy[1]);
	return (ret);
}

int	ft_execcommon(t_ll_elem *cmd, t_info *info, int fds[2])
{
	int	i;

	debug_log("CMD: %s\n", ((t_llist *)cmd->key)->head->val);
	i = check_if_builtins(cmd, info);
	debug_log("------[%d]------\n", i);
	if (i == 7)
		return (ft_execve(cmd, info, fds));
	return (ft_execbuiltin(i, cmd, info, fds));
}

pid_t	executor(t_group *cmds, t_info *info)
{
	if (PRS_PIPELINE & cmds->type)
		return (pipeline(cmds, info));
	if (((t_cmd_info *)cmds->cmds->head->val)->flags & CMD_SUBSHELL)
		return (ft_subshell(cmds, info));
	return (ft_common(cmds, info));
}
