/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:22:42 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/03/16 19:31:24 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# define PRS_SIMPLE		0b0010000
# define PRS_PIPELINE	0b1000000
# define PRS_SUBSHELL	0b0100000

# include "linked_list.h"
# include "minishell.h"
# include <sys/types.h>
# include <dirent.h>

typedef struct s_chunk_info
{
	unsigned int	s;
	unsigned int	e;
}	t_chunk_info;

typedef struct s_fpack1
{
	t_chunk_info	*ci;
	unsigned int	*i;
	unsigned int	*size;
}	t_fpack1;

int				prs_parse(t_llist *tokens, t_info *info);
t_llist			*_prs_expand(t_llist *group, t_info *info);
int				_prs_logexec(t_llist *groups, t_info *info);
int				_prs_extract_var(const char *s, t_info *info,
					char **envpvar_ptr, unsigned int *j);
t_chunk_info	*_prs_chunk_info_new(unsigned int s, unsigned int e);
int				_prs_is_delim_token(void *tok_key);
int				_prs_handle_group0(t_llist *groups, t_llist *group,
					t_ll_elem *ptr, long type);
int				_prs_handle_group1(t_llist *group);
int				_prs_dstr(t_llist *groups);
int				_prs_field_exp_collect_chunks(char *s, t_info *info,
					t_llist *chunks, unsigned int *size);
int				_prs_update_g_exit_str(t_info *info);
int				_prs_alloc_ci(t_chunk_info **ci,
					unsigned int i, unsigned int j);
int				_prs_field_expansion_prep(t_llist *str, t_info *info,
					t_llist *chunks, t_expi *ei);
int				_prs_fecc_return(t_chunk_info *ci);
t_llist			*_prs_asterisk_expansion(const char *word);
int				_prs_expandable(void *key);
t_llist			*_prs_asterisk_expansion_pwd(const char *word);
int				_prs_field_expansion_free(t_llist *chunks, char **word);
int				_prs_handle_subsh_token(t_ll_elem *ptr, t_llist *expanded);
int				_prs_field_expansion_copy(t_llist *chunks, char *word);
void			_prs_pattern_match_while(const char *pattern,
					const char *str, char **dp);
int				_prs_aep_bool(struct dirent *dirf, const char *word);
int				_prs_asterisk_pattern_matches(const char *pattern,
					const char *str);
void			_prs_token_handle_loop1(t_llist **str, t_ll_elem **ptr);
void			_prs_token_handle_loop2(t_llist *expanded,
					char *word, t_llist *words);
t_ll_elem		*_prs_next_token(t_ll_elem *curr);
int				_prs_handle_dstr(t_group *group, t_llist *expanded);
int				_prs_handle_heredoc(t_group *cmds);
void			_prs_waitall(int pid, int *sig);
int				_prs_prepare_group(t_llist *expanded, t_group *cmds);
int				_prs_group_pipe(t_llist *expanded, t_llist *cmds);
int				_prs_group_cmd(t_ll_elem *h, t_llist *cmds);
int				_prs_logexec_bonus(t_ll_elem *ptr, int *expect);
void			_prs_alloc_cmd_info(t_cmd_info **info);
int				_prs_group_cmd_ret1(t_cmd_info *info, t_llist *args);
int				_prs_group_cmd_ret2(t_cmd_info *info, t_llist *args);
int				_prs_group_cmd_ret3(t_cmd_info *info, t_llist *args);

#endif
