/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 13:23:43 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/12/09 10:48:08 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "parser.h"
#include "lexer.h"

static const char	*_lx_get_name(int type)
{
	static const char *names[LX_NCONST] =
	{
		[LX_SEP] = "SEP",
		[LX_WORD] = "WORD",
		[LX_IF_AND] = "&&",
		[LX_IF_OR] = "||",
		[LX_REDIR_APPEND] = ">>",
		[LX_REDIR_IN] = "<",
		[LX_REDIR_OUT] = ">",
		[LX_REDIR_SOURCE] = "<<",
		[LX_EXP_FIELD] = "QUOTES \"",
		[LX_FIELD] = "QUOTES \'",
		[LX_PARN_L] = "(",
		[LX_PARN_R] = ")",
		[LX_PIPE] = "PIPE"
	};
	return (names[type]);
}

#include <stdio.h>

pid_t	_prs_handle_group(t_llist *group, t_info *info)
{
	t_llist	*expanded;

	expanded = _prs_expand(group, info);
	if (expanded)
	{
		printf("[parser3.c] EXPAND OK\n");
		for (t_ll_elem *h = expanded->head; h != NULL; h = h->next)
			printf("%10s | %s\n", _lx_get_name((int)h->key) , h->val);
		llist_free(expanded);
	}
	return (-1);
}

int	_prs_logexec(t_llist *groups, t_info *info)
{
	t_ll_elem	*ptr;
	int			expect;
	int			mode;
	int			sig;
	pid_t		pid;

	expect = 0;
	ptr = groups->head;
	while(NULL != ptr)
	{
		printf("[parser3.c] PRS_LOGEXEC EXPECT %d\n", expect);
		if (expect == 0)
		{
			pid = _prs_handle_group(ptr->val, info);
			if (pid < 0)
				return (1);
			expect = 1;
		}
		else
		{
			mode = (int)(ptr->key);
			if (pid > 0)
				waitpid(pid, &sig, 0);
			if (pid > 0 && ((WEXITSTATUS(sig) == 0 && mode == LX_IF_OR)
				|| (WEXITSTATUS(sig) != 0 && mode == LX_IF_AND)))
				return (0);
			expect = 0;
		}
		ptr = ptr->next;
	}
	return (0);
}
