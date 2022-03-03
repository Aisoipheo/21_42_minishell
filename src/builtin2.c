/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 16:43:41 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/03 12:37:20 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"

static int	ft_export_print(t_llist *args, t_info *info)
{
	(void)info;
	(void)args;
	t_ll_elem	*ptr;

	ptr = info->envp_list->head;
	while (ptr)
	{
		if (write(STDOUT_FILENO, "declare -x ", 11) == -1)
			return (ft_error(-1, "minishell: export: write", 1));
		if (write(STDOUT_FILENO, ptr->key, ft_strlen((char *)ptr->key)) == -1)
			return (ft_error(-1, "minishell: export: write", 1));
		if (write(STDOUT_FILENO, "=\"", 2) == -1)
			return (ft_error(-1, "minishell: export: write", 1));
		if (write(STDOUT_FILENO, ptr->val, ft_strlen((char *)ptr->val)) == -1)
			return (ft_error(-1, "minishell: export: write", 1));
		if (write(STDOUT_FILENO, "\"\n", 2) == -1)
			return (ft_error(-1, "minishell: export: write", 1));
		ptr = ptr->next;
	}
	return (0);
}

int	ft_export(t_llist *args, t_info *info)
{
	t_ll_elem	*ptr;
	char		*a;
	char		*b;

	ptr = args->head;
	if (args->size == 1)
		return (ft_export_print(args, info));
	ptr = ptr->next;
	while (ptr)
	{
		if (ft_strslice((char *)ptr->val, "=", &a, &b))
			return (ft_error(-1, "minishell: export: parse", 1));
		if (llist_set(info->envp_list, a, b))
			return (ft_error(-1, "minishell: export: set", 1));
		ptr = ptr->next;
	}
	info->envp_f = 1;
	return (0);
}

int	ft_unset(t_llist *args, t_info *info)
{
	t_ll_elem	*arg;

	arg = args->head;
	if (arg->next)
	{
		arg = arg->next;
		while (arg)
		{
			llist_del(info->envp_list, arg->val);
			arg = arg->next;
		}
	}
	info->envp_f = 1;
	return (0);
}

int	ft_exit(t_llist *args, t_info *info)
{
	(void)info;
	(void)args;
	info->exit_f = 0;
	return (0);
}
