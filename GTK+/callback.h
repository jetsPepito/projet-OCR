#ifndef CALLBACK_H
#define CALLBACK_H

#include <gtk/gtk.h>
#include <SDL/SDL_image.h>

#include "../ImageTreatment/image_treatment.h"

void on_open_image (GtkButton* , gpointer);
void cb_new (GtkWidget *, gpointer);
void cb_convert(GtkButton *, gpointer);
void cb_close (GtkWidget *, gpointer);
void cb_quit (GtkWidget *, gpointer);
void cb_modifie (GtkWidget *, gpointer);
void cb_about(GtkWidget *, gpointer);
void cb_save(GtkWidget *, gpointer);
void cb_saveas(GtkWidget *, gpointer);

#endif
