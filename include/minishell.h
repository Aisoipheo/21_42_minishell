/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 12:30:22 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/02/23 13:54:12 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "linked_list.h"
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>

# define CMD_APPEND 0b001
# define CMD_INSOURCE 0b010
# define CMD_SUBSHELL 0b100

// int	g_exit = 0;

// f1(char **ar, function_ptr *f) {
// 	ar[0] = "echo";
// 	f[0] = &ft_echo;
// 	ar[1] = "cd";
// 	f[1] = &ft_cd;
// 	f2()
// }

//(t_llist *)elems->key)

typedef struct s_info
{
	char			exit_f;
	char			envp_f;
	char			**envp;
	char			*reserved_words[7];
	builtin_ptr		*f_ptrs[7];
	t_llist			*envp_list;
}	t_info;

typedef int (*builtin_ptr)(t_llist *, t_info *);
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
/* ============ /Environqment ============ */

void	handler(int sig);
void	handler_term(t_info *info);

pid_t	executor(t_group *cmds, t_info *info);
pid_t	pipeline(t_group *cmds, t_info *info);
int		subshell_();

int		ft_strncmp(const char *str1, const char *str2, size_t n);
int		destroy(t_group *cmds, t_info *info);


/* ============ Built-ins ============ */
int			ft_echo(t_llist *args, t_info *info);
int			ft_cd(t_llist *args, t_info *info);
int			ft_pwd(t_llist *args, t_info *info);
int			ft_export(t_llist *args, t_info *info);
int			ft_unset(t_llist *args, t_info *info);
int			ft_env(t_llist *args, t_info *info);
int			ft_exit(t_llist *args, t_info *info);
/* ============ /Built-ins ============ */

#endif
