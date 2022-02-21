/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:32:20 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/02/21 18:11:12 by gmckinle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "readline/readline.h"
#include "readline/history.h"
#include "minishell.h"
#include "utils.h"
#include "lexer.h"
#include "errno.h"
#include "parser.h"


static char	*_ft_readline(const char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (line && *line)
		add_history(line);
	return (line);
}

static int	_ft_init(t_info *info, char *envp[])
{
	errno = 0;
	info->envp_list = llist_new(llist_str_kcmp, free, free);
	if (info->envp_list == NULL)
		return (ft_error(1, "minishell: ft_init", 1));
	if (ft_parse_envp(info->envp_list, envp))
		return (1);
	
	return (0);
}

//debug only
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
	t_info	info;
	t_llist	*tokens;

	(void)argc;
	(void)argv;
	if (_ft_init(&info, envp))
		return (EXIT_FAILURE);
	while(1)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		tokens = llist_new(llist_int_kcmp, NULL, free);
		line = _ft_readline("minishell$: ");
		if (line && *line)
		{
			if (lx_lexer(tokens, line) == 0)
			{
				printf("[main.c] TOKENS OK\n");
				for (t_ll_elem *h = tokens->head; h != NULL; h = h->next)
					printf("%10s | %s\n", _lx_get_name((int)h->key) , h->val);
				prs_parse(tokens, &info);
			}
			//
			llist_free(tokens);
			free(line);
		}
		if (!line)
			handler_term(&info);
	}
	rl_clear_history();
	return (EXIT_SUCCESS);
}
