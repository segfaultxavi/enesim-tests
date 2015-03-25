#include "Efl_Egueb.h"
#include "Egueb_Svg.h"
#include "Ecore.h"

int main (int argc, char *argv[]) {
	Efl_Egueb_Window *w;
	Egueb_Dom_Node *doc;

	if (!efl_egueb_init ())
		return -1;

	doc = egueb_svg_document_new ();
	w = efl_egueb_window_auto_new (doc, 0, 0, 500, 500);
	if (!w)
		return -1;

	ecore_main_loop_begin ();

	efl_egueb_window_free (w);
	efl_egueb_shutdown ();

	return 0;
}