/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avl_bst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 13:44:34 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/13 16:13:59 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "avl_bst.h"

t_avl_tree	*avl_new_tree(
				t_avl_data_cmp_ptr comparator,
				t_avl_data_dstr_ptr destructor
				)
{
	t_avl_tree	*tree;

	if (NULL == comparator || NULL == destructor)
		return (NULL);
	tree = (t_avl_tree *)malloc(sizeof(t_avl_tree));
	if (!tree)
		return (NULL);
	tree->size = 0;
	tree->compr = comparator;
	tree->dstr = destructor;
	return (tree);
}

void	*avl_get_val(t_avl_iter it)
{
	return (it->data);
}

static t_avl_node	*_avl_leftmost(t_avl_node *node)
{
	if (!node)
		return (NULL);
	while (node->left)
		node = node->left;
	return (node);
}

t_avl_iter	avl_begin(t_avl_tree *tree)
{
	return (_avl_leftmost(tree->root));
}

t_avl_iter	avl_next(t_avl_iter it)
{
	if (it->right)
		return (_avl_leftmost(it->right));
	while (it->parent && (it == it->parent->right))
		it = it->parent;
	return (it->parent);
}
