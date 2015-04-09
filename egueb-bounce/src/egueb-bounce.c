#include "Efl_Egueb.h"
#include "Egueb_Svg.h"
#include "Ecore.h"

#define WINDOW_SIZE 512
#define RADIUS 30

typedef struct _State {
	int posx, posy, dirx, diry;
	Egueb_Dom_Node *cir;
} State;

Eina_Bool timer (void *data) {
	State *state = (State *)data;
	Egueb_Svg_Coord coord;

	state->posx += state->dirx;
	state->posy += state->diry;
	if (state->posx < RADIUS || state->posx > WINDOW_SIZE - RADIUS)
		state->dirx *= -1;
	if (state->posy < RADIUS || state->posy > WINDOW_SIZE - RADIUS)
		state->diry *= -1;

	egueb_svg_length_set (&coord, state->posx, EGUEB_SVG_LENGTH_UNIT_PX);
	egueb_svg_element_circle_cx_set (state->cir, &coord);
	egueb_svg_length_set (&coord, state->posy, EGUEB_SVG_LENGTH_UNIT_PX);
	egueb_svg_element_circle_cy_set (state->cir, &coord);

	return ECORE_CALLBACK_RENEW;
}

int main (int argc, char *argv[]) {
	Efl_Egueb_Window *w;
	Egueb_Dom_Node *doc, *svg, *cir, *ext = NULL, *svg2;
	Egueb_Svg_Length len;
	Egueb_Svg_Paint paint;
	Enesim_Stream *stream;
	State state;

	if (!efl_egueb_init ())
		return -1;

	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;

	doc = egueb_svg_document_new ();

	svg = egueb_svg_element_svg_new ();
	egueb_dom_document_node_adopt (doc, svg, NULL);
	egueb_dom_node_child_append (doc, svg, NULL);

	cir = egueb_svg_element_circle_new ();
	egueb_svg_length_set (&len, RADIUS, EGUEB_SVG_LENGTH_UNIT_PX);
	egueb_svg_element_circle_r_set (cir, &len);
	egueb_svg_color_components_from (&paint.color, 255, 0, 0);
	egueb_svg_element_fill_set (cir, &paint);
	egueb_dom_document_node_adopt (doc, cir, NULL);
	egueb_dom_node_child_append (svg, cir, NULL);

	w = efl_egueb_window_auto_new (doc, 0, 0, WINDOW_SIZE, WINDOW_SIZE);
	if (!w)
		return -1;

	state.posx = 250;
	state.posy = 250;
	state.dirx = 8;
	state.diry = 6;
	state.cir = cir;

	ecore_timer_add (0.03, timer, &state);
	ecore_main_loop_begin ();

	efl_egueb_window_free (w);
	efl_egueb_shutdown ();

	return 0;
}