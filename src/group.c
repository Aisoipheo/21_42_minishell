/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 13:38:12 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/02/26 19:17:11 by gmckinle         ###   ########.fr       */
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
	if (NULL == g->cmds)
	{
		free(g);
		return (NULL);
	}
	return (g);
}

void	ft_group_free(void *group)
{
	llist_free(((t_group *)group)->cmds);
	free(group);
}
