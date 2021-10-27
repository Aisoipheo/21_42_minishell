/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avl_bst3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 16:31:35 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/27 12:25:21 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "avl_bst.h"

static void	_avl_rotate_left(t_avl_node *r, t_avl_node *p)
{
	t_avl_node	*t;

	t = p->left;
	p->parent = r->parent;
	if (r->parent && r->parent->right == r)
		r->parent->right = p;
	if (r->parent && r->parent->left == r)
		r->parent->left = p;
	r->parent = p;
	p->left = r;
	r->right = t;
	if (t)
		t->parent = r;
	if (p->__weight == 0)
	{
		p->__weight = -1;
		r->__weight = 1;
		return ;
	}
	p->__weight = 0;
	r->__weight = 0;
}

static void _avl_rotate_right(t_avl_node *r, t_avl_node *p)
{
	t_avl_node	*t;

	t = p->right;
	p->parent = r->parent;
	if (r->parent && r->parent->right == r)
		r->parent->right = p;
	if (r->parent && r->parent->left == r)
		r->parent->left = p;
	r->parent = p;
	p->right = r;
	r->left = t;
	if (t)
		t->parent = r;
	if (p->__weight == 0)
	{
		p->__weight = -1;
		r->__weight = 1;
		return ;
	}
	p->__weight = 0;
	r->__weight = 0;
}

void	_avl_balance(t_avl_node *node)
{
	if (node->__weight == 2)
	{

	}
	if (node->__weight == -2)
	{

	}
}

int	avl_insert(t_avl_tree *tree, void *data)
{
	int			cmp;
	t_avl_node	*ptr;
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
	ptr = tree->root;
	while(1)
	{
		cmp = tree->compr(ptr->data, data);
		if (cmp < 0)
		{
			if (ptr->right)
				ptr = ptr->right;
			else
				break ;
		}
		else
			ptr = ptr->left;
	}
	return (0);
}
