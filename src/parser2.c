/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 11:25:45 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 19:02:47 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>

#include "lexer.h"
#include "parser.h"
#include "utils.h"

#include <stdio.h>

static int	_prs_field_expansion(t_llist *str,
		t_info *info, char **word, t_expi *ei)
{
	t_llist	*chunks;

	chunks = llist_new(NULL, free, NULL);
	if (chunks == NULL)
		return (ft_error(1, "minishell: _prs_field_expansion", 1, 0));
	ei->size = 0;
	*word = NULL;
	if (_prs_field_expansion_prep(str, info, chunks, ei))
		return (_prs_field_expansion_free(chunks, word));
	*word = (char *)malloc(sizeof(char) * (ei->size + 1));
	if (*word == NULL)
		return (_prs_field_expansion_free(chunks, NULL));
	(*word)[ei->size] = '\0';
	if (_prs_field_expansion_copy(chunks, *word))
		return (_prs_field_expansion_free(chunks, word));
	llist_free(chunks);
	return (0);
}

int	_prs_asterisk_pattern_matches(const char *pattern, const char *str)
{
	char	**dp;
	int		i;
	int		strl;
	int		ptrl;

	ptrl = ft_strlen(pattern);
	strl = ft_strlen(str);
	dp = ft_malloc_char2dem(ptrl + 1, strl + 1);
	if (NULL == dp)
		return (0);
	i = 1;
	dp[0][0] = 1;
	while (i < ptrl + 1)
	{
		if (pattern[i - 1] == '*')
			dp[i][0] = dp[i - 1][0];
		++i;
	}
	_prs_pattern_match_while(pattern, str, dp);
	i = dp[ptrl][strl];
	ft_free_char2dem(dp, ptrl + 1);
	return (i);
}

t_llist	*_prs_asterisk_expansion_pwd(const char *word)
{
	DIR				*dir;
	struct dirent	*dirf;
	t_llist			*words;
	char			pwd[PATH_MAX];

	if (getcwd(pwd, PATH_MAX) == NULL)
		return (NULL);
	dir = opendir(pwd);
	words = llist_new(llist_int_kcmp, NULL, NULL);
	if (NULL == dir || NULL == words)
		return (NULL);
	dirf = readdir(dir);
	while (NULL != dirf)
	{
		if (_prs_aep_bool(dirf, word))
			llist_push(words, (void *)LX_WORD, ft_strcpy(dirf->d_name));
		dirf = readdir(dir);
	}
	closedir(dir);
	if (words->size == 0)
	{
		llist_free(words);
		words = NULL;
	}
	return (words);
}

static int	_prs_handle_token(t_ll_elem **ptr, t_llist *expanded, t_info *info)
{
	char				*word;
	t_llist				*words;
	t_llist				*str;
	static t_expi		ei;

	if ((int)(*ptr)->key == LX_REDIR_SOURCE)
		ei.f = 1;
	if (_prs_expandable((*ptr)->key))
	{
		_prs_token_handle_loop1(&str, ptr);
		_prs_field_expansion(str, info, &word, &ei);
		words = _prs_asterisk_expansion(word);
		if (NULL == words)
			llist_push(expanded, (void *)LX_WORD, word);
		else
			_prs_token_handle_loop2(expanded, word, words);
		llist_free(str);
	}
	else
		llist_push(expanded, (*ptr)->key, NULL);
	return (0);
}

t_llist	*_prs_expand(t_llist *group, t_info *info)
{
	int			_shlvl;
	t_llist		*expanded;
	t_ll_elem	*ptr;

	ptr = group->head;
	expanded = llist_new(llist_int_kcmp, NULL, free);
	_shlvl = 0;
	while (NULL != expanded && NULL != ptr)
	{
		_shlvl += ((int)ptr->key == LX_PARN_L)
			+ (-1) * ((int)ptr->key == LX_PARN_R);
		if (_shlvl > 0 && _prs_handle_subsh_token(ptr, expanded))
		{
			llist_free(expanded);
			return (NULL);
		}
		else if (_shlvl == 0 && _prs_handle_token(&ptr, expanded, info))
		{
			llist_free(expanded);
			return (NULL);
		}
		ptr = ptr->next;
	}
	return (expanded);
}
