/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdrizzle <rdrizzle@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 11:25:45 by rdrizzle          #+#    #+#             */
/*   Updated: 2021/12/13 17:14:28 by rdrizzle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "utils.h"

#include <stdio.h>

static const char *_prs_extract_var(const char *s, t_info *info, int *j)
{
	int		i;
	char	*name;
	char	*ptr;

	i = 0;
	if (s[i] == '\0')
		return (NULL);
	while(ft_isalnum(s[i]) || s[i] == '_')
		++i;
	name = ft_substr(s, 0, i);
	if (NULL == name)
		return (NULL);
	ptr = llist_getval(info->envp_list, name);
	free(name);
	if (NULL != ptr)
		(*j) += i + 1;
	return (ptr);
}

static char	*_prs_field_expansion(t_llist *str, t_info *info)
{
	t_ll_elem	*ptr;
	char		*s;
	int			len;
	const char	*envpvar;
	int			i;
	int			j;

	ptr = str->head;
	len = 0;
	while(NULL != ptr)
	{
		if ((int)ptr->key == LX_FIELD)
			len += ft_strlen((const char *)ptr->val);
		else
		{
			j = 0;
			while(((char *)ptr->val)[j])
			{
				envpvar = NULL;
				while(((char *)ptr->val)[j] && ((char *)ptr->val)[j] != '$')
						len += (++j || 1);
				if (((char *)ptr->val)[j] == '$')
					envpvar = _prs_extract_var((char *)ptr->val + j + 1, info, &j);
				if (NULL != envpvar)
					len += ft_strlen(envpvar);
				else
					if (((char *)ptr->val)[j] == '$')
						len += (++j || 1);
			}
		}
		ptr = ptr->next;
	}
	s = (char *)malloc(sizeof(char) * (len + 1));
	printf("[parser2.c] PRS_FIELD_EXPANSION EXPECTED WORD SIZE %d\n", len);
	if (NULL == s)
		return (NULL);
	ptr = str->head;
	i = 0;
	s[len] = '\0';
	while(NULL != ptr)
	{
		j = 0;
		if ((int)ptr->key == LX_FIELD)
			while (((char *)ptr->val)[j])
				s[i++] = ((char *)ptr->val)[j++];
		else
		{
			while (((char *)ptr->val)[j])
			{
				envpvar = NULL;
				while(((char *)ptr->val)[j] && ((char *)ptr->val)[j] != '$')
					s[i++] = ((char *)ptr->val)[j++];
				if (((char *)ptr->val)[j] == '$')
					envpvar = _prs_extract_var((char *)ptr->val + j + 1, info, &j);
				if (NULL != envpvar)
					while(*envpvar)
						s[i++] = *envpvar++;
				else
					if (((char *)ptr->val)[j] == '$')
						s[i++] = ((char *)ptr->val)[j++];
			}
		}
		ptr = ptr->next;
	}
	return (s);
}

#include <sys/types.h>
#include <dirent.h>

int	_prs_asterisk_pattern_matches(const char *pattern, const char *str)
{
	char	**dp;
	int		i;
	int		j;
	int		strl;
	int		ptrl;

	ptrl = ft_strlen(pattern);
	strl = ft_strlen(str);
	dp = ft_malloc_char2dem(ptrl + 1, strl + 1);
	if (NULL == dp)
		return (0);
	i = 1;
	dp[0][0] = 1;
	while (i < ptrl + 1)
	{
		if (pattern[i - 1] == '*')
			dp[i][0] = dp[i - 1][0];
		++i;
	}
	i = 1;
	while (i < ptrl + 1)
	{
		j = 1;
		while (j < strl + 1)
		{
			if (pattern[i - 1] == '*')
				dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
			else
				dp[i][j] = pattern[i - 1] == str[j - 1];
			++j;
		}
		++i;
	}
	i = dp[ptrl][strl];
	ft_free_char2dem(dp, ptrl + 1);
	return (i);
}

t_llist *_prs_asterisk_expansion_pwd(const char *word, t_info *info)
{
	DIR				*dir;
	struct dirent	*dirf;
	t_llist			*words;

	dir = opendir(llist_getval(info->envp_list, "PWD"));
	words = llist_new(llist_int_kcmp, NULL, NULL);
	if (NULL == dir || NULL == words)
	{
		printf("[parser2.c] ASTERISK_EXPANSION_PWD FAILED\n");
		return (NULL);
	}
	dirf = readdir(dir);
	while (NULL != dirf)
	{
		printf("[parser2.c] ASTERISK_EXPANSION_PWD TRY <%s>\n", dirf->d_name);
		if (((word[0] == '.' && dirf->d_name[0] == '.') || (word[0] != '.' && dirf->d_name[0] != '.')) && _prs_asterisk_pattern_matches(word, dirf->d_name))
		{
			printf("[parser2.c] ASTERISK_EXPANSION_PWD WORD MATCHED\n");
			llist_push(words, (void *)LX_WORD, ft_strcpy(dirf->d_name));
		}
		dirf = readdir(dir);
	}
	closedir(dir);
	if (words->size == 0)
	{
		llist_free(words);
		words = NULL;
	}
	return (words);
}

// t_llist *_prs_asterisk_expansion_path(const char *word)
// {
// 	DIR				*dir;
// 	struct dirent	*dirf;
// 	t_llist			*words;
// 	char			*fullpath;

// 	dir = opendir(word);
// 	words = llist_new(llist_int_kcmp, NULL, NULL);
// 	if (NULL == dir || NULL == words)
// 	{
// 		//error
// 		return (NULL);
// 	}
// 	dirf = readdir(dir);
// 	while (NULL != dirf)
// 	{
// 		fullpath = ft_strjoin2();
// 		if (_prs_asterisk_pattern_matches(word, dirf->d_name))
// 			llist_push(words, LX_WORD, ft_strcpy(dirf->d_name));
// 		dirf = readdir(dir);
// 		free(fullpath);
// 	}
// 	if (words->size == 0)
// 	{
// 		llist_free(words);
// 		words = NULL;
// 	}
// 	closedir(dir);
// 	return (words);
// }

t_llist *_prs_asterisk_expansion(const char *word, t_info *info)
{
	if (!ft_strcontains(word, '*'))
		return (NULL);
	if (ft_strcontains(word, '/'))
		return (NULL);
	// 	return (_prs_asterisk_expansion_path(word));
	return (_prs_asterisk_expansion_pwd(word, info));
}

static int	_prs_expandable(void *key)
{
	int	k;

	k = (int)key;
	return (k == LX_WORD || k == LX_EXP_FIELD
			|| k == LX_FIELD);
}

static int	_prs_handle_token(t_ll_elem **ptr, t_llist *expanded, t_info *info)
{
	char		*word;
	t_llist		*words;
	t_ll_elem	*wptr;
	t_llist		*str;

	if (_prs_expandable((*ptr)->key))
	{
		printf("[parser2.c] PRS_HANDLE_TOKEN TRY EXPAND TOKEN <%s>\n", (char *)(*ptr)->val);
		str = llist_new(llist_int_kcmp, NULL, NULL);
		llist_push(str, (*ptr)->key, (*ptr)->val);
		while((*ptr)->next && _prs_expandable(((t_ll_elem *)(*ptr)->next)->key))
		{
			(*ptr) = (*ptr)->next;
			llist_push(str, (*ptr)->key, (*ptr)->val);
		}
		word = _prs_field_expansion(str, info);
		printf("[parser2.c] PRS_HANDLE_TOKEN EXPANDED WORD <%s>\n", word);
		words = _prs_asterisk_expansion(word, info);
		if (NULL == words)
			llist_push(expanded, (void *)LX_WORD, word);
		else
		{
			free(word);
			wptr = words->head;
			while(NULL != wptr)
			{
				printf("[parser2.c] PRS_HANDLE_TOKEN PUSH WORD <%s>\n", (char *)wptr->val);
				llist_push(expanded, (void *)LX_WORD, wptr->val);
				if (wptr->next)
					llist_push(expanded, (void *)LX_SEP, NULL);
				wptr = wptr->next;
			}
			llist_free(words);
		}
		llist_free(str);
	}
	else
		llist_push(expanded, (*ptr)->key, NULL);
	return (0);
}


t_llist	*_prs_expand(t_llist *group, t_info *info)
{

	t_llist		*expanded;
	t_ll_elem	*ptr;

	ptr = group->head;
	expanded = llist_new(llist_int_kcmp, NULL, free);
	while(NULL != expanded && NULL != ptr)
	{
		if (_prs_handle_token(&ptr, expanded, info))
		{
			llist_free(expanded);
			return (NULL);
		}
		ptr = ptr->next;
	}
	return (expanded);
}
