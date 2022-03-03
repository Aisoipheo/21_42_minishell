/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:54:25 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/03 18:17:45 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "minishell.h"
#include "utils.h"

int	ft_echo(t_llist *args, t_info *info)
{
	t_ll_elem	*arg;
	char		is_n;

	(void)info;
	// (void)args;
	arg = args->head;
	is_n = 0;
	if (args->size > 1)
	{
		arg = arg->next;
		is_n = ft_strcmp(arg->val, "-n") == 0;
		if (is_n)
			arg = arg->next;
		while (arg)
		{
			if (write(STDOUT_FILENO, arg->val, ft_strlen((char *)arg->val))
				== -1)
				return (ft_error(-1, "minishell: echo", 1));
			if (arg->next && write(STDOUT_FILENO, " ", 1) == -1)
				return (ft_error(-1, "minishell: echo", 1));
			arg = arg->next;
		}
	}
	if (!is_n)
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (ft_error(-1, "minishell: echo", 1));
	return (0);
}

int	ft_cd(t_llist *args, t_info *info)
{
	t_ll_elem	*arg;
	char		*path;
	char		*pathcopy;
	char		*pwdcopy;

	arg = args->head;
	if (arg->next)
		path = (char *)((t_ll_elem *)(arg->next))->val;
	else
		path = (char *)llist_getval(info->envp_list, "HOME");
	if (chdir(path) == -1)
		return (ft_error(-1, "minishell: cd: chdir", 1));
	pathcopy = ft_strcpy(path);
	path = (char *)llist_getval(info->envp_list, "PWD");
	pwdcopy = ft_strcpy(path);
	if (pathcopy == NULL || (path && pwdcopy == NULL))
	{
		free(pathcopy);
		return (ft_error(-1, "minishell: cd: malloc", 1));
	}
	if (path && (llist_set(info->envp_list, "OLDPWD", pwdcopy)
			|| llist_set(info->envp_list, "PWD", pathcopy)))
		return (ft_error(-1, "minishell: cd: malloc", 1));
	info->envp_f = 1;
	return (0);
}

int	ft_pwd(t_llist *args, t_info *info)
{
	char			pwd[PATH_MAX];
	unsigned int	i;

	(void)info;
	(void)args;
	getcwd(pwd, PATH_MAX);
	i = 0;
	while (i < PATH_MAX && pwd[i])
		++i;
	if (i < PATH_MAX)
		pwd[i++] = '\n';
	if (write(STDOUT_FILENO, pwd, i) == -1)
		return (ft_error(-1, "minishell: pwd: getcwd", 1));
	return (0);
}

int	ft_env(t_llist *args, t_info *info)
{
	t_ll_elem	*ptr;

	(void)args;
	ptr = info->envp_list->head;
	while (ptr)
	{
		if (write(STDOUT_FILENO, ptr->key, ft_strlen((char *)ptr->key)) == -1)
			return (ft_error(-1, "minishell: env: write", 1));
		if (write(STDOUT_FILENO, "=", 1) == -1)
			return (ft_error(-1, "minishell: env: write", 1));
		if (write(STDOUT_FILENO, ptr->val, ft_strlen((char *)ptr->val)) == -1)
			return (ft_error(-1, "minishell: env: write", 1));
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (ft_error(-1, "minishell: env: write", 1));
		ptr = ptr->next;
	}
	return (0);
}
