// A program
// which displays the main window with full menus and working statusbar
//24.04.2011 V 0.0 created by AT
//25.04.2011-26.04.2011 Build Statusbar Messages working correct
//29.04.2011 put first things for Drawing the Graph in the window
//03.05.2011 paint graph background white
//03.05.2011 work on the file open dialog
/*#################################*/
/* Libaries */
/*#################################*/
#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>
#include <gdk/gdkkeysyms.h>

#define debug 0
/*#################################*/
/*Function Prototypes*/
/*#################################*/
GdkPixbuf *create_pixbuf(const gchar * filename);

void toggle_statusbar(GtkWidget *widget, gpointer statusbar);
void toggle_toolbar(GtkWidget *widget, gpointer toolbar);
void show_about(GtkWidget *widget, gpointer data);
void file_open(GtkWidget *widget, gpointer data);
void file_save(GtkWidget *widget, gpointer data);

void push_item( GtkWidget *widget,gpointer data );
void pop_item( GtkWidget *widget,gpointer data );

static gboolean on_expose_event(GtkWidget *widget,GdkEventExpose *event,gpointer data);

static gboolean statusbar_hint (GtkMenuItem *menuitem,GdkEventProximity *event,GtkStatusbar *statusbar);    
/*#################################*/
/*Global Variables*/
/*#################################*/
int main( int argc, char *argv[])
{
  /* Create the Widgets */
  GtkWidget *window, *vbox, *menubar, *filemenu,*editmenu,*viewmenu,*helpmenu,
  *file, *edit, *view, *help,*pref_1,*toolbar,*help_Dialog,*about_Dialog,
  *new,*sep1,*sep2, *open,*save,*dialog, *save_as,*export, *quit, *sep, *tog_stat,*statusbar, *tog_tool;
  /*Widget for the Graph Drawing Area*/
  GtkWidget *darea;
  
  /* Some nesseary inits */
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
  /* Add Window Icon max size 128x128 Pixel current 64x64 used */
  gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("window_symbol.png"));
  /* Set Border With of the window in Pixels */
  gtk_container_set_border_width(GTK_CONTAINER(window), 0);
  /* Create a new Vertical Box, FALSE= WIdgets have not equal space, 0 pixels between widgets */
  vbox = gtk_vbox_new(FALSE, 0);
  /* Add the Vertical Box to the main window */
  gtk_container_add(GTK_CONTAINER(window), vbox);
  /*Create the Drawing area for the graph*/
  darea = gtk_drawing_area_new();
  /* Create the Menu Bar */
  menubar = gtk_menu_bar_new();
  /* Create the Toolbar Bar */
  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
  gtk_container_set_border_width(GTK_CONTAINER(toolbar), 0);
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
  /* Use the open point from the libary and the stock symbol */
  open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
  g_signal_connect(G_OBJECT(open), "activate",G_CALLBACK(file_open),file_open);
  /*Connect signal to display the Message for the Save Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (open), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (open), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (open), "menuhint",(gpointer) "Open a recorded File");
  /* Create vertical seperator line in the file menu */
  sep1 = gtk_separator_menu_item_new();
  /* Use the save point from the libary and the stock symbol */
  save = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, NULL);
  /*Connect signal to display the Message for the Save Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (save), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (save), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (save), "menuhint",(gpointer) "Save to specified File");
  /* Use the save_as point from the libary and the stock symbol */
  save_as = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, NULL);
  //g_signal_connect(G_OBJECT(save_as), "activate",G_CALLBACK(file_save_as),file_save_as);
  /*Connect signal to display the Message for the Save_as Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (save_as), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (save_as), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (save_as), "menuhint",(gpointer) "Save to a new File");
  /* Create vertical seperator line in the file menu */
  sep2 = gtk_separator_menu_item_new();
  /* For the export function, no stock symbol exists, used instead a point with only a label*/
  export = gtk_menu_item_new_with_label("Export");
  /*Connect signal to display the Message for the Export Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (export), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (export), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (export), "menuhint",(gpointer) "Export Recorded File");
  /* Create vertical seperator line in the file menu */
  sep = gtk_separator_menu_item_new();
  quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);
  /*Connect signal to display the Message for the Quit Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (quit), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (quit), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (quit), "menuhint",(gpointer) "End Program");
  /* Add accelerator to quit point in the file menu */
  gtk_widget_add_accelerator(quit, "activate", accel_group,GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
  /* ############################# Items for the Edit Bar################## */
  pref_1= gtk_menu_item_new_with_label("Preferences");
  /*Connect signal to display the Message for the Preferneces Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (pref_1), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (pref_1), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (pref_1), "menuhint",(gpointer) "Show the Preferences Dialog");
  /* ############################# Items for the View Bar################## */
  /*Menu Item for Show Hide the Statusbarbar*/
  tog_stat = gtk_check_menu_item_new_with_label("View Statusbar");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(tog_stat), TRUE);
  /*Connect signal to display the Message for the show/hide Statusbar Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (tog_stat), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (tog_stat), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (tog_stat), "menuhint",(gpointer) "Show/Hide the Statusbar");
  /*Menu Item for Show Hide the Toolbar*/
  tog_tool = gtk_check_menu_item_new_with_label("View Toolbar");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(tog_tool), TRUE);
  /*Connect signal to display the Message for the show/hide Toolbar Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (tog_tool), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (tog_tool), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (tog_tool), "menuhint",(gpointer) "Show/Hide the Toolbar");
  /* ############################# Items for the Help Bar################## */
  help_Dialog= gtk_menu_item_new_with_label("Get Help Online");
  /*Connect signal to display the Message for the help dialog in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (help_Dialog), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (help_Dialog), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (help_Dialog), "menuhint",(gpointer) "Get Online Help");

  about_Dialog= gtk_menu_item_new_with_label("About");
  /*Connect signal to view the about dialog*/
  g_signal_connect(G_OBJECT(about_Dialog), "activate",G_CALLBACK(show_about),show_about);
  /*Connect signal to display the Message for the about dialog in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (about_Dialog), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves the about menu point*/ 
  g_signal_connect (G_OBJECT (about_Dialog), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (about_Dialog), "menuhint",(gpointer) "Shows the about Dialog of this Program");

  /* Add the File menu and the point content points to the menu bar */
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), sep1);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save_as);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), sep2);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), export);
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
  /* Add the whole drawing area  at the beginning of the vbox after the toolbox bar */
  gtk_container_add(GTK_CONTAINER(vbox), darea);
  /* Add the statusbar to the end of the vbox */
  gtk_box_pack_end(GTK_BOX(vbox), statusbar, FALSE, TRUE, 0);
  /*Connect signal to view the Preferences Tab*/
  
  /*Connect signal to view / hide the statusbar*/
  g_signal_connect(G_OBJECT(tog_stat), "activate",G_CALLBACK(toggle_statusbar), statusbar);
  /*Connect signal to view / hide the toolbar*/
  g_signal_connect(G_OBJECT(tog_tool), "activate",G_CALLBACK(toggle_toolbar), toolbar);
  /*Connect signal for the preferences Dialog*/
 // g_signal_connect(G_OBJECT(pref_1), "activate",G_CALLBACK(NULL), NULL);
  /* necessary for the (x) Button in the right corner */
  g_signal_connect_swapped(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);
  /* Signal for the Quit function in the menu */  
  g_signal_connect(G_OBJECT(quit), "activate",G_CALLBACK(gtk_main_quit), NULL);
  
  g_signal_connect(darea, "expose-event",G_CALLBACK (on_expose_event), NULL);
  gtk_widget_set_app_paintable(window, TRUE);
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
/*######################################*/
/*About Dialog*/
/*######################################*/

void show_about(GtkWidget *widget, gpointer data)
{
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("about_symbol.png", NULL);
  GtkWidget *dialog = gtk_about_dialog_new();
  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "eStick Control Center ecc");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "0.1"); 
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"(c) Osman Jacob,Alois Taranetz,Tibor Elias");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "A Programm to record and playback data from the eStick");
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),"http://www.technikum-wien.at/studium/bachelor/elektronik/interna/?full=1080");
  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
  g_object_unref(pixbuf), pixbuf = NULL;
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}
/*######################################*/
/*Statusbar Messages*/
/*######################################*/
/*Display Message on the Statusbar*/
/*
/* Add or remove a status bar menu hint, depending on whether this function
* is initialized by a proximity-in-event or proximity-out-event. */
static gboolean statusbar_hint (GtkMenuItem *menuitem,GdkEventProximity *event,GtkStatusbar *statusbar)
{
  gchar *hint;
  guint id = gtk_statusbar_get_context_id (statusbar, "MenuItemHints");
if (event->type == GDK_ENTER_NOTIFY)
{
  hint = (gchar*) g_object_get_data (G_OBJECT (menuitem), "menuhint");
  gtk_statusbar_push (statusbar, id, hint);
}
else if (event->type == GDK_LEAVE_NOTIFY)
  gtk_statusbar_pop (statusbar, id);
return FALSE;
}
 
/*######################################*/
/*File Open Dialog*/
/*######################################*/

void file_open(GtkWidget *widget, gpointer data)
{
  GtkWidget *dialog;
  gchar *filename;
  GtkFileFilter *filter;
  gint result;

  /*Create the File Open Dialog*/
  dialog = gtk_file_chooser_dialog_new ("Open", NULL,
  /*Set File Chooser in Mode Open*/
  GTK_FILE_CHOOSER_ACTION_OPEN,GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,GTK_STOCK_OK, GTK_RESPONSE_OK,NULL);
  
  /*Set File Filter for File Open Dialog*/
  filter = gtk_file_filter_new ();
  gtk_file_filter_set_name(filter,"log");
  gtk_file_filter_add_pattern (filter, "*.log");
  gtk_file_chooser_add_filter (GTK_DIALOG (dialog),filter);

  result = gtk_dialog_run (GTK_DIALOG (dialog));
  if (result == GTK_RESPONSE_OK)
  {
     filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
     g_print ("Open: %s\n", filename);
  }
  gtk_widget_destroy (dialog);

}

/*######################################*/
/*File Save Dialog*/
/*######################################*/

/*######################################*/
/*File Save As Dialog*/
/*######################################*/
/*
void file_save_as(GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog;
    gchar *filename;
    dialog = gtk_file_chooser_dialog_new ("Save File As ...", NULL,GTK_FILE_CHOOSER_ACTION_SAVE,GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,NULL);
    gint result = gtk_dialog_run (GTK_DIALOG (dialog));
    if (result == GTK_RESPONSE_ACCEPT)
    {
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      //gtk_button_set_label (button, filename);
    }
    gtk_widget_destroy (dialog);
}
*/
/*######################################*/
/*Draw Lines in the Drawing Area*/
/*######################################*/
static gboolean on_expose_event(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
  cairo_t *cr;
  int i;
  cr = gdk_cairo_create (widget->window);
  /*Paint the Background white*/
  cairo_move_to(cr, 0, 0);  
  //cairo_translate(cr, 0, 0);
  /*Set Color for the Background*/
  cairo_set_source_rgb(cr, 255, 255, 255);
  cairo_paint(cr);
  /*Set the color for the lines of the grid*/
  cairo_set_source_rgba(cr, 0, 0, 0, 1);
  /*Set the line with for the lines of the grid*/
  cairo_set_line_width(cr, 1.5);

  for(i=20;i<8000;i=i+20)
  {
  cairo_move_to(cr, 0, i);  
  cairo_line_to(cr, 500, i);
  cairo_stroke(cr);
/*
  cairo_move_to(cr, 0, 50);  
  cairo_line_to(cr, 500, 50);
  cairo_stroke(cr);

  cairo_move_to(cr, 0, 70);  
  cairo_line_to(cr, 500, 70);
  cairo_stroke(cr);*/
  }
  cairo_destroy(cr);

  return FALSE;
}
