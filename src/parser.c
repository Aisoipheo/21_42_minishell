/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:23:34 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 18:13:58 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "utils.h"

int	_prs_is_delim_token(void *tok_key)
{
	const int	k = (int)tok_key;

	return (k == LX_IF_AND || k == LX_IF_OR);
}

static int	_prs_collect_tokens(t_llist *group,
				t_ll_elem **ptr, long *type, int *lvl)
{
	while (NULL != *ptr && (*ptr)->key == LX_SEP)
		*ptr = (*ptr)->next;
	while (NULL != *ptr && ((*lvl > 0 || !_prs_is_delim_token((*ptr)->key))))
	{
		(*lvl) += ((int)(*ptr)->key == LX_PARN_L);
		(*lvl) -= ((int)(*ptr)->key == LX_PARN_R);
		(*type) |= (*lvl > 0) << 5;
		(*type) |= ((int)(*ptr)->key == LX_PIPE && *lvl == 0) << 6;
		if (llist_push(group, (*ptr)->key, (*ptr)->val))
			return (ft_error(1, "minishell: _prs_collect_tokens", 1, 0));
		*ptr = (*ptr)->next;
	}
	return (0);
}

// FROM line 38
		// while (NULL != *ptr && (*ptr)->key == LX_SEP)
		// 	*ptr = (*ptr)->next;
	// if (NULL != *ptr && ((*lvl > 0 || !_prs_is_delim_token((*ptr)->key))))
		// 	llist_push(group, (void *)LX_SEP, NULL);

static int	_prs_check_syntax(t_llist *groups)
{
	t_ll_elem	*ptr;
	int			mode;
	int			expected;

	expected = 0;
	ptr = groups->head;
	while (NULL != ptr)
	{
		if (expected == 0 && _prs_is_delim_token(ptr->key))
			return (ft_error(1,
					"minishell: syntax error near token `||' or `&&'", 0, 258));
		else
			expected = 1;
		if (expected == 1 && _prs_is_delim_token(ptr->key))
		{
			expected = 0;
			mode = (int)ptr->key;
		}
		ptr = ptr->next;
	}
	if (expected == 0)
		return (ft_error(1,
				"minishell: syntax error near token `||' or `&&'", 0, 258));
	return (0);
}

// GROUP TYPES
//  001	| builtin or binary
//  x11	| subshell
//  101	| pipeline
static int	_prs_group(t_llist *groups, t_llist *tokens)
{
	t_ll_elem	*ptr;
	t_llist		*group;
	long		type;
	int			lvl;

	ptr = tokens->head;
	while (NULL != ptr)
	{
		group = llist_new(NULL, NULL, NULL);
		if (NULL == group)
			return (ft_error(1, "minishell: _prs_group", 1, 0));
		type = PRS_SIMPLE;
		lvl = 0;
		if (_prs_collect_tokens(group, &ptr, &type, &lvl))
			return (1);
		if (lvl && _prs_handle_group1(group))
			return (ft_error(1,
					"minishell: syntax error near token `(' or `)'", 0, 258));
		if (_prs_handle_group0(groups, group, ptr, type))
			return (1);
		if (NULL != ptr)
			ptr = ptr->next;
	}
	return (0);
}

//expect envp here
int	prs_parse(t_llist *tokens, t_info *info)
{
	t_llist		*groups;

	groups = llist_new(NULL, NULL, llist_free_wrapper);
	if (NULL == groups)
		return (1);
	if (_prs_group(groups, tokens))
		return (_prs_dstr(groups));
	if (groups->size == 0)
	{
		llist_free(groups);
		return (0);
	}
	if (_prs_check_syntax(groups))
		return (_prs_dstr(groups));
	if (_prs_logexec(groups, info))
		return (_prs_dstr(groups));
	llist_free(groups);
	return (0);
}
