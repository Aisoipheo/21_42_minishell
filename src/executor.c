/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmckinle <gmckinle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 19:59:22 by gmckinle          #+#    #+#             */
/*   Updated: 2022/02/19 21:19:02 by gmckinle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "utils.h"



pid_t	executor(t_group *cmds, t_info *info)
{

	if (PRS_PIPELINE && cmds->type)
		pipeline(cmds, info);
	else if (PRS_SUBSHELL && cmds->type)
		;
	else if (PRS_SIMPLE && cmds->type)
		;
	else
	{
		;
	}

}
