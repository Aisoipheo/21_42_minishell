/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:59:22 by gmckinle          #+#    #+#             */
/*   Updated: 2022/02/20 20:02:43 by gmckinle         ###   ########.fr       */
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

int	check_if_builtins(t_group *cmds)
{
	t_ll_elem *elems;
	elems = cmds->cmds->head;
	printf("!!![%s]!!!\n", ((t_llist *)elems->key)->head->val);
	int i = 0;
	int N = 7;
	while (i < N)
	{
		if (ft_strcmp(((t_llist *)elems->key)->head->val, reserved_words[i]))
		{
			f_ptrs[i](param1, param2);
			return (1);
		}
		i++; 
	}
	return (0);
}

typedef int (*function_ptr)(char **, t_info *);
char **reserved_words[7] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
function_ptr *f_ptrs[7] = {&ft_echo, &ft_cd, &ft_pwd, &ft_export, &ft_unset, &ft_env, &ft_exit};

// f1(char **ar, function_ptr *f) {
// 	ar[0] = "echo";
// 	f[0] = &ft_echo;
// 	ar[1] = "cd";
// 	f[1] = &ft_cd;
// 	ar[2] = "cd";
// 	f[2] = &ft_cd;
// 	ar[3] = "cd";
// 	f[3] = &ft_cd;
// 	ar[1] = "cd";
// 	f[1] = &ft_cd;
// 	f2()
// }


pid_t	executor(t_group *cmds, t_info *info)
{
	(void)info;
	check_if_builtins(cmds);
	if (PRS_PIPELINE && cmds->type)
		return (pipeline(cmds, info));

	return (0);
}
