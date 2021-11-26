/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 12:54:16 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/11/26 12:09:11 by rdrizzle         ###   ########.fr       */
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

static int	_lx_error(const char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	return (1);
}

int	lx_lexer(t_llist *tokens, const char *line)
{
	unsigned long	i;
	unsigned long	t;

	i = 0;
	while (line && line[i])
	{
		if (line[i] && ft_isspace(line[i]))
			llist_push(tokens, (int *)LX_SEP, NULL);
		while (line[i] && ft_isspace(line[i]))
			++i;
		if (line[i] == '\'' || line[i] == '\"')
		{
			t = _lx_case_quotes(tokens, line, i);
			if (t == 0)
				return (_lx_error("unclosed quote\n"));
			i += t + 1;
		}
		else if (line[i] && _lx_is_special(line[i]))
			i += _lx_case_metachar(tokens, line, i) + 1;
		else if (line[i])
			i += _lx_case_word(tokens, line, i);
	}
	return (0);
}
