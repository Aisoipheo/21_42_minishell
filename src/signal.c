/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 19:02:12 by gmckinle          #+#    #+#             */
/*   Updated: 2022/03/14 16:56:30 by gmckinle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"

/*	Системный вызов kill используется для отправки сигнала определенному процессу или группе процессов.
- Если pid положительный, сигнал sig отправляется на pid.
- Если pid равен 0, сигнал sig отправляется всем процессам в группе процессов, к которой принадлежит текущий процесс.
- Если pid равен -1, все процессы, кроме процесса 1, отправляют сигнал sig.
- Если pid меньше -1, сигнал sig отправляется процессам всех групп, включая процесс -pid.
- Если sig равен 0, сигнал не отправляется, но возможна проверка ошибок. */

/*	ctrl+D => EOF (read return 0?)
	ctrl+C => signal SIGINT to process which must to stop
	SIGINT – сигнал-прерывание.
	both actions are about to stop prog

	◦ ctrl-C print a new prompt on a newline.
	◦ ctrl-D exit the shell.
	◦ ctrl-\ do nothing. (SIGQUIT)	*/

void	handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_redisplay();
	write(1, "  \b\b\n", 5);
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
	// g_error = 1;
}

void	handler_term(t_info *info)
{
	(void)info;
	write(1, "minishell$ exit\n", ft_strlen("minishell$ exit\n"));
	rl_redisplay();
	//destroy(cmds, info);
	exit(EXIT_SUCCESS);
}

void	handler_in_executor(int sig)
{
	if (sig == SIGINT)
	{
		(void)sig;
		write(2, "\n", 1);
		// g_var = 130;
	}
	else if (sig == SIGQUIT)
	{
		(void)sig;
		write(2, "Quit: 3\n", 8);
		// g_var = 131;
	}
}


void	handler_in_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		(void)sig;
		write(2, "\n", 1);
		// g_var = 130;
		exit(130);
	}
	else if (sig == SIGQUIT)
	{
		(void)sig;
		write(2, "Quit: 3\n", 8);
		// g_var = 131;
		exit(131);
	}
}
