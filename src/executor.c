/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:59:22 by gmckinle          #+#    #+#             */
/*   Updated: 2022/02/21 18:21:55 by gmckinle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"

/*
◦ echo with option -n
◦ cd with only a relative or absolute path
◦ pwd with no options
◦ export with no options
◦ unset with no options
◦ env with no options or arguments
◦ exit with no options
*/

int	forking();
void ft_cd()
{
	printf("echo\n");
}

void ft_pwd()
{
	printf("echo\n");
}

void ft_export()
{
	printf("echo\n");
}

void ft_unset()
{
	printf("echo\n");
}

void ft_env()
{
	printf("echo\n");
}

void ft_exit()
{
	printf("echo\n");
}

void	ft_echo()
{
	printf("echo\n");
}

int	check_if_builtins(t_group *cmds, t_info *info)
{
	t_ll_elem *elems;
	elems = cmds->cmds->head;
	printf("!!![%s]!!!\n", ((t_llist *)elems->key)->head->val);
	int i = 0;
	int N = 7;
	while (i < N)
	{
		if (ft_strcmp(((t_llist *)elems->key)->head->val, info->reserved_words[i]))
		{
			(*info->f_ptrs[i])((t_llist *)elems->key, info);
			return (1);
		}
		i++;
	}
	return (0);
}

pid_t	executor(t_group *cmds, t_info *info)
{
	(void)info;
	check_if_builtins(cmds);
	if (PRS_PIPELINE && cmds->type)
		return (pipeline(cmds, info));

	return (0);
}
