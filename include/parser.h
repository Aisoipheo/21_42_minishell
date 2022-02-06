/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:22:42 by rdrizzle          #+#    #+#             */
/*   Updated: 2022/01/23 15:28:17 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# define PRS_SIMPLE		0b0010000
# define PRS_PIPELINE	0b1000000
# define PRS_SUBSHELL	0b0100000

# include "linked_list.h"
# include "minishell.h"

typedef struct s_chunk_info
{
	unsigned int	s;
	unsigned int	e;
}	t_chunk_info;

int			prs_parse(t_llist *tokens, t_info *info);
t_llist		*_prs_expand(t_llist *group, t_info *info);
int			_prs_logexec(t_llist *groups, t_info *info);

#endif
