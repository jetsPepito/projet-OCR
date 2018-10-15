#include "error.c"
#include "callback.h"
#include "document.h"

static void open_file(const gchar *, GtkTextView *);

void cb_new (GtkWidget *p_widget, gpointer user_data)
{
  if (docs.actif)
  {
    cb_close (p_widget, user_data);
  }
  /* Pour l'instant il faut allouer la memoire, par la suite on modifiera
     simplement le pointeur */
  docs.actif = g_malloc (sizeof (*docs.actif));
  docs.actif->chemin = NULL;
  /* Pour l'instant, on se contente de stocker le seul GtkTextView qui existe,
    par la suite, il faudra en creer un nouveau ! */
  docs.actif->p_text_view = GTK_TEXT_VIEW (user_data);
  /* Le document vient d'etre creer, il n'est donc pas modifie */
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

void cb_open(GtkWidget *p_widget, gpointer user_data)
{
  GtkWidget *p_dialog = NULL;

  p_dialog = gtk_file_chooser_dialog_new ("Ouvrir un fichier", NULL,
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

  if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
  {
    gchar *file_name = NULL;

    file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
    open_file (file_name, GTK_TEXT_VIEW (user_data));
    g_free (file_name), file_name = NULL;
  }
  gtk_widget_destroy(p_dialog);

  (void)p_widget;
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

void cb_save (GtkWidget *p_widget, gpointer user_data)
{
  if (docs.actif)
  {
    if (!docs.actif->sauve)
    {
      /* Le fichier n'a pas encore ete enregistre */
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
          docs.actif->chemin = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
        }
        gtk_widget_destroy (p_dialog);
      }
      /* Soit le fichier a deja ete enregistre, soit l'utilisateur vient de nous
         fournir son nouvel emplacement, on peut donc l'enregistrer */
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
          contents = gtk_text_buffer_get_text (p_text_buffer, &start, &end, FALSE);
          locale = g_locale_from_utf8 (contents, -1, NULL, NULL, NULL);
          g_free (contents), contents = NULL;
          fprintf (fichier, "%s", locale);
          g_free (locale), locale = NULL;
          fclose (fichier), fichier = NULL;
          docs.actif->sauve = TRUE;
        }
        else
        {
          print_warning ("Impossible de sauvegarder le fichier %s", docs.actif->chemin);
        }
      }
    }
  }
  else
  {
    print_warning ("Aucun document ouvert");
  }

  /* Parametres inutilises */
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
  /* Avant de fermer, il faut verifier qu'un document a bien ete ouvert */
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
      gtk_box_pack_start (GTK_BOX (GTK_DIALOG (p_dialog)->vbox), p_label, TRUE, TRUE, 0);
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
      g_free (docs.actif->chemin), docs.actif->chemin = NULL;
      docs.actif->p_text_view = NULL;
      g_free (docs.actif), docs.actif = NULL;
    }
  }
  else
  {
    print_warning ("Aucun document ouvert");
  }

  /* Parametres inutilises */
  (void)p_widget;
  (void)user_data;
}

static void open_file (const gchar *file_name, GtkTextView *p_text_view)
{
  g_return_if_fail (file_name && p_text_view);
  {
    gchar *contents = NULL;

    if (g_file_get_contents (file_name, &contents, NULL, NULL))
    {
      /* Copie de contents dans le GtkTextView */
      gchar *utf8 = NULL;
      GtkTextIter iter;
      GtkTextBuffer *p_text_buffer = NULL;

      cb_new (NULL, p_text_view);
      docs.actif->chemin = g_strdup (file_name);
      gtk_widget_set_sensitive (GTK_WIDGET (docs.actif->p_text_view), TRUE);
      p_text_buffer = gtk_text_view_get_buffer (p_text_view);
      gtk_text_buffer_get_iter_at_line (p_text_buffer, &iter, 0);
      utf8 = g_locale_to_utf8 (contents, -1, NULL, NULL, NULL);
      g_free (contents), contents = NULL;
      gtk_text_buffer_insert (p_text_buffer, &iter, utf8, -1);
      g_free (utf8), utf8 = NULL;
      /* Nous sommes obliges de remetre sauve a TRUE car l'insertion du contenu
         du fichier dans le GtkTextView a appele cb_modfie */
      docs.actif->sauve = TRUE;
    }
    else
    {
    	print_warning ("Impossible d'ouvrir le fichier %s\n", file_name);
    }
  }
}
