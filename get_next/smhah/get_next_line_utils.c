/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smhah <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 18:30:52 by smhah             #+#    #+#             */
/*   Updated: 2019/11/01 19:56:35 by smhah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strchr(const char *str, int c)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return ((char *)str + i);
		i++;
	}
	if (str[i] == c)
		return ((char *)str + i);
	return (NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	unsigned int	i;
	unsigned int	j;
	char			*s;

	if (s1 && s2)
		i = ft_strlen((char *)s1);
	else
		return (NULL);
	j = ft_strlen((char *)s2);
	if (!(s = (char *)malloc(sizeof(char) * (i + j + 1))))
		return (0);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		s[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
		s[i++] = s2[j++];
	s[i] = '\0';
	if (*s2)
		free(s2);
	return (s);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*buf;
	size_t		i;
	size_t		j;

	i = 0;
	if (s)
	{
		if (!(buf = (char *)malloc(sizeof(char) * (len + 1))))
			return (NULL);
	}
	else
		return (NULL);
	j = ft_strlen(s);
	while ((i < len) && (start + i < j))
	{
		buf[i] = s[start + i];
		i++;
	}
	buf[i] = '\0';
	return (buf);
}
