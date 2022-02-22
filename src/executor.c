/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:59:22 by gmckinle          #+#    #+#             */
/*   Updated: 2022/02/22 14:54:21 by rdrizzle         ###   ########.fr       */
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

int	check_if_builtins(t_group *cmds, t_info *info)
{
	t_ll_elem	*elems;
	int			i;

	i = 0;
	elems = cmds->cmds->head;
	while (i < 7)
	{
		if (ft_strcmp(((t_llist *)elems->key)->head->val, info->reserved_words[i]))
			return (i);
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
	int	builtin_index;

	builtin_index = check_if_builtins(cmds, info);
	if (builtin_index < 8)
		return ((*info->f_ptrs[builtin_index])(cmds->cmds->head->key, info)); // проверка на билтин ВСЕ ОК
	else if (PRS_PIPELINE & cmds->type) // проверка на пайп ВСЕ ОК
		return (pipeline(cmds, info));
	else if (((t_cmd_info *)cmds->cmds->head->val)->flags & CMD_SUBSHELL) // проверка на сабшелл ВСЕ ОК
		return (ft_subshell());
	return (0);
}
