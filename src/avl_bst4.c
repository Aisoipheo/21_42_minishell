/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avl_bst4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 14:03:49 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/27 16:11:01 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "avl_bst.h"

static void	_avl_bst_insert(t_avl_node *root, t_avl_node *new_node,
	t_avl_data_cmp_ptr compr)
{
	while (1)
	{
		if (compr(root->data, new_node->data) < 0)
		{
			if (root->right)
				root = root->right;
			else
			{
				root->right = new_node;
				new_node->parent = root;
				return ;
			}
		}
		else
		{
			if (root->left)
				root = root->left;
			else
			{
				root->left = new_node;
				new_node->parent = root;
				return ;
			}
		}
	}
}

int	avl_insert(t_avl_tree *tree, void *data)
{
	t_avl_node	*tmp;

	tmp = _avl_new_node(data);
	if (!tmp)
		return (1);
	++tree->size;
	if (!tree->root)
	{
		tree->root = tmp;
		return (0);
	}
	_avl_bst_insert(tree->root, tmp, tree->compr);
	_avl_increase_heights(tree, tmp);
	return (0);
}

void	_avl_increase_heights(t_avl_tree *tree, t_avl_node *new_node)
{
	t_avl_node	*ptr;

	ptr = new_node->parent;
	while(NULL != ptr)
	{
		if (new_node == ptr->right)
			++ptr->__weight;
		else
			--ptr->__weight;
		if (ptr->__weight == 2 || ptr->__weight == -2 || ptr->__weight == 0)
		{
			if (ptr->__weight)
				_avl_balance(tree, ptr, new_node);
			return ;
		}
		new_node = ptr;
		ptr = new_node->parent;
	}
}

void	avl_del_node(t_avl_tree *tree, void *data)
{

}
