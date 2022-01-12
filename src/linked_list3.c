/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 13:27:42 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/12/06 13:32:39 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	llist_int_kcmp(const void *k1, const void *k2)
{
	if ((int)k1 == (int)k2)
		return (0);
	if ((int)k1 < (int)k2)
		return (-1);
	return (1);
}

int	llist_str_kcmp(const void *k1, const void *k2)
{
	return (ft_strcmp((const char *)k1, (const char *)k2));
}
