/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:32:20 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 22:14:28 by rdrizzle         ###   ########.fr       */
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

static char	*_ft_readline(const char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (line && *line)
		add_history(line);
	return (line);
}

//debug only
// const char	*_lx_get_name(int type)
// {
// 	static const char *names[LX_NCONST] =
// 	{
// 		[LX_SEP] = "SEP",
// 		[LX_WORD] = "WORD",
// 		[LX_IF_AND] = "&&",
// 		[LX_IF_OR] = "||",
// 		[LX_REDIR_APPEND] = ">>",
// 		[LX_REDIR_IN] = "<",
// 		[LX_REDIR_OUT] = ">",
// 		[LX_REDIR_SOURCE] = "<<",
// 		[LX_EXP_FIELD] = "QUOTES \"",
// 		[LX_FIELD] = "QUOTES \'",
// 		[LX_PARN_L] = "(",
// 		[LX_PARN_R] = ")",
// 		[LX_PIPE] = "PIPE"
// 	};
// 	return (names[type]);
// }

void	_main_pre(t_llist **tokens, char **line, char *argv[], int argc)
{
	(void)argv;
	(void)argc;
	signal(SIGINT, handler);
	errno = 0;
	*tokens = llist_new(llist_int_kcmp, NULL, free);
	*line = _ft_readline(MINIS_PROMPT);
}

void	_main_post(t_llist *tokens)
{
	llist_free(tokens);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;
	t_info	info;
	t_llist	*tokens;

	if (ft_init(&info, envp))
		return (EXIT_FAILURE);
	while (info.exit_f)
	{
		_main_pre(&tokens, &line, argv, argc);
		if (line && *line)
		{
			if (lx_lexer(tokens, line) == 0)
				prs_parse(tokens, &info);
			free(line);
		}
		else if (line == NULL)
		{
			info.exit_f = 0;
			write(STDOUT_FILENO, "exit\n", 5);
		}
		_main_post(tokens);
	}
	ft_destroy(&info);
	return (g_exit);
}
