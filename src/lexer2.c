/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 10:34:11 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/14 20:10:48 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "lexer.h"
#include "linked_list.h"
#include "utils.h"
#include "norm_hacks.h"

static int	_lx_get_type(const char *line, unsigned long i)
{
	if (line[i] == '|' && line[i + 1] && line[i + 1] == '|')
		return (LX_IF_OR);
	if (line[i] == '|')
		return (LX_PIPE);
	if (line[i] == '>' && line[i + 1] && line[i + 1] == '>')
		return (LX_REDIR_APPEND);
	if (line[i] == '>')
		return (LX_REDIR_OUT);
	if (line[i] == '<' && line[i + 1] && line[i + 1] == '<')
		return (LX_REDIR_SOURCE);
	if (line[i] == '<')
		return (LX_REDIR_IN);
	if (line[i] == '&' && line[i + 1] && line[i + 1] == '&')
		return (LX_IF_AND);
	if (line[i] == '(')
		return (LX_PARN_L);
	if (line[i] == ')')
		return (LX_PARN_R);
	return (LX_NCONST);
}

static unsigned long	_lx_find_quote(const char *line, char c)
{
	unsigned long	i;

	i = 1;
	while (line[i])
	{
		if (line[i] == c)
			return (i);
		++i;
	}
	return (0);
}

int	_lx_case_quotes(t_llist *tokens, const char *line,
									unsigned long i, unsigned long *t)
{
	char			*sstr;

	*t = _lx_find_quote(line + i, line[i]);
	if (*t == 0)
		return (*t);
	sstr = ft_substr(line, i + 1, *t - 1);
	if (sstr == NULL)
		return (1);
	if (llist_push(tokens, nh_trop(line[i] == '\'', (int *)LX_FIELD,
				(int *)LX_EXP_FIELD), sstr))
	{
		free(sstr);
		return (1);
	}
	return (0);
}

int	_lx_case_metachar(t_llist *tokens, const char *line,
									unsigned long i, unsigned long *t)
{
	*t = _lx_get_type(line, i);
	if (*t == LX_NCONST)
		return (ft_error(1,
				"minishell: lx_case_metachar: error type of metachar", 0, 258));
	if (llist_push(tokens, (void *)(*t), NULL))
		return (ft_error(1, "minishell: lx_case_metachar", 1, 0));
	*t = (*t == LX_REDIR_APPEND || *t == LX_REDIR_SOURCE
			|| *t == LX_IF_AND || *t == LX_IF_OR);
	return (0);
}

int	_lx_case_word(t_llist *tokens, const char *line,
								unsigned long i, unsigned long *t)
{
	char			*sstr;

	*t = 0;
	while (line[i + *t] && !ft_isspace(line[i + *t])
		&& !_lx_is_special(line[i + *t]) && line[i + *t] != '\''
		&& line[i + *t] != '\"')
		++(*t);
	sstr = ft_substr(line, i, *t);
	if (sstr == NULL)
		return (1);
	if (llist_push(tokens, (void *)LX_WORD, sstr))
		return (1);
	return (0);
}
