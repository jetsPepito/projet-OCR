#include "error.c"
#include "document.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include "../ImageTreatment/image_treatment.h"
#include <stdio.h>
#include <string.h>


void cb_save (GtkWidget *p_widget, gpointer user_data)
{
  if (docs.actif)
  {
    if (!docs.actif->sauve)
    {
      /* The file isn't just saved */
      if (!docs.actif->chemin)
      {
        GtkWidget *p_dialog = NULL;

        p_dialog = gtk_file_chooser_dialog_new ("Sauvegarder le fichier", NULL,
                                                GTK_FILE_CHOOSER_ACTION_SAVE,
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                           GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                                NULL);
        if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
        {
          docs.actif->chemin = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER
                                                                  (p_dialog));
        }
        gtk_widget_destroy (p_dialog);
      }
      /*Either the file has already been saved, or we can save it if we find a new place*/
      if (docs.actif->chemin)
      {
        FILE *fichier = NULL;

        fichier = fopen (docs.actif->chemin, "w");
        if (fichier)
        {
          gchar *contents = NULL;
          gchar *locale = NULL;
          GtkTextIter start;
          GtkTextIter end;
          GtkTextBuffer *p_text_buffer = NULL;

          p_text_buffer = gtk_text_view_get_buffer (docs.actif->p_text_view);
          gtk_text_buffer_get_bounds (p_text_buffer, &start, &end);
          contents = gtk_text_buffer_get_text(p_text_buffer,&start,&end,FALSE);
          locale = g_locale_from_utf8 (contents, -1, NULL, NULL, NULL);
          g_free (contents), contents = NULL;
          fprintf (fichier, "%s", locale);
          g_free (locale), locale = NULL;
          fclose (fichier), fichier = NULL;
          docs.actif->sauve = TRUE;
        }
        else
        {
          print_warning ("Impossible de sauvegarder le fichier %s",
                                                docs.actif->chemin);
        }
      }
    }
  }
  else
  {
    print_warning ("Aucun document ouvert");
  }


  (void)p_widget;
  (void)user_data;
}

void cb_saveas (GtkWidget *p_widget, gpointer user_data)
{
  if (docs.actif)
  {
    document_t tmp = *docs.actif;

    docs.actif->chemin = NULL;
    docs.actif->sauve = FALSE;
    cb_save (p_widget, user_data);
    if (!docs.actif->sauve)
    {
      (*docs.actif) = tmp;
    }
  }
  else
  {
    print_warning ("Aucun document ouvert");
  }
}

void cb_close(GtkWidget *p_widget, gpointer user_data)
{
  /* We verify if the document has already been opened, before to close it*/
  if (docs.actif)
  {
    if (!docs.actif->sauve)
    {
      GtkWidget *p_dialog = NULL;
      GtkWidget *p_label = NULL;

      p_dialog = gtk_dialog_new_with_buttons ("Sauvegarder",
                                              docs.p_main_window,
                                              GTK_DIALOG_MODAL,
                                              GTK_STOCK_YES, GTK_RESPONSE_YES,
                                              GTK_STOCK_NO, GTK_RESPONSE_NO,
                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
      p_label = gtk_label_new ("Voulez-vous sauvegarder les modifications ?");
      gtk_box_pack_start (GTK_BOX (GTK_DIALOG (p_dialog)->vbox),
                                        p_label, TRUE, TRUE, 0);
      gtk_widget_show_all (GTK_DIALOG (p_dialog)->vbox);
      switch (gtk_dialog_run (GTK_DIALOG (p_dialog)))
      {
        case GTK_RESPONSE_YES:
          cb_save (p_widget, user_data);
        break;
        case GTK_RESPONSE_NO:
        break;
        case GTK_RESPONSE_CANCEL:
          gtk_widget_destroy (p_dialog);
          return;
        break;
      }
      gtk_widget_destroy (p_dialog);
    }
    {
      GtkTextIter start;
      GtkTextIter end;
      GtkTextBuffer *p_text_buffer = NULL;

      p_text_buffer = gtk_text_view_get_buffer (docs.actif->p_text_view);
      gtk_text_buffer_get_bounds (p_text_buffer, &start, &end);
      gtk_text_buffer_delete (p_text_buffer, &start, &end);
      gtk_widget_set_sensitive (GTK_WIDGET (docs.actif->p_text_view), FALSE);


      //g_free (docs.actif->chemin), docs.actif->chemin = NULL;

      docs.actif->p_text_view = NULL;

      g_free(docs.actif), docs.actif = NULL;
    }
  }
  else
  {
    print_warning ("Aucun document ouvert");
  }


  (void)p_widget;
  (void)user_data;
}

void on_open_image (GtkButton* button, gpointer user_data)
{
	GtkWidget *image = GTK_WIDGET (user_data);
	GtkWidget *toplevel = gtk_widget_get_toplevel (image);
	GtkFileFilter *filter = gtk_file_filter_new ();
	GtkWidget *dialog = gtk_file_chooser_dialog_new ( "Open image",
	                                                 GTK_WINDOW (toplevel),
	                                                GTK_FILE_CHOOSER_ACTION_OPEN,
	                                           GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
	                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                 NULL);

	gtk_file_filter_add_pixbuf_formats (filter);
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),
	                             filter);

	switch (gtk_dialog_run (GTK_DIALOG (dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
		{
			gchar *filename =
				gtk_file_chooser_get_filename (GTK_FILE_CHOOSER
								(dialog)); 
  			docs.actif = g_malloc (sizeof (*docs.actif));
			docs.actif->chemin = filename;
			gtk_image_set_from_file (GTK_IMAGE (image), filename);
			break;
		}
		default:
			break;
	}
	gtk_widget_destroy (dialog);

	(void)button;

}


void cb_new (GtkWidget *p_widget, gpointer user_data)
{
  if (docs.actif)
  {
    cb_close (p_widget, user_data);
  }

 // We allocate the memory
  docs.actif = g_malloc (sizeof (*docs.actif));
  docs.actif->chemin = NULL;

  // We stock the GtkTextView
  docs.actif->p_text_view = GTK_TEXT_VIEW (user_data);

  // We don't modify the document because we have just created
  docs.actif->sauve = TRUE;
  gtk_widget_set_sensitive (GTK_WIDGET (docs.actif->p_text_view), TRUE);
}


void cb_convert(GtkButton *button, gpointer user_data)
{
	SDL_Surface *img = IMG_Load((gchar*)docs.actif->chemin);	
	gchar *contents = convert(img);

	gchar *utf8 = NULL;
	GtkTextIter iter;
        GtkTextBuffer *p_text_buffer = NULL;
	GtkTextView *p_text_view = GTK_TEXT_VIEW(user_data);

	docs.actif->p_text_view = p_text_view;

        cb_new (NULL,p_text_view);

        gtk_widget_set_sensitive (GTK_WIDGET (p_text_view), TRUE);
        p_text_buffer = gtk_text_view_get_buffer (p_text_view);
        gtk_text_buffer_get_iter_at_line (p_text_buffer, &iter, 0);

        utf8 = g_locale_to_utf8 (contents, -1, NULL, NULL, NULL);
        g_free (contents), contents = NULL;

        gtk_text_buffer_insert (p_text_buffer, &iter, utf8, -1);

        g_free (utf8), utf8 = NULL;
      /*We obliged to put sauve to TRUE because the insertion
       of file's contains in the GtkTextView called cb_modifie*/
        docs.actif->sauve = TRUE;

	(void)button;
}


void cb_quit(GtkWidget *p_widget, gpointer user_data)
{
  if (docs.actif)
  {
    cb_close (p_widget, user_data);
  }
  if (!docs.actif)
  {
    gtk_main_quit();
  }
}


void cb_modifie (GtkWidget *p_widget, gpointer user_data)
{
  if (docs.actif)
  {
    docs.actif->sauve = FALSE;
  }

  (void)p_widget;
  (void)user_data;
}


void cb_about (GtkWidget *p_widget, gpointer user_data)
 /*We create an apropos (a dialog) with credits..(BONUS)*/
{
  GtkWidget *p_about_dialog = NULL;

  p_about_dialog = gtk_about_dialog_new ();
  gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (p_about_dialog), "1.0");
  gtk_about_dialog_set_name (GTK_ABOUT_DIALOG (p_about_dialog), "OCR");

  {
    const gchar *authors[4] = {"Antoine Gonzalez\n","Jérémy Salfati\n",
                                "Louis Le Gatt\n", "Cédric Parpet\n"};

    gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (p_about_dialog), authors);
  }
  {
    gchar *contents = NULL;

    if (g_file_get_contents ("COPYING", &contents, NULL, NULL))
    {
      gchar *utf8 = NULL;

      utf8 = g_locale_to_utf8 (contents, -1, NULL, NULL, NULL);
      g_free (contents), contents = NULL;
      gtk_about_dialog_set_license (GTK_ABOUT_DIALOG (p_about_dialog), utf8);
      g_free (utf8), utf8 = NULL;
    }
  }
  {
    GdkPixbuf *p_logo = NULL;

    p_logo = gdk_pixbuf_new_from_file ("logo.png", NULL);
    gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG (p_about_dialog), p_logo);
  }
  gtk_dialog_run (GTK_DIALOG (p_about_dialog));
  gtk_widget_destroy(p_about_dialog);

  (void)p_widget;
  (void)user_data;
}

