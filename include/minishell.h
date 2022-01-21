/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:30:22 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/01/15 15:47:03 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "linked_list.h"

# define CMD_APPEND 0b001
# define CMD_INSOURCE 0b010
# define CMD_SUBSHELL 0b100

typedef struct s_info
{
	char		**envp;
	char		envp_f;
	t_llist		*envp_list;
}	t_info;

typedef struct s_cmd_info
{
	char	*in_file;
	char	*out_file;
	int		flags;
	int		_shlvl; //dont touch it
}	t_cmd_info;

typedef struct s_group
{
	int			type;
	t_llist		*cmds;
}	t_group;

t_group		*ft_group_new(int type);
void		ft_group_free(void *group);

/* ============ Environment ============ */
int			ft_parse_envp(t_llist *list, char *envp[]);
char		**ft_compose_envp(t_llist *list);
/* ============ /Environment ============ */

/* ============ Built-ins ============ */
// int			ft_builtin_env(t_var_list *env);
/* ============ /Built-ins ============ */

#endif
