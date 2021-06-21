/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <nic.flenghi@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 13:52:44 by fnichola          #+#    #+#             */
/*   Updated: 2021/05/27 16:07:02 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	expand_line(char **line, size_t add_length)
{
	char	*new_line;
	size_t	i;

	new_line = (char *)malloc(sizeof(char) * (ft_strlen(*line) + add_length + 1));
		if (!new_line)
			{
				free(*line);
				return (0);
			}
		i = 0;
		while ((*line)[i])
		{
			new_line[i] = (*line)[i];
			i++;
		}
		new_line[i] = 0;
		free(*line);
		*line = new_line;
	return (1);
}

/**
 * Shift remaining content of buffer (after newline) to the beginning.
 * Fill the rest of the buffer with null (zeros).
 * e.g. "world.\nThis is" --> "This is\0\0\0\0\0\0\0"
 */

void	shift_buf(char *buf, size_t i)
{
	size_t	j;

	j = 0;
	if (i > 0)
	{
		while (i < BUFFER_SIZE)
			buf[j++] = buf[i++];
		while (j < BUFFER_SIZE)
			buf[j++] = 0;
	}
}

/**
 * Copy contents of buffer (up to newline) to line array.
 * Call expand_line to make space in array.
 *
 * Return: 0 error, 1 OK
 */

int	copy_buf_to_line(char *buf, char **line)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (buf[i] && buf[i] != '\n' && i < BUFFER_SIZE)
		i++;
	if (i > 0)
	{
		if(!expand_line(line, i))
			return (0);
		i = 0;
		j = 0;
		while ((*line)[j])
			j++;
		while (buf[i] && buf[i] != '\n' && i < BUFFER_SIZE)
			(*line)[j++] = buf[i++];
		(*line)[j] = 0;
	}
	shift_buf(buf, i);
	return (1);
}



/**
 * Reads a line read from a file descriptor, without the newline.
 *
 * RETURN:
 * 1 : A line has been read
 * 0 : EOF has been reached
 * -1 : An error happened
 */

int	get_next_line(int fd, char **line)
{
	static char	buf[BUFFER_SIZE];


	*line = (char *)malloc(sizeof(char) * 1);
	if (!*line)
		return (-1);
	(*line)[0] = 0;
	if (buf[0])
	{
		shift_buf(buf, 1);
		copy_buf_to_line(buf, line);
		if (buf[0] == '\n')
			return (1);
	}
	read(fd, buf, BUFFER_SIZE);
	while (buf[0])
	{
		copy_buf_to_line(buf, line);
		if (buf[0] == '\n')
			return (1);
		read(fd, buf, BUFFER_SIZE);
	}

	return (0);
}



int	main(void)
{
	char	*line;
	while (get_next_line(0, &line) > 0)
		printf("\"%s\"\n", line);
	printf("\"%s\"\n", line);
	return (0);
}
