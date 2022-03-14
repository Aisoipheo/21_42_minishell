/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 12:54:16 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/14 20:09:48 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "lexer.h"
#include "linked_list.h"
#include "utils.h"

int	_lx_is_special(const char c)
{
	return (c == '|'
		|| c == '>'
		|| c == '<'
		|| c == '&'
		|| c == '('
		|| c == ')');
}

// static int	_lx_error(const char *msg)
// {
// 	write(STDERR_FILENO, msg, ft_strlen(msg));
// 	return (1);
// }

static int	_lx_handle_char(t_llist *tokens, const char *line, unsigned long *i)
{
	unsigned long	t;

	if (line[*i] == '\'' || line[*i] == '\"')
	{
		if (_lx_case_quotes(tokens, line, *i, &t))
			return (ft_error(1, "minishell: lx_lexer", 1, 0));
		if (t == 0)
			return (ft_error(1, "minishell: lexer: unclosed single or"
					" double quotes", 0, 0));
		*i += t + 1;
	}
	else if (line[*i] && _lx_is_special(line[*i]))
	{
		if (_lx_case_metachar(tokens, line, *i, &t))
			return (1);
		*i += t + 1;
	}
	else if (line[*i])
	{
		if (_lx_case_word(tokens, line, *i, &t))
			return (ft_error(1, "minishell: lx_lexer", 1, 0));
		*i += t;
	}
	return (0);
}

int	lx_lexer(t_llist *tokens, const char *line)
{
	unsigned long	i;

	i = 0;
	while (line && line[i])
	{
		if (line[i] && ft_isspace(line[i]) && llist_push(tokens,
				(int *)LX_SEP, NULL))
			return (ft_error(1, "minishell: lx_lexer", 1, 0));
		while (line[i] && ft_isspace(line[i]))
			++i;
		if (_lx_handle_char(tokens, line, &i))
			return (1);
	}
	return (0);
}
