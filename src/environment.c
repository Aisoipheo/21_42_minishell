/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 14:22:18 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/01/19 18:49:34 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "minishell.h"
#include "linked_list.h"
#include "utils.h"

int	ft_parse_envp(t_llist *l, char *envp[])
{
	char			*key;
	char			*val;
	unsigned int	i;

	if (!envp || !*envp || !l)
		return (-1);
	i = 0;
	while (envp[i])
	{
		if (ft_strslice(envp[i], "=", &key, &val))
			return (ft_error(1, "minishell: ft_parse_envp", 1));
		if (llist_push(l, key, val))
			return (ft_error(1, "minishell: ft_parse_envp", 1));
		++i;
	}
	return (0);
}

char	**ft_compose_envp(t_llist *env)
{
	char			**envp;
	unsigned int	i;
	t_ll_elem		*ptr;

	envp = (char **)malloc(sizeof(char *) * (env->size + 1));
	if (!envp)
		return (NULL);
	i = env->size;
	ptr = env->head;
	envp[i] = NULL;
	while (i--)
	{
		envp[i] = ft_strjoin2(ptr->key, ptr->val, '=', 1);
		if (!envp[i])
			return (_ft_strdestroy2(envp));
		ptr = ptr->next;
	}
	return (envp);
}
