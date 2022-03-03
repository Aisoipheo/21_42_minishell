/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 13:38:12 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/03 18:02:18 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "minishell.h"
#include "linked_list.h"

t_group	*ft_group_new(int type)
{
	t_group		*g;

	g = (t_group *)malloc(sizeof(t_group));
	if (!g)
		return (NULL);
	g->type = type;
	g->cmds = llist_new(NULL, llist_free_wrapper, free);
	g->files = llist_new(NULL, NULL, free);
	if (NULL == g->cmds || NULL == g->files)
	{
		if (g->cmds)
			llist_free(g->cmds);
		if (g->files)
			llist_free(g->files);
		free(g);
		return (NULL);
	}
	return (g);
}

void	ft_group_free(void *group)
{
	t_ll_elem	*ptr;

	ptr = ((t_group *)group)->files->head;
	while (ptr)
	{
		unlink(ptr->val);
		ptr = ptr->next;
	}
	llist_free(((t_group *)group)->cmds);
	llist_free(((t_group *)group)->files);
	free(group);
}
