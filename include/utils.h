/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 14:54:47 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/01/12 13:35:00 by rdrizzle         ###   ########.fr       */
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
char	*ft_strjoin2(const char *s1, const char *s2, char d, char isd);
int		ft_isspace(char c);
char	*ft_substr(const char *s, unsigned long begin, unsigned long count);
int		ft_isalnum(char c);
void	*ft_free_char2dem(char **arr, int m);
char	**ft_malloc_char2dem(int m, int n);
int		ft_error(int ret, const char *msg, char liberr);

#endif
