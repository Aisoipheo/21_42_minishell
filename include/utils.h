/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 14:54:47 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/14 19:58:40 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

int		ft_strlen(const char *s);
char	*ft_strcpy(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
char	**ft_strsplit(const char *s, const char *delim);
int		ft_strcontains(const char *s, const char c);
int		ft_strslice(const char *s, const char *delim, char **a, char **b);
char	**_ft_strdestroy2(char **str2, int j);
char	*ft_strjoin2(const char *s1, const char *s2, char d, char isd);
int		ft_isspace(char c);
char	*ft_substr(const char *s, unsigned long begin, unsigned long count);
int		ft_isalnum(char c);
void	*ft_free_char2dem(char **arr, int m);
char	**ft_malloc_char2dem(int m, int n);
int		ft_error(int ret, const char *msg, char liberr, int g_e);
char	*ft_uitoa(unsigned int n);
int		ft_abs(int n);
char	*ft_itoa(int n);
int		remap_fds(int in, int out);
int		get_in_fd(t_cmd_info *c_info, t_llist *files);
int		get_out_fd(t_cmd_info *c_info);
int		ft_atoi(const char *s, int *n);
int		ft_is_valid_id(const char *s);

#endif
