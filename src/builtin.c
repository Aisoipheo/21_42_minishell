/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:54:25 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 20:02:09 by rdrizzle         ###   ########.fr       */
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
	arg = args->head->next;
	is_n = 0;
	g_exit = 0;
	if (args->size > 1)
	{
		is_n = ft_strcmp(arg->val, "-n") == 0;
		if (is_n)
			arg = arg->next;
		while (arg)
		{
			if (write(STDOUT_FILENO, arg->val, ft_strlen((char *)arg->val))
				== -1)
				return (ft_error(-1, "minishell: echo", 1, 0));
			if (arg->next && write(STDOUT_FILENO, " ", 1) == -1)
				return (ft_error(-1, "minishell: echo", 1, 0));
			arg = arg->next;
		}
	}
	if (!is_n)
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (ft_error(-1, "minishell: echo", 1, 0));
	return (0);
}

int	ft_cd(t_llist *args, t_info *info)
{
	t_ll_elem	*arg;
	char		*path;
	char		*pathcopy;
	char		*pwdcopy;

	g_exit = 0;
	arg = args->head;
	path = (char *)llist_getval(info->envp_list, "HOME");
	if (arg->next)
		path = (char *)((t_ll_elem *)(arg->next))->val;
	if (chdir(path) == -1)
		return (ft_error(-1, "minishell: cd: chdir", 1, 0));
	pathcopy = ft_strcpy(path);
	path = (char *)llist_getval(info->envp_list, "PWD");
	pwdcopy = ft_strcpy(path);
	if (pathcopy == NULL || pwdcopy == NULL)
		return (builtin_handler1(pwdcopy, pathcopy));
	if (path && (llist_set(info->envp_list, "OLDPWD", pwdcopy)
			|| llist_set(info->envp_list, "PWD", pathcopy)))
		return (ft_error(-1, "minishell: cd: malloc", 1, 0));
	info->envp_f = 1;
	return (0);
}

int	ft_pwd(t_llist *args, t_info *info)
{
	char			pwd[PATH_MAX];
	unsigned int	i;

	(void)info;
	(void)args;
	g_exit = 0;
	if (getcwd(pwd, PATH_MAX) == NULL)
		return (ft_error(-1, "minishell: pwd: getcwd", 1, 0));
	i = 0;
	while (i < PATH_MAX && pwd[i])
		++i;
	if (i < PATH_MAX)
		pwd[i++] = '\n';
	if (write(STDOUT_FILENO, pwd, i) == -1)
		return (ft_error(-1, "minishell: pwd: write", 1, 0));
	return (0);
}

int	ft_env(t_llist *args, t_info *info)
{
	t_ll_elem	*ptr;

	(void)args;
	g_exit = 0;
	ptr = info->envp_list->head;
	while (ptr)
	{
		if (write(STDOUT_FILENO, ptr->key, ft_strlen((char *)ptr->key)) == -1)
			return (ft_error(-1, "minishell: env: write", 1, 0));
		if (write(STDOUT_FILENO, "=", 1) == -1)
			return (ft_error(-1, "minishell: env: write", 1, 0));
		if (write(STDOUT_FILENO, ptr->val, ft_strlen((char *)ptr->val)) == -1)
			return (ft_error(-1, "minishell: env: write", 1, 0));
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (ft_error(-1, "minishell: env: write", 1, 0));
		ptr = ptr->next;
	}
	return (0);
}
