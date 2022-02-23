/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 20:36:53 by gmckinle          #+#    #+#             */
/*   Updated: 2022/02/23 18:00:08 by gmckinle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"
#include "parser.h"

pid_t	pipeline(t_group *cmds, t_info *info)
{
	(void)cmds;
	(void)info;
	printf("PIPELINE\n");
	return (0);
}
