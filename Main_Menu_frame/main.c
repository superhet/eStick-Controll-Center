// A program
// which displays the main window frame without any window content
//14.04.2011 V 0.0 created by AT
//17.04.2011 
/*#################################*/
/* Libaries */
/*#################################*/
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define debug 0
/*#################################*/
/*Function Prototypes*/
/*#################################*/
GdkPixbuf *create_pixbuf(const gchar * filename);
void toggle_statusbar(GtkWidget *widget, gpointer statusbar);
void toggle_toolbar(GtkWidget *widget, gpointer toolbar);
/*#################################*/
/*Global Variables*/
/*#################################*/
int main( int argc, char *argv[])
{
  /* Create the Widgets */
  GtkWidget *window, *vbox, *menubar, *filemenu,*editmenu,*viewmenu,*helpmenu,
  *file, *edit, *view, *help,*pref_1,*toolbar,*about_Dialog,*help_Dialog,
  *new, *open, *quit, *sep, *tog_stat, *tog_tool, *statusbar;
  
  GtkAccelGroup *accel_group = NULL;
  GtkToolItem *test_item;

  /* For GTK always necessary */
  gtk_init(&argc, &argv);
  /* Create window on toplevel and fill in the parameters */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  /* Create WIndow always in the middle of the screen */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  /* Default Window size at startup */
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
  /* Set the window title */
  gtk_window_set_title(GTK_WINDOW(window), "eStick Control Center V0.0-ecc");
  /* Add Window Icon */
  gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("window_symbol.png"));
  /* Set Border With of the window in Pixels */
  gtk_container_set_border_width(GTK_CONTAINER(window), 0);
  /* Create a new Vertical Box, FALSE= WIdgets have not equal space, 0 pixels between widgets */
  vbox = gtk_vbox_new(FALSE, 0);
  /* Add the Vertical Box to the main window */
  gtk_container_add(GTK_CONTAINER(window), vbox);
  /* Create the Menu Bar */
  menubar = gtk_menu_bar_new();
  /* Create the Toolbar Bar */
  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
  gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);
  /* Create a new Statusbar*/
  statusbar = gtk_statusbar_new();
  /* Create the Handlers for the submenus*/
  filemenu = gtk_menu_new();
  editmenu = gtk_menu_new();
  viewmenu = gtk_menu_new();
  helpmenu = gtk_menu_new();
  /* Create new accelerator Group */
  accel_group = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);
  /* Create the File Drop Down Point in the menu bar with mnemonic character */
  file = gtk_menu_item_new_with_mnemonic("_File");
  /* Create the Edit Drop Down Point in the menu bar with mnemonic character */
  edit = gtk_menu_item_new_with_mnemonic("_Edit");
  /* Create the View Drop Down Point in the menu bar with mnemonic character */
  view = gtk_menu_item_new_with_mnemonic("_View");
  /* Create the Help Drop Down Point in the menu bar with mnemonic character */
  help = gtk_menu_item_new_with_mnemonic("_Help");
  /* ############################# Items for the Menu Bar################## */
  /* Use the New and the open point from the libary and their stock symbols */
  new = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
  open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
  /* Create vertical seperator line in the file menu */
  sep = gtk_separator_menu_item_new();
  quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);
  /* Add accelerator to quit point in the file menu */
  gtk_widget_add_accelerator(quit, "activate", accel_group,GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
  /* ############################# Items for the Edit Bar################## */
  pref_1= gtk_menu_item_new_with_label("Preferences");
  /* ############################# Items for the View Bar################## */
  tog_stat = gtk_check_menu_item_new_with_label("View Statusbar");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(tog_stat), TRUE);
  tog_tool = gtk_check_menu_item_new_with_label("View Toolbar");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(tog_tool), TRUE);
  /* ############################# Items for the Help Bar################## */
   help_Dialog= gtk_menu_item_new_with_label("Get Help Online");
   about_Dialog= gtk_menu_item_new_with_label("About");
  /* Add the File menu and the point content points to the menu bar */
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), sep);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
  /* Add the Edit menu and the point content points to the menu bar */
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), editmenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), pref_1);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), edit);
  /* Add the View menu and the point content points to the menu bar */
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), viewmenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), tog_stat);
  gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), tog_tool);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), view);
  /* Add the Help menu and the point content points to the menu bar */
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), helpmenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu), help_Dialog);
  gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu), about_Dialog);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help);
  /* ############################# Items for the Toolbar Bar################## */
  test_item = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), test_item, -1);

   /* Add the whole menubar at the beginning of the vbox  */
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
  /* Add the whole toolbar  at the beginning of the vbox after the menu bar */
  gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 5);

 
  /* Add the statusbar to the end of the vbox */
  gtk_box_pack_end(GTK_BOX(vbox), statusbar, FALSE, TRUE, 0);
  /*Connect signal to view the Preferences Tab*/
 // g_signal_connect(G_OBJECT(pref_1), "activate",G_CALLBACK(NULL), NULL);
  /*Connect signal to view / hide the statusbar*/
  g_signal_connect(G_OBJECT(tog_stat), "activate",G_CALLBACK(toggle_statusbar), statusbar);
  /*Connect signal to view / hide the tool*/
  g_signal_connect(G_OBJECT(tog_tool), "activate",G_CALLBACK(toggle_toolbar), toolbar);
  /* necessary for the (x) Button in the right corner */
  g_signal_connect_swapped(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);
  /* Signal for the Quit function in the menu */  
  g_signal_connect(G_OBJECT(quit), "activate",G_CALLBACK(gtk_main_quit), NULL);
   
  /* Show all widgets in the main window*/
  gtk_widget_show_all(window);

  /* GTK main loop, wait for events */
  gtk_main();

  return 0;
}
/*######################################*/
/*Function for displying the window Icon*/
/*######################################*/

GdkPixbuf *create_pixbuf(const gchar * filename)
{
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   if(!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}
/*######################################*/
/*View/Hide the statusbar*/
/*######################################*/
void toggle_statusbar(GtkWidget *widget, gpointer statusbar) 
{
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
    gtk_widget_show(statusbar);
  } else {
    gtk_widget_hide(statusbar);
  }
}
/*######################################*/
/*View/Hide the toolbar*/
/*######################################*/
void toggle_toolbar(GtkWidget *widget, gpointer toolbar) 
{
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
    gtk_widget_show(toolbar);
  } else {
    gtk_widget_hide(toolbar);
  }
}

