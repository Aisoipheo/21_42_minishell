/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:01:18 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/11/25 14:50:49 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "linked_list.h"

t_llist	*llist_new(t_llist_key_comparator_ptr key_cmpr,
			t_llist_key_destructor_ptr key_dstr,
			t_llist_val_destructor_ptr val_dstr)
{
	t_llist	*list;

	list = (t_llist *)malloc(sizeof(t_llist));
	if (!list)
		return (NULL);
	list->head = NULL;
	list->key_cmpr = key_cmpr;
	list->key_dstr = key_dstr;
	list->val_dstr = val_dstr;
	list->size = 0;
	return (list);
}

void	llist_free(t_llist *list)
{
	t_ll_elem	*elem;
	t_ll_elem	*elem_next;

	if (!list)
		return ;
	elem = list->head;
	while (elem != NULL)
	{
		elem_next = elem->next;
		if (NULL != list->key_dstr)
			list->key_dstr(elem->key);
		if (NULL != list->val_dstr)
			list->val_dstr(elem->val);
		free(elem);
		elem = elem_next;
	}
	free(list);
}

static t_ll_elem	*_new_ll_elem(void *key, void *val)
{
	t_ll_elem	*elem;

	elem = (t_ll_elem *)malloc(sizeof(t_ll_elem));
	if (!elem)
		return (NULL);
	elem->key = key;
	elem->val = val;
	elem->next = NULL;
	return (elem);
}

int	llist_push(t_llist *list, void *key, void *val)
{
	t_ll_elem	*ptr;

	if (!list)
		return (1);
	if (!list->head)
	{
		list->head = _new_ll_elem(key, val);
		if (list->head)
			++(list->size);
		return (list->head == NULL);
	}
	ptr = list->head;
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = _new_ll_elem(key, val);
	if (ptr->next)
		++(list->size);
	return (ptr->next == NULL);
}

int	llist_set(t_llist *list, void *key, void *new_val)
{
	t_ll_elem	*ptr;

	if (!list)
		return (1);
	if (list->size == 0)
		return (llist_push(list, key, new_val));
	ptr = list->head;
	while (1)
	{
		if (list->key_cmpr(key, ptr->key) == 0)
		{
			if (NULL != list->val_dstr)
				list->val_dstr(ptr->val);
			ptr->val = new_val;
			return (0);
		}
		if (ptr->next)
			ptr = ptr->next;
		else
			break ;
	}
	ptr->next = _new_ll_elem(key, new_val);
	if (ptr->next)
		++(list->size);
	return (ptr->next == NULL);
}
