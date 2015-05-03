#include "Efl_Egueb.h"
#include "Egueb_Svg.h"
#include "Ecore.h"

#define WINDOW_SIZE 512

const char *android_svg = " \
	<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 96 105\"> \
	  <g fill=\"#97C024\" stroke=\"#97C024\" stroke-linejoin=\"round\" stroke-linecap=\"round\"> \
		<path d=\"M14,40v24M81,40v24M38,68v24M57,68v24M28,42v31h39v-31z\" stroke-width=\"12\"/> \
		<path d=\"M32,5l5,10M64,5l-6,10 \" stroke-width=\"2\"/> \
	  </g> \
	  <path d=\"M22,35h51v10h-51zM22,33c0-31,51-31,51,0\" fill=\"#97C024\"/> \
	  <g fill=\"#FFF\"> \
		<circle cx=\"36\" cy=\"22\" r=\"2\"/> \
		<circle cx=\"59\" cy=\"22\" r=\"2\"/> \
	  </g> \
	</svg>";

int main (int argc, char *argv[]) {
	Efl_Egueb_Window *w;
	Egueb_Dom_Node *doc, *svg, *ext = NULL;
	Enesim_Stream *stream;

	if (!efl_egueb_init ())
		return -1;

	doc = egueb_svg_document_new ();

	stream = enesim_stream_buffer_new ((void *)android_svg, strlen (android_svg), NULL);
	if (!stream)
		return -1;
	if (!egueb_dom_parser_parse (stream, &ext))
		return -1;
	svg = egueb_dom_document_node_adopt (doc, egueb_dom_node_child_first_get (ext), NULL);
	egueb_dom_node_child_append (doc, svg, NULL);

	w = efl_egueb_window_auto_new (doc, 0, 0, WINDOW_SIZE, WINDOW_SIZE);
	if (!w)
		return -1;

	ecore_main_loop_begin ();

	efl_egueb_window_free (w);
	efl_egueb_shutdown ();

	return 0;
}