/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:22:42 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/12/06 12:06:51 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# define PRS_SIMPLE		0b00100000
# define PRS_PIPELINE	0b10000000
# define PRS_SUBSHELL	0b01000000

# include "linked_list.h"
# include "minishell.h"

int	prs_parse(t_llist *tokens, t_info *info);
t_llist	*_prs_expand(t_llist *group, t_info *info);
int	_prs_logexec(t_llist *groups, t_info *info);

#endif
