/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 14:22:18 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/13 12:53:30 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "minishell.h"
#include "var_list.h"
#include "utils.h"

int	ft_parse_envp(t_c_list *l, const char *envp[])
{
	t_env_var	*ev;
	t_c_size	i;
	char		*key;
	char		*val;

	if (!envp || !*envp || !l)
		return (-1);
	i = 0;
	while(envp[i])
	{
		if (ft_strslice(envp[i], "=", &key, &val))
			return (1);
		ev = _vl_new_var(key, val);
		free(key);
		free(val);
		if (!ev)
			return (1);
		ev->attr = 1;
		if (c_list_pb(l, ev, _vl_destr_env_var) == NULL)
			return (1);
		++i;
	}
}

char	**ft_compose_envp(t_c_list *env)
{
	char		**envp;
	t_c_size	i;
	t_c_it		*it;

	envp = (char **)malloc(sizeof(char *) * (c_list_size(env) + 1));
	if (!envp)
		return (NULL);
	i = c_list_size(env);
	it = c_list_begin(env);
	envp[i] = NULL;
	while(i--)
	{
		envp[i] = ft_strjoin2(((t_env_var *)it->data)->key,
								((t_env_var *)it->data)->val, '=');
		if (!envp[i])
			return (_ft_strdestroy2(envp));
		it = c_next(it);
	}
	return (envp);
}
