/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 12:54:16 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/11 14:49:44 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "tokenizer.h"
#include "utils.h"

static int	_tkz_tokencnt(const char *line)
{

}

int	tkz_tokenize(const char *line)
{
	if (!line)
		return (0);
	while(*line)
	{
		while(ft_isspace(*line))
			++line;

	}
}
