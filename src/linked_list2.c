/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:40:35 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/02/22 17:23:08 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "linked_list.h"

static void	_llist_destroy_elem(t_llist *list, t_ll_elem *elem, t_ll_elem *prev)
{
	if (prev)
		prev->next = elem->next;
	else
		list->head = elem->next;
	if (NULL != list->key_dstr)
		list->key_dstr(elem->key);
	if (NULL != list->val_dstr)
		list->val_dstr(elem->val);
	free(elem);
	--(list->size);
}

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
			_llist_destroy_elem(list, ptr, prev);
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

int	llist_haskey(t_llist *list, const void *key) {
	t_ll_elem	*ptr;

	ptr = list->head;
	while (ptr) {
		if (list->key_cmpr(ptr->key, key) == 0)
			return (1);
		ptr = ptr->next;
	}
	return (0);
}
