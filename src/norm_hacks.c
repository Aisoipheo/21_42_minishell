/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_hacks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 13:50:09 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/11/25 13:52:10 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "norm_hacks.h"

void	*nh_trop(int a, void *b, void *c)
{
	if (a)
		return (b);
	return (c);
}
