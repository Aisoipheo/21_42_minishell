/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 13:02:40 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/02 17:17:56 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "minishell.h"

int	ft_error(int ret, const char *msg, char liberr)
{
	static const char	*invcall = "*** THIS SHOULD NOT OCCUR ***";

	g_exit = 1;
	(void)invcall;
	if (msg && *msg && !liberr)
	{
		write(STDERR_FILENO, msg, ft_strlen(msg));
		write(STDERR_FILENO, "\n", 1);
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
