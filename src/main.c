/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:32:20 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/11/26 12:45:53 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "readline/readline.h"
#include "readline/history.h"
#include "minishell.h"
#include "utils.h"
#include "lexer.h"


static char	*_ft_readline(const char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (line && *line)
		add_history(line);
	return (line);
}

static int _ft_cmpr(const void *a, const void *b)
{
	return (ft_strcmp((const char *)a, (const char *)b));
}

static int	_ft_init(t_llist *list, char *envp[])
{
	ft_parse_envp(list, envp);
	return (0);
}

static int _int_cmpr(const void *a, const void *b)
{
	if ((int)a == (int)b)
		return (0);
	if ((int)a < (int)b)
		return (-1);
	return (1);
}

const char	*_lx_get_name(int type)
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

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;
	t_llist	*list;

	(void)argc;
	(void)argv;
	list = llist_new(_ft_cmpr,free,free);
	_ft_init(list, envp);
	while(1)
	{
		t_llist *tokens = llist_new(_int_cmpr, NULL, free);
		line = _ft_readline("prompt > ");
		lx_lexer(tokens, line);
		printf("TOKENS OK\n");
		for (t_ll_elem *h = tokens->head; h != NULL; h = h->next)
			printf("%10s | %s\n", _lx_get_name((int)h->key) , h->val);
		llist_free(tokens);
		free(line);
	}
	rl_clear_history();
	return (0);
}
