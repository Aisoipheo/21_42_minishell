/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 16:43:41 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 16:51:20 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"

static int	ft_export_print(t_info *info)
{
	t_ll_elem	*ptr;

	ptr = info->envp_list->head;
	while (ptr)
	{
		if (write(STDOUT_FILENO, "declare -x ", 11) == -1)
			return (ft_error(-1, "minishell: export: write", 1, 0));
		if (write(STDOUT_FILENO, ptr->key, ft_strlen((char *)ptr->key)) == -1)
			return (ft_error(-1, "minishell: export: write", 1, 0));
		if (write(STDOUT_FILENO, "=\"", 2) == -1)
			return (ft_error(-1, "minishell: export: write", 1, 0));
		if (write(STDOUT_FILENO, ptr->val, ft_strlen((char *)ptr->val)) == -1)
			return (ft_error(-1, "minishell: export: write", 1, 0));
		if (write(STDOUT_FILENO, "\"\n", 2) == -1)
			return (ft_error(-1, "minishell: export: write", 1, 0));
		ptr = ptr->next;
	}
	g_exit = 0;
	return (0);
}

int	ft_export(t_llist *args, t_info *info)
{
	t_ll_elem	*ptr;
	char		*a;
	char		*b;

	ptr = args->head;
	if (args->size == 1)
		return (ft_export_print(info));
	ptr = ptr->next;
	g_exit = 0;
	while (ptr)
	{
		if (ft_strslice((char *)ptr->val, "=", &a, &b))
			return (ft_error(-1, "minishell: export: parse", 1, 0));
		if (!ft_is_valid_id(a))
		{
			g_exit = 1;
			write(STDERR_FILENO, "minishell: export: not valid\n", 30);
		}
		else if (llist_set(info->envp_list, a, b))
			return (ft_error(-1, "minishell: export: set", 1, 0));
		ptr = ptr->next;
	}
	info->envp_f = 1;
	return (0);
}

int	ft_unset(t_llist *args, t_info *info)
{
	t_ll_elem	*arg;

	g_exit = 0;
	arg = args->head;
	if (arg->next)
	{
		arg = arg->next;
		while (arg)
		{
			if (ft_is_valid_id(arg->val))
				llist_del(info->envp_list, arg->val);
			else
			{
				g_exit = 1;
				write(STDERR_FILENO, "minishell: unset: not valid\n", 29);
			}
			arg = arg->next;
		}
	}
	info->envp_f = 1;
	return (0);
}

int	ft_exit(t_llist *args, t_info *info)
{
	int	ex;

	(void)info;
	g_exit = 0;
	if (args->size > 2)
	{
		write(STDERR_FILENO, "minishell: exit: too many args\n", 32);
		g_exit = 1;
		return (0);
	}
	info->exit_f = 0;
	if (args->size == 2)
	{
		if (ft_atoi(((t_ll_elem *)args->head->next)->val, &ex))
		{
			write(STDERR_FILENO, "minishell: exit: only numeric arg\n", 35);
			g_exit = 255;
			return (0);
		}
		g_exit = ex % 256;
	}
	return (0);
}
