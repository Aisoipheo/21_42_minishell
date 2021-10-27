/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 14:54:47 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/13 16:24:58 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

int		ft_strlen(const char *s);
char	*ft_strcpy(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
char	**ft_strsplit(const char *s, const char *delim);
int		ft_strcontains(const char *s, const char c);
int		ft_strslice(const char *s, const char *delim, char **a, char **b);
char	**_ft_strdestroy2(char **str2);
char	*ft_strjoin2(const char *s1, const char *s2, char delim);
int		ft_isspace(char c);

#endif
