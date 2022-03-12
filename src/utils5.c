/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:16:15 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/12 15:21:25 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	get_in_fd(t_cmd_info *c_info, t_llist *files)
{
	if (!c_info->in_file)
		return (0);
	if (c_info->flags & CMD_INSOURCE && create_heredoc(c_info, files))
		return (-1);
	return (open(c_info->in_file, O_RDONLY, 0644));
}

int	get_out_fd(t_cmd_info *c_info)
{
	if (!c_info->out_file)
		return (1);
	if (CMD_APPEND & c_info->flags)
		return (open(c_info->out_file, O_CREAT | O_WRONLY | O_APPEND, 0644));
	return (open(c_info->out_file, O_CREAT | O_WRONLY, 0644));
}
