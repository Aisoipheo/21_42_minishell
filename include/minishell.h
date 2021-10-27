/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:30:22 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/10/13 16:36:03 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "avl_bst.h"

# define MINISH_FT_EXIT_SUCCESS 0
# define MINISH_FT_EXIT_FAILURE 1
# define MINISH_FT_EXIT_INVUSAGE -1

typedef struct s_info
{
	char		**envp;
	char		envp_f;
	int			status;
	t_avl_tree	*avl_envp;
}	t_info;

/* ============ Environment ============ */
int			ft_parse_envp(t_c_list *l, const char *envp[]);
char		**ft_compose_envp(t_c_list *env);
/* ============ /Environment ============ */

/* ============ Built-ins ============ */
// int			ft_builtin_env(t_var_list *env);
/* ============ /Built-ins ============ */

#endif
