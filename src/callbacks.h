#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>
typedef struct {
    GtkProgressBar *progress_bar;
    int progress;
    gdouble capacite;
    gdouble disponible;
} ProgressData;

typedef struct {
    char nompark[100];
    int idpark;
    char localisationpark[100];
    int capacitepark;
    int nbrdispopark;
    int prixpark;
    char etatpark[50];
    char typepark[50];
    int id_agents;
} Parking;
typedef struct {
    int idagent;
    int idparking;
    int cin;
    char nom[100];
    char prenom[100];
    char adresse[200];
    char role[50];
    int jour;
    int mois;
    int annee;
    char sexe[10];
    int tel;
    char exp[100];
    char statut[50];
    char email[100];
    char image[100];
    char experience[200];
} Agent;

// Function declarations


void on_btn_aff_clicked(GtkButton *button, gpointer user_data);
void on_btn_ajouterpark_clicked(GtkButton *button, gpointer user_data);
void on_btn_supp_clicked(GtkWidget *widget, gpointer data);
void remove_parking_from_file(const gchar *nompark, int idpark);
void on_btn_search_park_clicked(GtkButton *button, gpointer user_data);
void on_btn_modif_clicked(GtkButton *button, gpointer user_data);
void on_btn_save_modif_clicked(GtkButton *button, gpointer user_data);
void load_agents(const char *filename, GHashTable *parking_agents_map);



void
on_button_assign_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_refreshagent_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void setup_treeview_columns(GtkTreeView *treeview);
void populate_treeview(GtkTreeView *treeview, const char *filename);

void setup_treeview_columns_for_treeview2(GtkTreeView *treeview);
void populate_treeview2(GtkTreeView *treeview, const char *filename);
void load_parkings_and_agents_for_treeview2(const char *parking_filename, const char *agents_filename, GtkTreeView *treeview);
void populate_treeview3(GtkTreeView *treeview, const char *filename);
void setup_treeview3_columns(GtkTreeView *treeview);
void populate_shift_txt_from_parking_and_agents();
void
on_btn_aff_agents_clicked              (GtkButton       *button,
                                        gpointer         user_data);



void
on_btn_sort_by_id_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_btn_sort_by_nom_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_btn_sort_by_capacite_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_btn_sort_by_prix_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data);


void
on_btn_visual_clicked                  (GtkButton       *button,
                                        gpointer         user_data);



gboolean update_progress(gpointer data);
#endif /* CALLBACKS_H */
