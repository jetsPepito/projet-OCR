#include "error.c"
#include "callback.h"
#include "document.h"


void cb_new (GtkWidget *p_widget, gpointer user_data)
{
  if (docs.actif)
  {
    cb_close (p_widget, user_data);
  }
  /* We allocate the memory*/
  docs.actif = g_malloc (sizeof (*docs.actif));
  docs.actif->chemin = NULL;
  /* We stock the GtkTextView*/
  docs.actif->p_text_view = GTK_TEXT_VIEW (user_data);
  /* We don't modify the document because we have just created  */
  docs.actif->sauve = TRUE;
  gtk_widget_set_sensitive (GTK_WIDGET (docs.actif->p_text_view), TRUE);
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

void cb_close(GtkWidget *p_widget, gpointer user_data)
{
  /* We verify if the document has already been opened, before to close it*/
  if (docs.actif)
  {
    if (!docs.actif->sauve)
    {
      GtkTextIter start;
      GtkTextIter end;
      GtkTextBuffer *p_text_buffer = NULL;

      p_text_buffer = gtk_text_view_get_buffer (docs.actif->p_text_view);
      gtk_text_buffer_get_bounds (p_text_buffer, &start, &end);
      gtk_text_buffer_delete (p_text_buffer, &start, &end);
      gtk_widget_set_sensitive (GTK_WIDGET (docs.actif->p_text_view), FALSE);
      g_free (docs.actif->chemin), docs.actif->chemin = NULL;
      docs.actif->p_text_view = NULL;
      g_free (docs.actif), docs.actif = NULL;
    }
  }
  else
  {
    print_warning ("Aucun document ouvert");
  }


  (void)p_widget;
  (void)user_data;
}
