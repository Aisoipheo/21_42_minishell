/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/05 17:28:22 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/05 17:37:39 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"
#include "utils.h"

static	int	read_until_nl(char	**line, t_info *info)
{
	t_llist		*b;
	char		buff[256];

	*line = NULL;
	b = llist_new(NULL, NULL, free);
	if (NULL == b)
		return (ft_error(1, "minishell: get_line", 1));
	while (info->c)
	{
		if (read(STDIN_FILENO, buff, 256) == -1)
	}
	return (0);
}

int	get_line(const char *prompt, char **line, t_info *info)
{
	if (write(STDOUT_FILENO, prompt, ft_strlen(prompt)) == -1)
		return (ft_error(1, "minishell: get_line", 1));
	return (read_until_nl(line, info));
}
