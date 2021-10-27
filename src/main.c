/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:32:20 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/11 12:58:35 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "readline/readline.h"
#include "readline/history.h"
#include "minishell.h"

static char	*_ft_readline(const char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (line && *line)
		add_history(line);
	return (line);
}

// static int	_ft_init();

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;

	(void)argc;
	(void)argv;
	// _ft_init();
	while(1)
	{
		line = _ft_readline("&> ");
		free(line);
	}
	// rl_clear_history();
	return (0);
}
