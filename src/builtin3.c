/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 19:49:45 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 20:00:06 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"

void	not_valid_handler(char *a, char *b)
{
	g_exit = 1;
	free(a);
	free(b);
	write(STDERR_FILENO, "minishell: export: not valid\n", 30);
}

int	builtin_handler1(char *pwdcopy, char *pathcopy)
{
	free(pathcopy);
	free(pwdcopy);
	return (ft_error(-1, "minishell: cd: malloc", 1, 0));
}
