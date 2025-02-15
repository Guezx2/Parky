#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"

int main(int argc, char *argv[]) {
    GtkWidget *Parking;
#ifdef ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif

    gtk_set_locale();
    gtk_init(&argc, &argv);

    add_pixmap_directory("/home/guezguez/Desktop/NEWWWWWWWWWWW/pixmaps");

    /*
     * The following code was added by Glade to create one of each component
     * (except popup menus), just so that you see something after building
     * the project. Delete any components that you don't want shown initially.
     */
    Parking = create_Parking();
    gtk_widget_show(Parking);

    gtk_main();
    return 0;
}
