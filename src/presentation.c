/**
 * File              : presentation.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 06.09.2024
 * Last Modified Date: 07.09.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
/**
 * presentation.c
 * Copyright (c) 2024 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "zip_entry_read.h"
#include "presentation.h"

int read_presentation(zip_t *zip, ezxml_t *presentation, 
		ezxml_t *rels)
{
	size_t size;
	void *p_buf = NULL;
	if (zip_entry_read(zip, 
				"ppt/presentation.xml",
				&p_buf, &size))
	{
		fprintf(stderr, "No presentation\n");
		return -1;
	}

	// parse xml
	if (presentation)
		*presentation = ezxml_parse_str(p_buf, size);

	void *rels_buf = NULL;
	if (zip_entry_read(zip, 
				"ppt/_rels/presentation.xml.rels",
				&rels_buf, &size))
	{
		fprintf(stderr, "No rels for presentation\n");
		return -1;
	}

	// parse rels
	if (rels)
		*rels = ezxml_parse_str(rels_buf, size);

	return 0;
}
