/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avl_bst.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 13:30:06 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/27 12:07:55 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVL_BST_H
# define AVL_BST_H

# define AVL_MAX_SIZE 0xFFFFFFFF

typedef void (*			t_avl_data_dstr_ptr)(void *);
typedef int (*			t_avl_data_cmp_ptr)(void *, void *);
typedef unsigned int	t_avl_size;

typedef struct s_avl_node
{
	struct s_avl_node	*left;
	struct s_avl_node	*right;
	struct s_avl_node	*parent;
	char				__weight;
	void				*data;
}	t_avl_node;

/*
	ITERATOR-TYPE: FORWARD
	NEVER PERFORM MANUAL ACTIONS WITH ITER
*/
typedef t_avl_node		*t_avl_iter;

typedef struct s_avl_tree
{
	t_avl_node			*root;
	t_avl_data_dstr_ptr	dstr;
	t_avl_data_cmp_ptr	compr;
	t_avl_size			size;
}	t_avl_tree;

t_avl_tree	*avl_new_tree(
				t_avl_data_cmp_ptr comparator,
				t_avl_data_dstr_ptr destructor
				);
t_avl_node	*_avl_new_node(void *data);
void		avl_del_tree(t_avl_tree *tree);
int			avl_insert(t_avl_tree *tree, void *data);
void		avl_del_node(t_avl_tree *tree, void *data);
void		*avl_find_val(t_avl_tree *tree, void *data);
t_avl_iter	avl_begin(t_avl_tree *tree);
t_avl_iter	avl_end(t_avl_tree *tree);
t_avl_iter	avl_next(t_avl_iter it);
void		*avl_get_val(t_avl_iter it);
void		_avl_balance(t_avl_node *node);

#endif
