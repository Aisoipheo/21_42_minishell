/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:32:20 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/09 17:58:45 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "readline/readline.h"
#include "readline/history.h"
#include "minishell.h"
#include "utils.h"
#include "lexer.h"
#include "parser.h"

int	g_exit;

static char	*_ft_readline(const char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (line && *line)
		add_history(line);
	return (line);
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
	if (ft_init(&info, envp))
		return (EXIT_FAILURE);
	while(info.exit_f)
	{
		debug_log("[main.c:70] EXIT STATUS: %d\n", g_exit);
		errno = 0;
		tokens = llist_new(llist_int_kcmp, NULL, free);
		line = _ft_readline("prompt > ");
		debug_log("[main.c:74] EXIT STATUS: %d\n", g_exit);
		if (line && *line)
		{
			if (lx_lexer(tokens, line) == 0)
			{
				debug_log("[main.c] TOKENS OK\n");
				debug_log("[main.c:80] EXIT STATUS: %d\n", g_exit);
				for (t_ll_elem *h = tokens->head; h != NULL; h = h->next)
					debug_log("%10s | %s\n", _lx_get_name((int)h->key) , h->val);
				prs_parse(tokens, &info);
				debug_log("[main.c:84]EXIT STATUS: %d\n", g_exit);
			}
			free(line);
			debug_log("[main.c] *** END OF WORK ***\n");
		} else if (line == NULL) {
			info.exit_f = 0;
			write(STDOUT_FILENO, "exit\n", 5);
		}
		llist_free(tokens);
	}
	ft_destroy(&info);
	debug_log("PPID: %d PID: %d\n", getppid(), getpid());
	return (EXIT_SUCCESS);
}
