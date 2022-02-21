/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:59:22 by gmckinle          #+#    #+#             */
/*   Updated: 2022/02/21 20:20:42 by gmckinle         ###   ########.fr       */
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
			return i;
		}
		i++;
	}
	return (i);
}

//is pipe? execute pipe
//						потому что в функции пайплайна будут вызываться все последующие функции
//is sub? execute sub
//is builtin? execute builtin
//execute default

// expr | expr | expr
// expr может быть или сабшелл или билтин или обычка

//в данной функции вернуть значение меньше нуля если произошла ошибка, иначе
//значение будет восприниматься

//cmd1 | cmd2 << delim | cmd3
//cmd1 << delim
pid_t	executor(t_group *cmds, t_info *info)
{
	int builtin_index = check_if_builtins(cmds, info);
	if (builtin_index < 8) return ((*info->f_ptrs[builtin_index])(cmds->cmds->head->key, info));
	if (((t_cmd_info *)cmds->cmds->head->val)->flags & CMD_SUBSHELL)
		return ft_subshell();
	if (PRS_PIPELINE && cmds->type)
		return (pipeline(cmds, info));

	return (0);
}
