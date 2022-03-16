/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser11.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 21:43:54 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 21:47:41 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "lexer.h"

void	_prs_only_empty_var(t_llist *str, char **word, t_expi *ei)
{
	t_ll_elem	*h;

	if (ei->size == 0)
	{
		h = str->head;
		while (h)
		{
			if ((int)h->key != LX_WORD)
				return ;
			h = h->next;
		}
		free(*word);
		*word = NULL;
	}
}
