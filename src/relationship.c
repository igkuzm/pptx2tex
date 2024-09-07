/**
 * File              : relationship.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 06.09.2024
 * Last Modified Date: 07.09.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
/**
 * relationship.c
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

#include <string.h>
#include "ezxml.h"
const char * get_relationship_target(ezxml_t relationships, 
		const char *rId)
{
	ezxml_t r;
	for(r = relationships->child;
			r; 
			r = r->next)
	{
		const char *id = 
			ezxml_attr(r, "Id");
		if (id && strcmp(rId, id) == 0)
			return ezxml_attr(r, "Target");
	}
	return NULL;
}
