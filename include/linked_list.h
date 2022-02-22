/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:00:46 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/02/22 17:21:00 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINKED_LIST_H
# define LINKED_LIST_H

/*
		comparator rules:
			>0 if (k1 > k2)
			0 if (k1 == k2)
			<0 if (k1 < k2)
*/
typedef int (*	t_llist_key_comparator_ptr)(const void	*k1,const void *k2);
typedef void (*	t_llist_key_destructor_ptr)(void	*key);
typedef void (*	t_llist_val_destructor_ptr)(void	*val);

typedef struct s_llist_element
{
	void	*key;
	void	*val;
	void	*next;
}	t_ll_elem;

typedef struct s_linked_list
{
	t_ll_elem					*head;
	t_llist_key_comparator_ptr	key_cmpr;
	t_llist_key_destructor_ptr	key_dstr;
	t_llist_val_destructor_ptr	val_dstr;
	unsigned int				size;
}	t_llist;

t_llist		*llist_new(t_llist_key_comparator_ptr key_cmpr,
				t_llist_key_destructor_ptr key_dstr,
				t_llist_val_destructor_ptr val_dstr);
void		llist_free(t_llist *list);
int			llist_set(t_llist *list, void *key, void *new_val);
int			llist_push(t_llist *list, void *key, void *val);
int			llist_del(t_llist *list, const void *key);
int			llist_haskey(t_llist *list, const void *key);
void		*llist_getval(t_llist *list, const void *key);
void		llist_free_wrapper(void *llist);

int			llist_int_kcmp(const void *k1, const void *k2);
int			llist_str_kcmp(const void *k1, const void *k2);

#endif
