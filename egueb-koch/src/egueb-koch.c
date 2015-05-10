#include "Efl_Egueb.h"
#include "Egueb_Svg.h"
#include "Ecore.h"

#define _GNU_SOURCE
#include <stdio.h>

const char *boilerplate_svg = " \
	<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 512 512\"> \
	  <g fill=\"none\" stroke=\"black\" stroke-linejoin=\"round\" > \
		<path d=\"M256,0%s\" stroke-width=\"1\"/> \
	  </g> \
	</svg>";

int vasprintf(char **strp, const char *fmt, va_list ap)
{
	va_list aq = NULL;
	int ret;

	va_copy(aq, ap);
	ret = vsnprintf(NULL, 0, fmt, aq);
	va_end(aq);
	if ((*strp = malloc(ret + 1)) == NULL)
		return (-1);
	ret = vsnprintf(*strp, ret + 1, fmt, ap);
	return (ret);
}

int asprintf(char **strp, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vasprintf(strp, fmt, ap);
	va_end(ap);
	return (ret);
}

void line (char **str, int *len, int x0, int y0, int x1, int y1) {
	if (abs(x1 - x0) > 1 || abs (y1 - y0) > 1) {
		int x2 = x0 + (x1 - x0) * 1 / 3;
		int y2 = y0 + (y1 - y0) * 1 / 3;
		int x3 = x0 + (x1 - x0) * 2 / 3;
		int y3 = y0 + (y1 - y0) * 2 / 3;
		int x4 = x0 + (x1 - x0) / 2 + (y1 - y0) / 4;
		int y4 = y0 + (y1 - y0) / 2 - (x1 - x0) / 4;
		line (str, len, x0, y0, x2, y2);
		line (str, len, x2, y2, x4, y4);
		line (str, len, x4, y4, x3, y3);
		line (str, len, x3, y3, x1, y1);
	} else {
		*str = realloc (*str, *len + 8 + 1);
		sprintf (*str + *len, "L%03d,%03d", x1, y1);
		*len += 8;
	}
}

int main (int argc, char *argv[]) {
	Efl_Egueb_Window *w;
	Egueb_Dom_Node *doc, *svg, *ext = NULL;
	Enesim_Stream *stream;
	char *def;
	char *str;
	int len = 0;

	if (!efl_egueb_init ())
		return -1;

	doc = egueb_svg_document_new ();

	str = malloc (1);
	*str = '\0';
	line (&str, &len, 256,   0, 512, 400);
	line (&str, &len, 512, 400,   0, 400);
	line (&str, &len,   0, 400, 256,   0);

	asprintf (&def, boilerplate_svg, str);
	free (str);

	stream = enesim_stream_buffer_new ((void *)def, strlen (def), NULL);
	if (!stream)
		return -1;
	if (!egueb_dom_parser_parse (stream, &ext))
		return -1;
	free (def);
	svg = egueb_dom_document_node_adopt (doc, egueb_dom_node_child_first_get (ext), NULL);
	egueb_dom_node_child_append (doc, svg, NULL);

	w = efl_egueb_window_auto_new (doc, 0, 0, 512, 512);
	if (!w)
		return -1;

	ecore_main_loop_begin ();

	efl_egueb_window_free (w);
	efl_egueb_shutdown ();

	return 0;
}
