/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 16:53:04 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 17:13:11 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"
#include "parser.h"

int	_prs_dstr(t_llist *groups)
{
	llist_free(groups);
	return (1);
}

int	_prs_handle_group0(t_llist *groups, t_llist *group, t_ll_elem *ptr,
		long type)
{
	if (group->size && llist_push(groups, (void *)type, group))
	{
		llist_free(group);
		return (ft_error(1, "minishell: _prs_group", 1, 0));
	}
	if (group->size == 0)
		llist_free(group);
	if (NULL != ptr && _prs_is_delim_token(ptr->key)
		&& llist_push(groups, ptr->key, NULL))
		return (ft_error(1, "minishell: _prs_group", 1, 0));
	return (0);
}

int	_prs_handle_group1(t_llist *group)
{
	llist_free(group);
	return (1);
}

t_chunk_info	*_prs_chunk_info_new(unsigned int s, unsigned int e)
{
	t_chunk_info	*ci;

	ci = (t_chunk_info *)malloc(sizeof(t_chunk_info));
	if (ci == NULL)
		return (NULL);
	ci->e = e;
	ci->s = s;
	return (ci);
}

int	_prs_update_g_exit_str(t_info *info)
{
	free(info->g_exit_str);
	info->g_exit_str = ft_itoa(g_exit);
	if (NULL == info->g_exit_str)
		return (ft_error(1, "minishell: _prs_update_g_exit_str", 1, 0));
	return (0);
}
