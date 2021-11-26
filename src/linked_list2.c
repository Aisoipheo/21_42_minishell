/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:40:35 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/11/26 10:37:55 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "linked_list.h"

int	llist_del(t_llist *list, const void *key)
{
	t_ll_elem	*ptr;
	t_ll_elem	*prev;

	if (!list)
		return (1);
	if (list->size == 0)
		return (0);
	ptr = list->head;
	prev = NULL;
	while (ptr)
	{
		if (list->key_cmpr(key, ptr->key) == 0)
		{
			if (prev)
				prev->next = ptr->next;
			list->key_dstr(ptr->key);
			list->val_dstr(ptr->val);
			free(ptr);
			--(list->size);
			return (0);
		}
		prev = ptr;
		ptr = ptr->next;
	}
	return (0);
}

void	*llist_getval(t_llist *list, const void *key)
{
	t_ll_elem	*ptr;

	if (!list || list->size == 0)
		return (NULL);
	ptr = list->head;
	while (ptr)
	{
		if (list->key_cmpr(key, ptr->key) == 0)
			return (ptr->val);
		ptr = ptr->next;
	}
	return (NULL);
}
