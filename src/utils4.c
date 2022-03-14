/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 13:02:40 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/14 20:16:53 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "minishell.h"

int	ft_error(int ret, const char *msg, char liberr, int g_e)
{
	static const char	*invcall = "*** THIS SHOULD NOT OCCUR ***";

	if (msg && *msg && !liberr)
	{
		write(STDERR_FILENO, msg, ft_strlen(msg));
		write(STDERR_FILENO, "\n", 1);
		g_exit = g_e;
		return (ret);
	}
	if (errno != 0 && liberr)
	{
		if (msg && *msg)
			perror(msg);
		else
			perror("minishell: ");
		g_exit = errno;
	}
	else if (errno != 0)
		perror(invcall);
	return (ret);
}

int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

char	*ft_itoa(int n)
{
	char	*s;
	int		cnt;
	int		i;

	cnt = 0;
	i = n;
	while (i)
	{
		++cnt;
		i /= 10;
	}
	i = n < 0;
	cnt += (cnt == 0);
	s = (char *)malloc(sizeof(char) * (cnt + 1 + i));
	if (NULL == s)
		return (NULL);
	s[cnt + i] = '\0';
	if (n < 0)
		s[0] = '-';
	while (cnt--)
	{
		s[cnt + i] = '0' + ft_abs(n % 10);
		n /= 10;
	}
	return (s);
}

char	*ft_uitoa(unsigned int n)
{
	char			*s;
	unsigned int	cnt;
	unsigned int	i;

	cnt = 0;
	i = n;
	while (i)
	{
		++cnt;
		i /= 10;
	}
	if (cnt == 0)
		cnt = 1;
	s = (char *)malloc(sizeof(char) * (cnt + 1));
	if (NULL == s)
		return (NULL);
	s[cnt] = '\0';
	while (cnt--)
	{
		s[cnt] = '0' + n % 10;
		n /= 10;
	}
	return (s);
}

int	remap_fds(int in, int out)
{
	if (in != STDIN_FILENO)
	{
		if (dup2(in, STDIN_FILENO) == -1)
			ft_error(1, "minishell: dup2: mapping to (stdin)", 1, 0);
		if (close(in) == -1)
			ft_error(1, "minishell: close: mapping to (stdin)", 1, 0);
		debug_log("remap close1 %d\n", in);
	}
	if (out != STDOUT_FILENO)
	{
		if (dup2(out, STDOUT_FILENO) == -1)
			ft_error(1, "minishell: dup2: mapping to (stdout)", 1, 0);
		if (close(out) == -1)
			ft_error(1, "minishell: close: mapping to (stdout)", 1, 0);
		debug_log("remap close2 %d\n", out);
	}
	return (0);
}

// DEBUG
#include <stdarg.h>
void	debug_log(const char *fmt, ...) {
	va_list	args;

	va_start(args, fmt);
	write(STDERR_FILENO, DEBUG_COLOUR, 8);
	vfprintf(stderr, fmt, args);
	write(STDERR_FILENO, RESET_COLOUR, 5);
	va_end(args);
}
