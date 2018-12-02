#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "callback.h"
#include "document.h"
#include "../ImageTreatment/image_treatment.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>


docs_t docs = {NULL,NULL,NULL};

int main (int argc, char **argv)
{
  GtkWidget *p_window = NULL;
  GtkWidget *p_main_box = NULL;
  GtkWidget *p_box2 = NULL;
  GtkWidget *p_button_box = NULL;

  /* Initialization of GTK+ */
  gtk_init (&argc, &argv);

  /* Creation of the main window of our interface */
  p_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_maximize (GTK_WINDOW (p_window));
  gtk_window_set_title (GTK_WINDOW (p_window), "OCR");
  g_signal_connect (G_OBJECT (p_window), "destroy", G_CALLBACK (cb_quit),NULL);
  docs.p_main_window = GTK_WINDOW(p_window);

  /* Creation of main container */
  p_main_box = gtk_hbox_new (FALSE,0);
  gtk_container_add (GTK_CONTAINER (p_window), p_main_box);
  cb_about(p_main_box,p_window);

  GtkWidget *p_Image;
  p_Image = gtk_image_new();

  /* Creation of the container of pictures*/
  p_box2 = gtk_hbox_new(TRUE,0);
  gtk_box_pack_start(GTK_BOX(p_main_box),p_box2,FALSE, FALSE,0);
  gtk_box_pack_start(GTK_BOX(p_box2),p_Image,TRUE,TRUE,0);


  /* Creation of the text area */
  GtkWidget *p_text_view = NULL; /*Display the contains of GtkTextBuffer*/
  GtkTextBuffer *p_text_buffer = NULL; /*Stock data*/
  GtkWidget *p_scrolled_window = NULL;

  p_text_view = gtk_text_view_new ();
  p_scrolled_window = gtk_scrolled_window_new(NULL,NULL);

  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (p_scrolled_window),
	 														GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_box_pack_start (GTK_BOX (p_main_box), p_scrolled_window, TRUE, TRUE, 0);

  p_text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (p_text_view));
  g_signal_connect(G_OBJECT(p_text_buffer), "changed", G_CALLBACK(cb_modifie),												NULL);
  gtk_widget_set_sensitive (p_text_view, FALSE);
  gtk_container_add (GTK_CONTAINER (p_scrolled_window), p_text_view);


  /* Creation of container for buttons */
  p_button_box = gtk_vbox_new(TRUE,0);
  gtk_box_pack_start (GTK_BOX (p_main_box), p_button_box, FALSE, FALSE, 0);

  /* Creation of button "Open" */
  {
    GtkWidget *p_button = NULL;

    p_button = gtk_button_new_from_stock (GTK_STOCK_OPEN);
    g_signal_connect(G_OBJECT(p_button),"clicked",G_CALLBACK(on_open_image),
		 																															p_Image);
    gtk_box_pack_start (GTK_BOX (p_button_box), p_button, FALSE, TRUE, 0);
  }

  /*Creation of button "Convert"*/
  {
    GtkWidget *p_button = NULL;

    p_button = gtk_button_new_with_label("Convert");
    g_signal_connect(G_OBJECT(p_button), "clicked", G_CALLBACK(cb_convert),
		    					p_text_view);
    gtk_box_pack_start(GTK_BOX(p_button_box),p_button,FALSE,TRUE,0);
  }

  /* Creation of button "Save text" */
  {
    GtkWidget *p_button = NULL;

    p_button = gtk_button_new_from_stock (GTK_STOCK_SAVE);
    g_signal_connect (G_OBJECT (p_button), "clicked", G_CALLBACK (cb_save),																			NULL);
    gtk_box_pack_start (GTK_BOX (p_button_box), p_button, FALSE, TRUE, 0);
  }

    /* Creation of button "Save as" */
  {
    GtkWidget *p_button = NULL;

    p_button = gtk_button_new_from_stock (GTK_STOCK_SAVE_AS);
    g_signal_connect(G_OBJECT(p_button),"clicked",G_CALLBACK(cb_saveas),NULL);
    gtk_box_pack_start (GTK_BOX (p_button_box), p_button, FALSE, TRUE, 0);
  }

  /* Creation of button "close"*/
  {
    GtkWidget *p_button = NULL;

    p_button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    g_signal_connect (G_OBJECT (p_button), "clicked", G_CALLBACK (cb_close),
																															p_text_view);
    gtk_box_pack_start (GTK_BOX (p_button_box), p_button, FALSE, TRUE, 0);
  }

  /* Creation du bouton "Quitter" */
  {
    GtkWidget *p_button = NULL;

    p_button = gtk_button_new_from_stock (GTK_STOCK_QUIT);
    g_signal_connect(G_OBJECT (p_button),"clicked",G_CALLBACK(cb_quit),NULL);
    gtk_box_pack_start (GTK_BOX (p_button_box), p_button, FALSE, TRUE, 0);
  }


  /* Display of the main window*/
  gtk_widget_show_all (p_window);
  gtk_main ();
  return EXIT_SUCCESS;
}
