/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avl_bst2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 15:54:58 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/13 17:31:57 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "avl_bst.h"

t_avl_iter	avl_end(t_avl_tree *tree)
{
	(void)tree;
	return (NULL);
}

t_avl_node	*_avl_new_node(void *data)
{
	t_avl_node	*node;

	node = (t_avl_node *)malloc(sizeof(t_avl_node));
	if (!node)
		return (NULL);
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->__weight = 0;
	return (node);
}

void	avl_del_tree(t_avl_tree *tree)
{
	t_avl_iter	it;
	t_avl_iter	next;

	if (!tree)
		return ;
	if (tree->size)
	{
		it = avl_begin(tree);
		while(it != avl_end(tree))
		{
			next = avl_next(it);
			tree->dstr(avl_get_value(it));
			free(it);
			it = next;
		}
	}
	free(tree);
}

void	*avl_find_val(t_avl_tree *tree, void *data)
{
	t_avl_node	*ptr;
	int			res;

	ptr = tree->root;
	while(1)
	{
		if (!ptr)
			return (NULL);
		res = tree->compr(ptr->data, data);
		if (res == 0)
			return (ptr->data);
		if (res < 0)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}
	return (NULL);
}
