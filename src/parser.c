/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:23:34 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/01/22 14:38:55 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "utils.h"

static int	_prs_is_delim_token(void *tok_key)
{
	const int	k = (int)tok_key;

	return (k == LX_IF_AND || k == LX_IF_OR);
}

static int	_prs_collect_tokens(t_llist *group, t_ll_elem **ptr, long *type, int *lvl)
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
			return (ft_error(1, "minishell: _prs_collect_tokens", 1));
		*ptr = (*ptr)->next;
		// while (NULL != *ptr && (*ptr)->key == LX_SEP)
		// 	*ptr = (*ptr)->next;
		// if (NULL != *ptr && ((*lvl > 0 || !_prs_is_delim_token((*ptr)->key))))
		// 	llist_push(group, (void *)LX_SEP, NULL);
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
			return (ft_error(1, "minishell: syntax error near token `||' or `&&'", 0));
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
		return (ft_error(1, "minishell: syntax error near token `||' or `&&'", 0));
	return (0);
}

// debug only
long long convert(int n) {
  long long bin = 0;
  int rem, i = 1;

  while (n!=0) {
    rem = n % 2;
    n /= 2;
    bin += rem * i;
    i *= 10;
  }

  return bin;
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
		printf("[parser.c] _PRS_GROUP NEW GROUP\n");
		group = llist_new(NULL, NULL, NULL);
		if (NULL == group)
			return (ft_error(1, "minishell: _prs_group", 1));
		type = PRS_SIMPLE;
		lvl = 0;
		if (_prs_collect_tokens(group, &ptr, &type, &lvl))
			return (1);
		if (group->size)
		{
			printf("[parser.c] _PRS_GROUP COLLECTED TOKENS:\n");
			for (t_ll_elem *h = group->head; h != NULL; h = h->next)
				printf("%10d | %s\n", (int)h->key , h->val);
		}
		else
			printf("[parser.c] _PRS_GROUP GROUP IS EMPTY\n");
		if (lvl)
			return (ft_error(1, "minishell: syntax error near token `(' or `)'", 0));
		if (group->size && llist_push(groups, (void *)type, group))
			return (ft_error(1, "minishell: _prs_group", 1));
		if (group->size == 0)
			llist_free(group);
		if (NULL != ptr && _prs_is_delim_token(ptr->key)
			&& llist_push(groups, ptr->key, NULL))
			return (ft_error(1, "minishell: _prs_group", 1));
		if (NULL != ptr)
			ptr = ptr->next;
	}
	printf("[parser.c] _PRS_GROUP GROUPS:\n");
	for (t_ll_elem *h = groups->head; h != NULL; h = h->next)
		printf("%0.10lld | %p\n", convert((int)h->key), h->val);
	return (0);
}

//expect envp here
int	prs_parse(t_llist *tokens, t_info *info)
{
	t_llist		*groups;

	groups = llist_new(NULL, NULL, llist_free_wrapper);
	if (NULL == groups)
		return (1); // malloc
	if (_prs_group(groups, tokens))
		return (1); // it brokey :c
	printf("[parser.c] PRS_GROUP OK\n");
	if (_prs_check_syntax(groups))
		return (1); // syntax error obv
	printf("[parser.c] PRS_CHECK_SYNTAX OK\n");
	if (_prs_logexec(groups, info))
		return (1); // it brokey :<
	printf("[parser.c] PRS_LOGEXEC OK\n");
	llist_free(groups);
	return (0);
}
