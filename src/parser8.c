/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 18:56:11 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 19:10:59 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "lexer.h"
#include "utils.h"

void	_prs_token_handle_loop1(t_llist **str, t_ll_elem **ptr)
{
	*str = llist_new(llist_int_kcmp, NULL, NULL);
	llist_push(*str, (*ptr)->key, (*ptr)->val);
	while ((*ptr)->next
		&& _prs_expandable(((t_ll_elem *)(*ptr)->next)->key))
	{
		(*ptr) = (*ptr)->next;
		llist_push(*str, (*ptr)->key, (*ptr)->val);
	}
}

void	_prs_token_handle_loop2(t_llist *expanded, char *word, t_llist *words)
{
	t_ll_elem			*wptr;

	free(word);
	wptr = words->head;
	while (NULL != wptr)
	{
		llist_push(expanded, (void *)LX_WORD, wptr->val);
		if (wptr->next)
			llist_push(expanded, (void *)LX_SEP, NULL);
		wptr = wptr->next;
	}
	llist_free(words);
}

t_ll_elem	*_prs_next_token(t_ll_elem *curr)
{
	if (NULL == curr)
		return (NULL);
	curr = curr->next;
	while(curr && curr->key == LX_SEP)
		curr = curr->next;
	return (curr);
}

int	_prs_handle_dstr(t_group *group, t_llist *expanded)
{
	ft_group_free(group);
	llist_free(expanded);
	return (-1);
}

int	_prs_handle_heredoc(t_group *cmds)
{
	t_ll_elem	*cmd;
	t_cmd_info	*ci;
	int			ret;

	cmd = cmds->cmds->head;
	while(cmd)
	{
		ci = (t_cmd_info *)cmd->val;
		ret = 0;
		if (ci->flags & CMD_INSOURCE)
			ret = create_heredoc(ci, cmds->files);
		if (ret == 2)
		{
			g_exit = 1;
			return (1);
		}
		cmd = cmd->next;
	}
	return (0);
}
