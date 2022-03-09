/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 17:07:58 by gmckinle          #+#    #+#             */
/*   Updated: 2022/03/09 18:07:55 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

static int	ft_execsubshell(t_group *cmds, t_info *info, int in, int out)
{
	int		pid;

	if (info->envp_f)
	{
		ft_free_char2dem(info->envp, -1);
		info->envp = ft_compose_envp(info->envp_list);
		if (NULL == info->envp)
			return (ft_error(-1, "minishell: ft_execve: compose_envp", 1));
		info->envp_f = 0;
	}
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid > 0)
		return (pid);
	if (remap_fds(in, out))
		return (-1);
	exit(prs_parse(cmds->cmds->head->key, info));
}

int	ft_subshell(t_group *cmds, t_info *info)
{
	int		fds[2];
	int		pid;

	fds[0] = get_in_fd(cmds->cmds->head->val, cmds->files);
	if (fds[0] == -1)
		return (ft_error(-1, "minishell: subshell: get_in_fd", 1));
	fds[1] = get_out_fd(cmds->cmds->head->val);
	if (fds[1] == -1)
		return (ft_error(-1, "minishell: subshell: get_out_fd", 1));
	pid = ft_execsubshell(cmds, info, fds[0], fds[1]);
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
	return (pid);
}
