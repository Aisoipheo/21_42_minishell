/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avl_bst3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 16:31:35 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/27 15:50:53 by rdrizzle         ###   ########.fr       */
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

static void	_avl_rotate_right(t_avl_node *r, t_avl_node *p)
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

static void _avl_rotate_rightLeft(t_avl_node *r, t_avl_node *p)
{
	t_avl_node	*y;
	t_avl_node	*t2;
	t_avl_node	*t3;

	y = p->left;
	t2 = y->left;
	t3 = y->right;
	p->left = t3;
	if (!t3)
		t3->parent = p;
	r->right = t2;
	if (!t2)
		t2->parent = r;
	y->parent = r->parent;
	y->left = r;
	y->right = p;
	p->parent = y;
	r->parent = y;
	r->__weight = 0;
	p->__weight = 0;
	if (y->__weight > 0)
		r->__weight = -1;
	if (y->__weight < 0)
		p->__weight = 1;
	y->__weight = 0;
}

static void	_avl_rotate_leftRight(t_avl_node *r, t_avl_node *p)
{
	t_avl_node	*y;
	t_avl_node	*t2;
	t_avl_node	*t3;

	y = p->left;
	t2 = y->left;
	t3 = y->right;
	p->left = t2;
	if (!t2)
		t2->parent = p;
	r->right = t3;
	if (!t3)
		t3->parent = r;
	y->parent = r->parent;
	y->left = p;
	y->right = r;
	p->parent = y;
	r->parent = y;
	r->__weight = 0;
	p->__weight = 0;
	if (y->__weight > 0)
		p->__weight = -1;
	if (y->__weight < 0)
		r->__weight = 1;
	y->__weight = 0;
}

void	_avl_balance(t_avl_tree *tree, t_avl_node *node, t_avl_node *child)
{
	t_avl_node	*parent;

	parent = node->parent;
	if (node->__weight == 2)
	{
		if (child->__weight < 0)
			_avl_rotate_rightLeft(node, child);
		else
			_avl_rotate_left(node, child);
	}
	if (node->__weight == -2)
	{
		if (child->__weight > 0)
			_avl_rotate_leftRight(node, child);
		else
			_avl_rotate_right(node, child);
	}
	if (parent && parent->right == node)
		parent->right = node->parent;
	if (parent && parent->left == node)
		parent->left = node->parent;
	if (!parent)
		tree->root = node->parent;
}
