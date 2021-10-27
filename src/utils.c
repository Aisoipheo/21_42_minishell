/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 14:56:16 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/09 15:06:42 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "utils.h"

int	ft_strcontains(const char *s, const char c)
{
	if (s)
	{
		while (*s && *s != c)
			++s;
		if (*s == c)
			return (1);
	}
	return (0);
}

static int	_ft_wcnt(const char *s, const char *delim)
{
	int		cnt;

	cnt = 0;
	if (s)
	{
		while (*s)
		{
			while (*s && ft_strcontains(delim, *s))
				++s;
			if (*s && !ft_strcontains(delim, *s))
				++cnt;
			while (*s && !ft_strcontains(delim, *s))
				++s;
		}
	}
	return (cnt);
}

static int	_ft_wlen(const char *s, const char *delim)
{
	int		len;

	len = 0;
	if (s)
	{
		while (*s)
		{
			while (*s && ft_strcontains(delim, *s))
				++s;
			while (s[len] && !ft_strcontains(delim, s[len]))
				++len;
		}
	}
	return (len);
}

char	**_ft_strdestroy2(char **str2)
{
	int		j;

	j = 0;
	while (str2[j])
		free(str2[j++]);
	free(str2);
	return (NULL);
}

char	**ft_strsplit(const char *s, const char *d)
{
	char	**ret;
	int		j;
	int		i;

	ret = (char **)malloc(sizeof(char *) * (_ft_wcnt(s, d) + 1));
	if (!ret)
		return (NULL);
	j = 0;
	while (*s)
	{
		while (*s && ft_strcontains(d, *s))
			++s;
		if (*s)
			ret[j] = (char *)malloc(sizeof(char) * (_ft_wlen(s, d) + 1));
		if (*s && !ret[j])
			return (_ft_strdestroy2(ret));
		i = 0;
		while (*s && !ft_strcontains(d, *s))
			ret[j][i++] = s;
		ret[j][i] = '\0';
	}
	ret[j] = NULL;
	return (ret);
}
