/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:23:34 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/12/09 10:45:45 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

static int	_prs_is_delim_token(void *tok_key)
{
	const int	k = (int)tok_key;

	return (k == LX_IF_AND || k == LX_IF_OR);
}

static int	_prs_collect_tokens(t_llist *group, t_ll_elem *ptr, long *type, int *lvl)
{
	while (NULL != ptr && !_prs_is_delim_token(ptr->key) && !(*lvl))
	{
		(*lvl) += ((int)ptr->key == LX_PARN_L);
		(*lvl) -= ((int)ptr->key == LX_PARN_R);
		(*type) |= (lvl > 0) << 6;
		(*type) |= ((int)ptr->key == LX_PIPE && lvl == 0) << 7;
		if (llist_push(group, ptr->key, ptr->val))
			return (1);
		ptr = ptr->next;
	}
	return (0);
}

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
			return (1); // syntax error near token (ptr->key)
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
		return (1); // syntax error near token (w/e mode there is)
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
			return (1); //malloc error
		type = PRS_SIMPLE;
		lvl = 0;
		if (_prs_collect_tokens(group, ptr, &type, &lvl))
			return (1); //malloc error
		if (NULL != ptr && _prs_is_delim_token(ptr->key))
			llist_push(groups, ptr->key, NULL);
		if (lvl)
			return (1); // error near token `('
		if (llist_push(groups, (void *)type, group))
			return (1); //malloc error
		ptr = ptr->next;
	}
	return (0);
}

void	_llist_free_wrapper(void *list)
{
	llist_free((t_llist *)list);
}

#include <stdio.h>

//expect envp here
int	prs_parse(t_llist *tokens, t_info *info)
{
	t_llist		*groups;

	groups = llist_new(NULL, NULL, _llist_free_wrapper);
	if (_prs_group(groups, tokens))
		return (1); // it brokey :c
	printf("[parser.c] PRS_GROUP OK\n");
	if (_prs_check_syntax(groups))
		return (1); // syntax error obv
	printf("[parser.c] PRS_CHECK_SYNTAX OK\n");
	if (_prs_logexec(groups, info))
		return (1); // it brokey :<
	printf("[parser.c] PRS_LOGEXEC OK\n");
	return (0);
}
