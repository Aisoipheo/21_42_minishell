/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:16:15 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 16:32:40 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "norm_hacks.h"

int	get_in_fd(t_cmd_info *c_info)
{
	if (!c_info->in_file)
		return (STDIN_FILENO);
	return (open(c_info->in_file, O_RDONLY, 0644));
}

int	get_out_fd(t_cmd_info *c_info)
{
	if (!c_info->out_file)
		return (STDOUT_FILENO);
	if (CMD_APPEND & c_info->flags)
		return (open(c_info->out_file, O_CREAT | O_WRONLY | O_APPEND, 0644));
	return (open(c_info->out_file, O_CREAT | O_WRONLY, 0644));
}

int	ft_atoi(const char *s, int *n)
{
	long long int	nb;
	int				i;
	int				op;

	nb = 0;
	i = 0;
	op = 1;
	if (s == NULL || *s == '\0')
		return (1);
	if (*s == '-' || *s == '+')
		++i;
	while (s[i])
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			op = 0;
			nb = nb * 10 + (s[i++] - '0')
				* (int)nh_trop(*s == '-', (void *)-1, (void *)1);
		}
		else
			return (1);
	}
	*n = nb;
	return (op);
}

int	ft_is_valid_id(const char *s)
{
	int		i;
	char	b;

	if (s == NULL || *s == '\0')
		return (0);
	b = 0;
	i = 0;
	while (s[i])
	{
		if (i == 0 && (s[i] == '_'
				|| (s[i] >= 'a' && s[i] <= 'z')
				|| (s[i] >= 'A' && s[i] <= 'Z')))
			b = 1;
		if (s[i] == '_' || ft_isalnum(s[i]))
			++i;
		else
			return (0);
	}
	return (b);
}
