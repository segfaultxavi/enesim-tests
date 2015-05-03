#include "Efl_Egueb.h"
#include "Egueb_Svg.h"
#include "Ecore.h"

#define WINDOW_SIZE 512
#define NUM_BALLS 50

#define RAND(x) ((x) * rand () / (RAND_MAX + 1))

typedef struct _State {
	int posx, posy, dirx, diry, radius;
	Egueb_Dom_Node *cir;
} State;

Eina_Bool timer (void *data) {
	State *state = (State *)data;
	Egueb_Svg_Coord coord;
	int ball;

	for (ball = 0; ball < NUM_BALLS; ball++, state++) {
		state->posx += state->dirx;
		state->posy += state->diry;
		if (state->posx < state->radius || state->posx > WINDOW_SIZE - state->radius)
			state->dirx *= -1;
		if (state->posy < state->radius || state->posy > WINDOW_SIZE - state->radius)
			state->diry *= -1;

		egueb_svg_length_set (&coord, state->posx, EGUEB_SVG_LENGTH_UNIT_PX);
		egueb_svg_element_circle_cx_set (state->cir, &coord);
		egueb_svg_length_set (&coord, state->posy, EGUEB_SVG_LENGTH_UNIT_PX);
		egueb_svg_element_circle_cy_set (state->cir, &coord);
	}

	return ECORE_CALLBACK_RENEW;
}

static void on_enter (Egueb_Dom_Event *ev, void *data) {
	State *state = (State *)data;
	Egueb_Svg_Paint paint;
	Egueb_Svg_Length len;

	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
	len.unit = EGUEB_SVG_LENGTH_UNIT_PX;
	len.value = 3;
	egueb_svg_color_components_from(&paint.color, 255, 0, 0);
	egueb_svg_element_stroke_set(state->cir, &paint);
	egueb_svg_element_stroke_width_set(state->cir, &len);
}

static void on_exit (Egueb_Dom_Event *ev, void *data) {
	State *state = (State *)data;
	Egueb_Svg_Length len;

	len.unit = EGUEB_SVG_LENGTH_UNIT_PX;
	len.value = 0;
	egueb_svg_element_stroke_width_set(state->cir, &len);
}

int main (int argc, char *argv[]) {
	Efl_Egueb_Window *w;
	Egueb_Dom_Node *doc, *svg;
	Egueb_Svg_Length len;
	Egueb_Svg_Paint paint;
	State *states;
	int ball;

	if (!efl_egueb_init ())
		return -1;

	states = malloc (sizeof (State) * NUM_BALLS);

	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;

	doc = egueb_svg_document_new ();

	svg = egueb_svg_element_svg_new ();
	egueb_dom_document_node_adopt (doc, svg, NULL);
	egueb_dom_node_child_append (doc, svg, NULL);

	for (ball = 0; ball < NUM_BALLS; ball++) {
		Egueb_Dom_Node *cir;
		State *state = &states[ball];

		state->radius = 10 + RAND (30);
		state->posx = state->radius + RAND (WINDOW_SIZE - 2 * state->radius);
		state->posy = state->radius + RAND (WINDOW_SIZE - 2 * state->radius);
		state->dirx = RAND (7) - 3;
		state->diry = RAND (7) - 3;

		cir = egueb_svg_element_circle_new ();
		egueb_svg_length_set (&len, state->radius, EGUEB_SVG_LENGTH_UNIT_PX);
		egueb_svg_element_circle_r_set (cir, &len);
		egueb_svg_color_components_from (&paint.color, RAND (200), RAND (200), RAND (200));
		egueb_svg_element_fill_set (cir, &paint);
		egueb_dom_node_event_listener_add(cir, EGUEB_DOM_EVENT_MOUSE_OVER, on_enter, EINA_FALSE, state);
		egueb_dom_node_event_listener_add(cir, EGUEB_DOM_EVENT_MOUSE_OUT, on_exit, EINA_FALSE, state);
		egueb_dom_document_node_adopt(doc, cir, NULL);
		egueb_dom_node_child_append (svg, cir, NULL);

		state->cir = cir;
	}

	w = efl_egueb_window_auto_new (doc, 0, 0, WINDOW_SIZE, WINDOW_SIZE);
	if (!w)
		return -1;

	ecore_timer_add (0.03, timer, states);
	ecore_main_loop_begin ();

	efl_egueb_window_free (w);
	efl_egueb_shutdown ();

	free (states);

	return 0;
}