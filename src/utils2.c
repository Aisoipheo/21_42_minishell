/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 16:03:06 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/12/08 13:43:24 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "utils.h"

int	ft_strlen(const char *s)
{
	int		i;

	i = 0;
	while (s && s[i])
		++i;
	return (i);
}

char	*ft_strcpy(const char *s)
{
	char		*copy;
	const int	length = ft_strlen(s);
	int			i;

	if (!s)
		return (NULL);
	copy = (char *)malloc(sizeof(char) * (length + 1));
	if (copy)
	{
		i = length;
		copy[length] = '\0';
		while (i--)
			copy[i] = s[i];
	}
	return (copy);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (-1);
	if (!s2)
		return (1);
	while (*s1 && *s2 && *s1 == *s2)
		s1 += (++s2 || 1);
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	ft_strslice(const char *s, const char *delim, char **a, char **b)
{
	int		j;
	int		i;

	if (!s || !delim)
		return (-1);
	j = 0;
	while (s[j] && !ft_strcontains(delim, s[j]))
		++j;
	i = j;
	*a = (char *)malloc(sizeof(char) * (j + 1));
	if (!a)
		return (1);
	while (i--)
		(*a)[i] = s[i];
	(*a)[j] = '\0';
	s += j + 1;
	*b = ft_strcpy(s);
	if (*b)
		return (0);
	free(*a);
	return (1);
}

char	*ft_strjoin2(const char *s1, const char *s2, char d, char isd)
{
	const int	size = ft_strlen(s1) + ft_strlen(s2) + 1 + isd != 0;
	char		*str;
	int			i;

	str = (char *)malloc(sizeof(char) * size);
	if (!str)
		return (NULL);
	i = 0;
	while (s1 && *s1)
		str[i++] = *s1++;
	if (isd)
		str[i] = d;
	while (s2 && *s2)
		str[i++] = *s2++;
	str[i] = '\0';
	return (str);
}
