#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define MAX_LINE_LENGTH 256  // Or any appropriate length


void setup_treeview_columns(GtkTreeView *treeview) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();

    column = gtk_tree_view_column_new_with_attributes("Nom Parking", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("ID Parking", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("Localisation", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("Capacité", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("Disponible", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("Prix", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("Etat", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 7, NULL);
    gtk_tree_view_append_column(treeview, column);

    // This column will now display the agent ID assigned to the parking
    column = gtk_tree_view_column_new_with_attributes("ID Agent", renderer, "text", 8, NULL);
    gtk_tree_view_append_column(treeview, column);
}



void populate_treeview(GtkTreeView *treeview, const char *filename) {
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *file = fopen(filename, "r");
    char line[256];

    // Create a GtkListStore with 9 columns (8 fields + 1 for agent ID)
    store = gtk_list_store_new(9, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, 
                               G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);

    gtk_tree_view_set_model(treeview, GTK_TREE_MODEL(store));

    while (fgets(line, sizeof(line), file)) {
        Parking p;
        
        // Parse the line to fill the parking structure, including the agent ID
        sscanf(line, "%99[^;];%d;%99[^;];%d;%d;%d;%99[^;];%99[^;];%d",
               p.nompark, &p.idpark, p.localisationpark, &p.capacitepark, 
               &p.nbrdispopark, &p.prixpark, p.etatpark, p.typepark, &p.id_agents);

        // Add data to the GtkListStore (including agent ID)
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, p.nompark, 1, p.idpark, 2, p.localisationpark, 
                           3, p.capacitepark, 4, p.nbrdispopark, 5, p.prixpark, 
                           6, p.etatpark, 7, p.typepark, 8, p.id_agents, -1);
    }

    fclose(file);
}



void on_btn_aff_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "treeview1");

    // Setup and populate TreeView
    setup_treeview_columns(GTK_TREE_VIEW(treeview));
    populate_treeview(GTK_TREE_VIEW(treeview), "parking.txt");
}
















void setup_treeview3_columns(GtkTreeView *treeview) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();

    // Column for ID Agent
    column = gtk_tree_view_column_new_with_attributes("ID Agent", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(treeview, column);

    // Column for Nom Agent
    column = gtk_tree_view_column_new_with_attributes("Nom Agent", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(treeview, column);
}
void populate_treeview3(GtkTreeView *treeview, const char *filename) {
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *file = fopen(filename, "r");
    char line[256];

    store = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_STRING); // 2 columns: ID Agent and Nom Agent
    gtk_tree_view_set_model(treeview, GTK_TREE_MODEL(store));

    if (!file) {
        perror("Error opening agents.txt");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        Agent a;
        char nom[100], prenom[100];

        // Parse the agent details
        sscanf(line, "%d %*d %*d %99s %99s %*s %*s %*d-%*d-%*d %*s %*d %*s %*s %*s %*s %*s %*s",
               &a.idagent, nom, prenom);

        // Safely combine nom and prenom into a.nom
        snprintf(a.nom, sizeof(a.nom), "%s %s", nom, prenom);

        // Add data to the GtkListStore
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, a.idagent, 1, a.nom, -1);
    }

    fclose(file);
}


void on_btn_aff_agents_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *treeview = lookup_widget(GTK_WIDGET(button), "treeview3");

    // Setup and populate TreeView3
    setup_treeview3_columns(GTK_TREE_VIEW(treeview));
    populate_treeview3(GTK_TREE_VIEW(treeview), "agents.txt");
}




















// Function to initialize a Parking instance with default values
void init_parking(Parking *p) {
    strcpy(p->nompark, "");
    p->idpark = 0;
    strcpy(p->localisationpark, "");
    p->capacitepark = 0;
    p->nbrdispopark = 0;
    p->prixpark = 0;
    strcpy(p->etatpark, "N/A");
    strcpy(p->typepark, "N/A");
    p->id_agents = 0;
}


int ajouter(Parking p, const char *fichier) {
    FILE *file = fopen(fichier, "a");
    if (file != NULL) {
        fprintf(file, "%s;%d;%s;%d;%d;%d;%s;%s;%d\n",
                p.nompark, p.idpark, p.localisationpark, p.capacitepark,
                p.nbrdispopark, p.prixpark, p.etatpark, p.typepark, p.id_agents);
        fclose(file);
        return 1;
    }
    return 0;
}


void on_btn_ajouterpark_clicked(GtkButton *button, gpointer user_data) {
    Parking p;
    GtkWidget *entry_nompark, *entry_idpark, *entry_localisationpark, *entry_capacitepark;
    GtkWidget *entry_nbrdispopark, *entry_prixpark, *combobox_etatpark, *combobox_typepark;
    GtkWidget *message, *id_uni_label;
    char fichier[] = "parking.txt";
    char msg[] = "Enregistrement avec succès";
    FILE *file;
    char line[256];
    int id_exists = 0;

    // Initialize the parking structure with default values
    init_parking(&p);

    // Get widgets
    entry_nompark = lookup_widget(GTK_WIDGET(button), "entry_nompark");
    entry_idpark = lookup_widget(GTK_WIDGET(button), "entry_idpark");
    entry_localisationpark = lookup_widget(GTK_WIDGET(button), "entry_localisationpark");
    entry_capacitepark = lookup_widget(GTK_WIDGET(button), "entry_capacitepark");
    entry_nbrdispopark = lookup_widget(GTK_WIDGET(button), "entry_nbrdispopark");
    entry_prixpark = lookup_widget(GTK_WIDGET(button), "entry_prixpark");
    combobox_etatpark = lookup_widget(GTK_WIDGET(button), "combobox_etatpark");
    combobox_typepark = lookup_widget(GTK_WIDGET(button), "combobox_typepark");
    message = lookup_widget(GTK_WIDGET(button), "labelmsg");
    id_uni_label = lookup_widget(GTK_WIDGET(button), "id_uni");

    // Fill parking structure
    strcpy(p.nompark, gtk_entry_get_text(GTK_ENTRY(entry_nompark)));
    p.idpark = atoi(gtk_entry_get_text(GTK_ENTRY(entry_idpark)));
    strcpy(p.localisationpark, gtk_entry_get_text(GTK_ENTRY(entry_localisationpark)));
    p.capacitepark = atoi(gtk_entry_get_text(GTK_ENTRY(entry_capacitepark)));
    p.nbrdispopark = atoi(gtk_entry_get_text(GTK_ENTRY(entry_nbrdispopark)));
    p.prixpark = atoi(gtk_entry_get_text(GTK_ENTRY(entry_prixpark)));

    if (combobox_etatpark != NULL) {
        gchar *etat = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_etatpark));
        if (etat != NULL) {
            strcpy(p.etatpark, etat);
            g_free(etat);
        }
    }

    if (combobox_typepark != NULL) {
        gchar *type = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox_typepark));
        if (type != NULL) {
            strcpy(p.typepark, type);
            g_free(type);
        }
    }

    // Check if the ID already exists in the file
    file = fopen(fichier, "r");
    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            Parking temp;
            sscanf(line, "%99[^;];%d;%99[^;];%d;%d;%d;%99[^;];%99[^\n]",
                   temp.nompark, &temp.idpark, temp.localisationpark, &temp.capacitepark,
                   &temp.nbrdispopark, &temp.prixpark, temp.etatpark, temp.typepark);

            if (temp.idpark == p.idpark) {
                id_exists = 1;
                break;
            }
        }
        fclose(file);
    }

    if (id_exists) {
        gtk_label_set_text(GTK_LABEL(id_uni_label), "This ID already exists");
    } else if (ajouter(p, fichier)) {
        gtk_label_set_text(GTK_LABEL(message), msg);
        gtk_label_set_text(GTK_LABEL(id_uni_label), ""); // Clear ID error message

        // Clear the input fields after successful addition
        gtk_entry_set_text(GTK_ENTRY(entry_nompark), "");
        gtk_entry_set_text(GTK_ENTRY(entry_idpark), "");
        gtk_entry_set_text(GTK_ENTRY(entry_localisationpark), "");
        gtk_entry_set_text(GTK_ENTRY(entry_capacitepark), "");
        gtk_entry_set_text(GTK_ENTRY(entry_nbrdispopark), "");
        gtk_entry_set_text(GTK_ENTRY(entry_prixpark), "");
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_etatpark), -1);
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_typepark), -1);
    } else {
        gtk_label_set_text(GTK_LABEL(message), "Erreur lors de l'enregistrement");
    }
}

void on_btn_supp_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *treeview = lookup_widget(widget, "treeview1");
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkListStore *store;
    gchar *nompark;
    int idpark;

    // Get the selected row
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        // Get the values from the selected row (assuming they are in columns 0 and 1)
        gtk_tree_model_get(model, &iter, 0, &nompark, 1, &idpark, -1);

        // Create a confirmation dialog
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_WARNING,
                                                   GTK_BUTTONS_YES_NO,
                                                   "Are you sure you want to delete the parking '%s' with ID %d?",
                                                   nompark, idpark);

        // Run the dialog and get the user's response
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (response == GTK_RESPONSE_YES) {
            // Remove the selected row from the tree view (GtkListStore)
            store = GTK_LIST_STORE(model);
            gtk_list_store_remove(store, &iter);

            // Update the parking.txt file to remove the entry
            remove_parking_from_file(nompark, idpark);
        }

        // Free allocated memory for strings retrieved from the model
        g_free(nompark);
    }
}

// Function to remove parking from the file
void remove_parking_from_file(const gchar *nompark, int idpark) {
    FILE *file = fopen("parking.txt", "r");
    FILE *temp_file = fopen("temp.txt", "w");
    char line[256];
    Parking p;

    if (file == NULL || temp_file == NULL) {
        return;
    }

    // Read the original file line by line
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%99[^;];%d;%99[^;];%d;%d;%d;%99[^;];%99[^\n]", p.nompark, &p.idpark, p.localisationpark, &p.capacitepark, &p.nbrdispopark, &p.prixpark, p.etatpark, p.typepark);

        // Skip the line that matches the parking to delete
        if (strcmp(nompark, p.nompark) != 0 || idpark != p.idpark) {
           fprintf(temp_file, "%s;%d;%s;%d;%d;%d;%s;%s;%d\n",
        p.nompark, p.idpark, p.localisationpark, p.capacitepark, 
        p.nbrdispopark, p.prixpark, p.etatpark, p.typepark, p.id_agents);

        }
    }

    fclose(file);
    fclose(temp_file);

    // Remove the original file and rename the temp file
    remove("parking.txt");
    rename("temp.txt", "parking.txt");
}

void on_btn_search_park_clicked(GtkButton *button, gpointer user_data) {
    const char *search_term;
    GtkWidget *entry_search_park, *dialog, *label;
    FILE *file = fopen("parking.txt", "r");
    char line[256];
    Parking p;
    int found = 0; // Flag to check if we find the parking ID

    // Get the ID entered in the entry widget
    entry_search_park = lookup_widget(GTK_WIDGET(button), "entry_search_park");
    if (entry_search_park == NULL) {
        g_warning("Widget 'entry_search_park' not found.");
        return;
    }
    
    search_term = gtk_entry_get_text(GTK_ENTRY(entry_search_park));
    
    // Open the parking file for reading
    if (file == NULL) {
        g_warning("Failed to open 'parking.txt'.");
        return;
    }

    // Search for the parking information based on the entered ID
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%99[^;];%d;%99[^;];%d;%d;%d;%99[^;];%99[^;];%d",
       p.nompark, &p.idpark, p.localisationpark, &p.capacitepark, 
       &p.nbrdispopark, &p.prixpark, p.etatpark, p.typepark, &p.id_agents);


        if (p.idpark == atoi(search_term)) {
            // Create the message dialog to show the parking info
            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
                                            "Parking Found:\n\n"
                                            "Name: %s\nID: %d\nLocation: %s\nCapacity: %d\nAvailable: %d\nPrice: %d\nState: %s\nType: %s", 
                                            p.nompark, p.idpark, p.localisationpark, p.capacitepark, 
                                            p.nbrdispopark, p.prixpark, p.etatpark, p.typepark);
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            found = 1;
            break;
        }
    }

    if (!found) {
        // Create the message dialog if parking is not found
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, 
                                        "Parking with ID %s not found.", search_term);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

    fclose(file);
}

void on_btn_modif_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_search_park = lookup_widget(GTK_WIDGET(button), "entry_search_park");
    GtkWidget *entry_nompark = lookup_widget(GTK_WIDGET(button), "entry_nompark");
    GtkWidget *entry_idpark = lookup_widget(GTK_WIDGET(button), "entry_idpark");
    GtkWidget *entry_localisationpark = lookup_widget(GTK_WIDGET(button), "entry_localisationpark");
    GtkWidget *entry_capacitepark = lookup_widget(GTK_WIDGET(button), "entry_capacitepark");
    GtkWidget *entry_nbrdispopark = lookup_widget(GTK_WIDGET(button), "entry_nbrdispopark");
    GtkWidget *entry_prixpark = lookup_widget(GTK_WIDGET(button), "entry_prixpark");
    GtkWidget *notebook = lookup_widget(GTK_WIDGET(button), "notebook1");
    
    GtkWidget *combobox_etatpark = lookup_widget(GTK_WIDGET(button), "combobox_etatpark");
    GtkWidget *combobox_typepark = lookup_widget(GTK_WIDGET(button), "combobox_typepark");
    

    const char *id_search = gtk_entry_get_text(GTK_ENTRY(entry_search_park));
    FILE *file = fopen("parking.txt", "r");
    if (!file) return;

    char line[512];
    int found = 0;

    // Type options
    const char *type_options[] = {
        "Residential", "Commercial", "Covered", "Private", "Public", "Open-air", "Disabled-accessible"
    };
    
    // Etat options
    const char *etat_options[] = {
        "Open", "Closed", "Under Maintenance", "Full"
    };

    // Get the model of the combo boxes
    GtkListStore *store_etat = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(combobox_etatpark)));
    GtkListStore *store_type = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(combobox_typepark)));

    // Clear the model (removes all items)
    gtk_list_store_clear(store_etat);
    gtk_list_store_clear(store_type);

    for (int i = 0; i < 4; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store_etat, &iter);
        gtk_list_store_set(store_etat, &iter, 0, etat_options[i], -1);
    }

    // Populate ComboBoxes with the options
    for (int i = 0; i < 7; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store_type, &iter);
        gtk_list_store_set(store_type, &iter, 0, type_options[i], -1);
    }

    // Read file and find parking
    while (fgets(line, sizeof(line), file)) {
        char nom[100], id[10], localisation[100], capacite[10], dispo[10], prix[10], type[50], etat[50];
        sscanf(line, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]",
               nom, id, localisation, capacite, dispo, prix, type, etat);

        if (strcmp(id, id_search) == 0) {
            gtk_entry_set_text(GTK_ENTRY(entry_nompark), nom);
            gtk_entry_set_text(GTK_ENTRY(entry_idpark), id);
            gtk_entry_set_text(GTK_ENTRY(entry_localisationpark), localisation);
            gtk_entry_set_text(GTK_ENTRY(entry_capacitepark), capacite);
            gtk_entry_set_text(GTK_ENTRY(entry_nbrdispopark), dispo);
            gtk_entry_set_text(GTK_ENTRY(entry_prixpark), prix);

            // Set the ComboBox selection for 'etat'
            for (int i = 0; i < 4; i++) {
                if (strcmp(etat, etat_options[i]) == 0) {
                    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_etatpark), i);
                    break;
                }
            }
            // Set the ComboBox selection for 'type'
            for (int i = 0; i < 7; i++) {
                if (strcmp(type, type_options[i]) == 0) {
                    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_typepark), i);
                    break;
                }
            }

            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        // Optionally, display a message to indicate no matching parking was found.
    }

    // Navigate to the first tab of notebook1
    if (notebook != NULL) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
    }
}




void on_btn_save_modif_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_search_park = lookup_widget(GTK_WIDGET(button), "entry_search_park");
    GtkWidget *entry_nompark = lookup_widget(GTK_WIDGET(button), "entry_nompark");
    GtkWidget *entry_idpark = lookup_widget(GTK_WIDGET(button), "entry_idpark");
    GtkWidget *entry_localisationpark = lookup_widget(GTK_WIDGET(button), "entry_localisationpark");
    GtkWidget *entry_capacitepark = lookup_widget(GTK_WIDGET(button), "entry_capacitepark");
    GtkWidget *entry_nbrdispopark = lookup_widget(GTK_WIDGET(button), "entry_nbrdispopark");
    GtkWidget *entry_prixpark = lookup_widget(GTK_WIDGET(button), "entry_prixpark");
    GtkWidget *combobox_typepark = lookup_widget(GTK_WIDGET(button), "combobox_typepark");
    GtkWidget *combobox_etatpark = lookup_widget(GTK_WIDGET(button), "combobox_etatpark");

    const char *id_search = gtk_entry_get_text(GTK_ENTRY(entry_search_park));
    const char *new_nom = gtk_entry_get_text(GTK_ENTRY(entry_nompark));
    const char *new_id = gtk_entry_get_text(GTK_ENTRY(entry_idpark));
    const char *new_localisation = gtk_entry_get_text(GTK_ENTRY(entry_localisationpark));
    const char *new_capacite = gtk_entry_get_text(GTK_ENTRY(entry_capacitepark));
    const char *new_dispo = gtk_entry_get_text(GTK_ENTRY(entry_nbrdispopark));
    const char *new_prix = gtk_entry_get_text(GTK_ENTRY(entry_prixpark));

    GtkComboBox *combo_box_type = GTK_COMBO_BOX(combobox_typepark);
    GtkComboBox *combo_box_etat = GTK_COMBO_BOX(combobox_etatpark);

    // Get the active index of the combo boxes
    gint active_type = gtk_combo_box_get_active(combo_box_type);
    gint active_etat = gtk_combo_box_get_active(combo_box_etat);

    // If the combo box has valid items, retrieve the active text
    const char *new_type = (active_type != -1) ? gtk_combo_box_get_active_text(combo_box_type) : NULL;
    const char *new_etat = (active_etat != -1) ? gtk_combo_box_get_active_text(combo_box_etat) : NULL;

    FILE *file = fopen("parking.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp) return;

    char line[512];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char nom[100], id[10], localisation[100], capacite[10], dispo[10], prix[10], type[50], etat[50];
        sscanf(line, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]",
               nom, id, localisation, capacite, dispo, prix, type, etat);

        if (strcmp(id, id_search) == 0) {
            fprintf(temp, "%s;%s;%s;%s;%s;%s;%s;%s;0\n",
                    new_nom, new_id, new_localisation, new_capacite, new_dispo, new_prix, new_type, new_etat);
            found = 1;
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove("parking.txt");
        rename("temp.txt", "parking.txt");
    } else {
        remove("temp.txt");
        // Optionally, notify the user that no matching parking was found.
    }

    // Clear the input fields after successful modification
    gtk_entry_set_text(GTK_ENTRY(entry_nompark), "");
    gtk_entry_set_text(GTK_ENTRY(entry_idpark), "");
    gtk_entry_set_text(GTK_ENTRY(entry_localisationpark), "");
    gtk_entry_set_text(GTK_ENTRY(entry_capacitepark), "");
    gtk_entry_set_text(GTK_ENTRY(entry_nbrdispopark), "");
    gtk_entry_set_text(GTK_ENTRY(entry_prixpark), "");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_etatpark), -1);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_typepark), -1);
}


void on_button_assign_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry_idagent = lookup_widget(GTK_WIDGET(button), "entry_idagent");
    GtkWidget *entry_asignpark = lookup_widget(GTK_WIDGET(button), "entry_asignpark");

    const gchar *idagent_str = gtk_entry_get_text(GTK_ENTRY(entry_idagent));
    const gchar *asignpark_str = gtk_entry_get_text(GTK_ENTRY(entry_asignpark));

    int idagent = atoi(idagent_str);
    int asignpark = atoi(asignpark_str);

    // Open parking.txt file to find and update parking data
    FILE *parking_file = fopen("parking.txt", "r+");
    if (parking_file == NULL) {
        printf("Error opening parking file\n");
        return;
    }

    FILE *temp_parking_file = fopen("temp_parking.txt", "w");
    if (temp_parking_file == NULL) {
        printf("Error opening temporary parking file\n");
        fclose(parking_file);
        return;
    }

    char line[256];
    Parking p;
    int agent_found = 0;

    // Loop through parking.txt to find the parking and update the assigned agent
    while (fgets(line, sizeof(line), parking_file)) {
        sscanf(line, "%99[^;];%d;%99[^;];%d;%d;%d;%99[^;];%99[^;];%d", 
               p.nompark, &p.idpark, p.localisationpark, &p.capacitepark, 
               &p.nbrdispopark, &p.prixpark, p.etatpark, p.typepark, &p.id_agents);

        if (p.idpark == asignpark) {
            p.id_agents = idagent;  // Assign the agent to the parking
            agent_found = 1;
        }

        // Write the updated parking data to the temporary file
        fprintf(temp_parking_file, "%s;%d;%s;%d;%d;%d;%s;%s;%d\n", 
                p.nompark, p.idpark, p.localisationpark, p.capacitepark, 
                p.nbrdispopark, p.prixpark, p.etatpark, p.typepark, p.id_agents);
    }

    fclose(parking_file);
    fclose(temp_parking_file);

    // If the agent was found and assigned, replace the old parking file
    if (agent_found) {
        rename("temp_parking.txt", "parking.txt");
        printf("Agent assigned successfully.\n");

        // Now open agents.txt to update the agent's status from 'unassigned' to 'assigned'
        FILE *agents_file = fopen("agents.txt", "r+");
        if (agents_file == NULL) {
            printf("Error opening agents file\n");
            return;
        }

        FILE *temp_agents_file = fopen("temp_agents.txt", "w");
        if (temp_agents_file == NULL) {
            printf("Error opening temporary agents file\n");
            fclose(agents_file);
            return;
        }

        Agent agent;
        // Loop through agents.txt to find the agent and update the status and parking ID
        while (fgets(line, sizeof(line), agents_file)) {
            sscanf(line, "%d %d %d %s %s %s %s %d-%d-%d %s %d %s %s %s %s %s", 
                   &agent.idagent, &agent.idparking, &agent.cin, agent.nom, agent.prenom, 
                   agent.adresse, agent.role, &agent.jour, &agent.mois, &agent.annee, 
                   agent.sexe, &agent.tel, agent.exp, agent.statut, agent.email, 
                   agent.image, agent.experience);

            // If the agent's ID matches the one being assigned, update their status and parking ID
            if (agent.idagent == idagent) {
                strcpy(agent.statut, "assigned");
                agent.idparking = asignpark;  // Update parking ID in agent
            }

            // Write the updated agent data to the temporary file
            fprintf(temp_agents_file, "%d %d %d %s %s %s %s %d-%d-%d %s %d %s %s %s %s %s\n",
                    agent.idagent, agent.idparking, agent.cin, agent.nom, agent.prenom, 
                    agent.adresse, agent.role, agent.jour, agent.mois, agent.annee, 
                    agent.sexe, agent.tel, agent.exp, agent.statut, agent.email, 
                    agent.image, agent.experience);
        }

        fclose(agents_file);
        fclose(temp_agents_file);

        // Replace the old agents file with the updated one
        rename("temp_agents.txt", "agents.txt");
    } else {
        printf("Parking ID not found.\n");
        remove("temp_parking.txt");  // Clean up if no assignment was made
    }
}

void setup_treeview2_columns(GtkTreeView *treeview) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Create the cell renderer for text
    renderer = gtk_cell_renderer_text_new();

    // Setup columns similar to treeview1
    column = gtk_tree_view_column_new_with_attributes("Nom Parking", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("ID Parking", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("ID Agent 1", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("ID Agent 2", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("ID Agent 3", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("ID Agent 4", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(treeview, column);

    column = gtk_tree_view_column_new_with_attributes("ID Agent 5", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(treeview, column);
}

void populate_treeview2(GtkTreeView *treeview, const char *filename) {
    GtkListStore *store;
    GtkTreeIter iter;
    FILE *file = fopen(filename, "r");
    char line[256];
    int idparking;
    char nom_parking[50];
    char agent1[50], agent2[50], agent3[50], agent4[50], agent5[50];

    // Create a GtkListStore with 7 columns
    store = gtk_list_store_new(7, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
                               G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    // Set the model for the treeview
    gtk_tree_view_set_model(treeview, GTK_TREE_MODEL(store));

    // Read data from shift.txt
    while (fgets(line, sizeof(line), file)) {
        // Parse the line
        sscanf(line, "%d;%49[^;];%49[^;];%49[^;];%49[^;];%49[^;];%49[^;\n]", 
               &idparking, nom_parking, agent1, agent2, agent3, agent4, agent5);

        // Append data to the store
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, nom_parking,          // Nom Parking
            1, g_strdup_printf("%d", idparking),  // ID Parking
            2, strlen(agent1) > 0 ? agent1 : "N/A", // Agent 1
            3, strlen(agent2) > 0 ? agent2 : "N/A", // Agent 2
            4, strlen(agent3) > 0 ? agent3 : "N/A", // Agent 3
            5, strlen(agent4) > 0 ? agent4 : "N/A", // Agent 4
            6, strlen(agent5) > 0 ? agent5 : "N/A", // Agent 5
            -1);  // End of row data
    }

    fclose(file);
}

void populate_shift_txt_from_parking_and_agents() {
    FILE *parking_file = fopen("parking.txt", "r");
    if (parking_file == NULL) {
        printf("Error opening parking file\n");
        return;
    }

    FILE *agents_file = fopen("agents.txt", "r");
    if (agents_file == NULL) {
        printf("Error opening agents file\n");
        fclose(parking_file);
        return;
    }

    FILE *shift_file = fopen("shift.txt", "w");
    if (shift_file == NULL) {
        printf("Error opening shift file\n");
        fclose(parking_file);
        fclose(agents_file);
        return;
    }

    char line[256];
    Parking parking;
    Agent agent;
    int agent_count;
    char agent_ids[5][50];  // To store the agent IDs for this parking, up to 5 agents

    // Loop through the parking.txt to process each parking
    while (fgets(line, sizeof(line), parking_file)) {
        sscanf(line, "%99[^;];%d;%99[^;];%d;%d;%d;%99[^;];%99[^;];%d", 
               parking.nompark, &parking.idpark, parking.localisationpark, &parking.capacitepark, 
               &parking.nbrdispopark, &parking.prixpark, parking.etatpark, parking.typepark, &parking.id_agents);

        // Reset agent IDs
        memset(agent_ids, 0, sizeof(agent_ids));
        agent_count = 0;

        // Now check for agents associated with this parking in agents.txt
        rewind(agents_file);
        while (fgets(line, sizeof(line), agents_file)) {
            sscanf(line, "%d %d %d %s %s %s %s %d-%d-%d %s %d %s %s %s %s %s--", 
                   &agent.idagent, &agent.idparking, &agent.cin, agent.nom, agent.prenom, 
                   agent.adresse, agent.role, &agent.jour, &agent.mois, &agent.annee, 
                   agent.sexe, &agent.tel, agent.exp, agent.statut, agent.email, 
                   agent.image, agent.experience);

            // If the agent is assigned to the current parking, store their ID
            if (agent.idparking == parking.idpark && agent_count < 5) {
                sprintf(agent_ids[agent_count], "%d", agent.idagent);
                agent_count++;
            }
        }

        // Write parking and agent data into shift.txt
        fprintf(shift_file, "%d;%s;", parking.idpark, parking.nompark);
        for (int i = 0; i < 5; i++) {
            if (strlen(agent_ids[i]) > 0) {
                fprintf(shift_file, "%s;", agent_ids[i]);  // Write agent ID
            } else {
                fprintf(shift_file, "N/A;");  // If no agent assigned, write "N/A"
            }
        }
        fprintf(shift_file, "\n");
    }

    fclose(parking_file);
    fclose(agents_file);
    fclose(shift_file);

    printf("shift.txt updated successfully.\n");
}


void on_btn_refreshagent_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *treeview2 = lookup_widget(GTK_WIDGET(button), "treeview2");

    // Setup treeview2 columns
    setup_treeview2_columns(GTK_TREE_VIEW(treeview2));

    // Populate treeview2 with updated data from shift.txt
    populate_shift_txt_from_parking_and_agents();  // Ensure shift.txt is updated
    populate_treeview2(GTK_TREE_VIEW(treeview2), "shift.txt");
}

// Comparison functions
int compare_parking_by_id(const void *a, const void *b) {
    return ((Parking *)a)->idpark - ((Parking *)b)->idpark;
}

int compare_parking_by_nom(const void *a, const void *b) {
    return strcmp(((Parking *)a)->nompark, ((Parking *)b)->nompark);
}

int compare_parking_by_capacite(const void *a, const void *b) {
    return ((Parking *)b)->capacitepark - ((Parking *)a)->capacitepark;
}

int compare_parking_by_prix(const void *a, const void *b) {
    return ((Parking *)a)->prixpark - ((Parking *)b)->prixpark;
}

// Function to read parking data from file
int load_parking_data(Parking *parkings, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening parking.txt");
        return 0;
    }
    int count = 0;
    while (fscanf(file, "%99[^;];%d;%99[^;];%d;%d;%d;%49[^;];%49[^;];%d\n",
                  parkings[count].nompark, &parkings[count].idpark, parkings[count].localisationpark,
                  &parkings[count].capacitepark, &parkings[count].nbrdispopark, &parkings[count].prixpark,
                  parkings[count].typepark, parkings[count].etatpark, &parkings[count].id_agents) != EOF) {
        count++;
    }
    fclose(file);
    return count;
}

// Function to write parking data back to file
void save_parking_data(Parking *parkings, int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error writing to parking.txt");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s;%d;%s;%d;%d;%d;%s;%s;%d\n",
                parkings[i].nompark, parkings[i].idpark, parkings[i].localisationpark,
                parkings[i].capacitepark, parkings[i].nbrdispopark, parkings[i].prixpark,
                parkings[i].typepark, parkings[i].etatpark, parkings[i].id_agents);
    }
    fclose(file);
}

// Generalized sort function
void sort_parking_data(GtkToggleButton *togglebutton, int (*compare_func)(const void *, const void *)) {
    if (!gtk_toggle_button_get_active(togglebutton)) {
        return;
    }

    Parking parkings[100];
    int count = load_parking_data(parkings, "parking.txt");
    if (count > 0) {
        qsort(parkings, count, sizeof(Parking), compare_func);
        save_parking_data(parkings, count, "parking.txt");
    }
}

// Sorting callbacks
void on_btn_sort_by_id_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    sort_parking_data(togglebutton, compare_parking_by_id);
}

void on_btn_sort_by_nom_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    sort_parking_data(togglebutton, compare_parking_by_nom);
}
void on_btn_sort_by_prix_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    sort_parking_data(togglebutton, compare_parking_by_prix);
}

void on_btn_sort_by_capacite_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    sort_parking_data(togglebutton, compare_parking_by_capacite);
}







static gdouble current_progress = 0.0;  // Global variable to track the current progress
static gdouble target_progress = 0.0;   // Global variable to track the target progress

// This function will incrementally update the progress bar towards the target value
gboolean update_progress(gpointer progress_bar) {
    if (current_progress < target_progress) {
        current_progress += 0.005;  // Increment progress by 1% per call (adjust this rate)
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), current_progress);
        return TRUE;  // Continue the timeout function
    }
    return FALSE;  // Stop the timeout function once the progress reaches the target
}

void on_btn_visual_clicked(GtkButton *button, gpointer user_data) {
    GtkProgressBar *progress_bar = GTK_PROGRESS_BAR(lookup_widget(GTK_WIDGET(button), "progress_parking_availability"));
    GtkEntry *entry_search_park = GTK_ENTRY(lookup_widget(GTK_WIDGET(button), "entry_search_park"));
    const char *search_input = gtk_entry_get_text(entry_search_park);  // Get ID from the entry

    FILE *file = fopen("parking.txt", "r");
    if (!file) {
        printf("Error opening parking.txt\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    Parking parking_data;
    gboolean found = FALSE;

    // Search for the parking by ID in the file
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ";");

        // Read each token and populate the parking_data structure
        int field_index = 0;
        while (token != NULL) {
            switch (field_index) {
                case 0:
                    strncpy(parking_data.nompark, token, sizeof(parking_data.nompark));
                    break;
                case 1:
                    parking_data.idpark = atoi(token);
                    break;
                case 2:
                    strncpy(parking_data.localisationpark, token, sizeof(parking_data.localisationpark));
                    break;
                case 3:
                    parking_data.capacitepark = atoi(token);
                    break;
                case 4:
                    parking_data.nbrdispopark = atoi(token);
                    break;
                case 5:
                    parking_data.prixpark = atoi(token);
                    break;
                case 6:
                    strncpy(parking_data.typepark, token, sizeof(parking_data.typepark));
                    break;
                case 7:
                    strncpy(parking_data.etatpark, token, sizeof(parking_data.etatpark));
                    break;
                case 8:
                    parking_data.id_agents = atoi(token);
                    break;
            }
            token = strtok(NULL, ";");
            field_index++;
        }

        // Check if the parking ID matches the search input
        if (parking_data.idpark == atoi(search_input)) {
            found = TRUE;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Parking ID not found.\n");
        return;
    }

    // Ensure capacitepark and nbrdispopark are valid before calculating
    if (parking_data.capacitepark == 0 || parking_data.nbrdispopark == 0) {
        printf("Invalid capacite or disponible values: capacite = %d, disponible = %d\n", parking_data.capacitepark, parking_data.nbrdispopark);
        return;
    }

    // Calculate the percentage of available space
    gdouble percentage = ((double) parking_data.nbrdispopark / parking_data.capacitepark) * 100.0;

    // Ensure percentage is within valid range (0 to 100)
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;

    // Set the target progress value based on the calculated percentage
    target_progress = percentage / 100.0;

    // Initialize the current progress and start the loading animation
    current_progress = 0.0;

    // Set the progress bar to start the animation
    g_timeout_add(20, update_progress, progress_bar);  // Update every 20ms for smooth progress

    // Optionally update the progress bar text (optional)
    gchar *progress_text = g_strdup_printf("%.2f%% Available", percentage);
    gtk_progress_bar_set_text(progress_bar, progress_text);
    g_free(progress_text);
}

