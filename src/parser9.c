/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser9.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 19:03:41 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 22:47:50 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "lexer.h"
#include "utils.h"

int	_prs_prepare_group(t_llist *expanded, t_group *cmds)
{
	if (cmds->type & PRS_PIPELINE)
		return (_prs_group_pipe(expanded, cmds->cmds));
	return (_prs_group_cmd(expanded->head, cmds->cmds));
}

int	_prs_group_pipe(t_llist *expanded, t_llist *cmds)
{
	t_ll_elem	*curr;

	curr = expanded->head;
	while (curr != NULL)
	{
		if (_prs_group_cmd(curr, cmds))
			return (1);
		while (curr != NULL && (int)curr->key != LX_PIPE)
			curr = curr->next;
		if (curr && curr->next == NULL)
			return (ft_error(1, "minishell: parse error near `|'", 0, 258));
		if (curr)
			curr = curr->next;
	}
	return (0);
}

void	_prs_waitall(int pid, int *sig)
{
	int		pid2;

	pid2 = 1;
	while (pid2 > 0)
	{
		pid2 = waitpid(0, sig, 0);
		if (pid == pid2 && g_exit == 0)
			g_exit = WEXITSTATUS(*sig);
	}
}

int	_prs_logexec_bonus(t_ll_elem *ptr, int *expect)
{
	if ((g_exit == 0 && ((int)ptr->key == LX_IF_OR))
		|| (g_exit != 0 && ((int)ptr->key == LX_IF_AND)))
		return (1);
	*expect = 0;
	return (0);
}

void	_prs_alloc_cmd_info(t_cmd_info **info)
{
	*info = malloc(sizeof(t_cmd_info));
	if (*info == NULL)
		return ;
	(*info)->in_file = NULL;
	(*info)->out_file = NULL;
	(*info)->flags = 0;
	(*info)->_shlvl = 0;
}
