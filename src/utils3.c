/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 13:31:49 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/11/26 10:37:36 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "utils.h"

int	ft_isspace(char c)
{
	return (c == ' '
		|| c == '\n'
		|| c == '\t'
		|| c == '\r'
		|| c == '\v'
		|| c == '\f');
}

char	*ft_substr(const char *s, unsigned long begin, unsigned long n)
{
	char			*str;
	char			fail;
	unsigned int	i;

	str = (char *)malloc(sizeof(char) * (n + 1));
	if (!str)
		return (NULL);
	str[n] = '\0';
	i = 0;
	fail = 0;
	while (s && s[i] && i != begin)
		++i;
	if (i < begin)
		fail = 1;
	while (!fail && s && s[i] && (i - begin < n))
	{
		str[i - begin] = s[i];
		++i;
	}
	while (begin + n != i)
		str[i++] = '\0';
	return (str);
}
