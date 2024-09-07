#ifndef PRESENTATION_H
#define PRESENTATION_H
#include "ezxml.h"
#include <zip.h>
int read_presentation(zip_t *zip, ezxml_t *presentation, ezxml_t *rels);
#endif /* ifndef PRESENTATION_H */
