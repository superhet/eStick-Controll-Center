 /*This Program is the GUI for the eStickControlCenter*/

/*!
 * For the basic drawing function the example code from
 * M. Horauer, 2011
 * was used
 * gcc `pkg-config --cflags --libs gtk+-2.0` cairoplot.c -o demo -lm
 */
 /*10.05.2011 Try to combine my programm with the dem applikation*/
 /*11.05.2011 Doing Code Clean up*/
  /*31.05.2011 Split uo doesnt work took this version with all in functions in main*/

/*********************************************************** INCLUDES */
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <cairo.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <gdk/gdkkeysyms.h>
/************************************************************ DEFINES */
#define GRID 10
#define debug 0
/*#################################*/
/*Function Prototypes*/
/*#################################*/
gboolean draw_grid (cairo_t * cr);
gboolean draw_diagram (GtkWidget *widget);
/*Shown Name for the Graph*/

static gboolean on_expose_event(GtkWidget *widget,
                                GdkEventExpose *event, gpointer data);
static gpointer gui_thread (gpointer data);
static gboolean gui_handler (GtkWidget *widget);
static gpointer data_thread (gpointer data);
static gboolean data_handler (gpointer data);

GdkPixbuf *create_pixbuf(const gchar * filename);

void toggle_statusbar(GtkWidget *widget, gpointer statusbar);
void toggle_toolbar(GtkWidget *widget, gpointer toolbar);
void show_about(GtkWidget *widget, gpointer data);
void show_help(GtkWidget *widget, gpointer data);
void error_ext1(GtkWidget *widget, gpointer window);
void error_ext2(GtkWidget *widget, gpointer window);
void error_ext3(GtkWidget *widget, gpointer window);
void error_ext4(GtkWidget *widget, gpointer window);
void error_ext5(GtkWidget *widget, gpointer window);
void file_new(GtkWidget *widget, gpointer data);
void file_open(GtkWidget *widget, gpointer data);
static void file_save_as (GtkWidget *widget, GtkWindow *window);
static void file_save (GtkWidget *widget, GtkWindow *window);
static void preferences (GtkWidget *widget, GtkWindow *window);
//static void export_diag (GtkWidget *widget, GtkWindow *window);
void export_diag(GtkWidget *widget, gpointer data);
void push_item( GtkWidget *widget,gpointer data );
void pop_item( GtkWidget *widget,gpointer data );

static gboolean statusbar_hint (GtkMenuItem *menuitem,GdkEventProximity *event,GtkStatusbar *statusbar);    

void pref_check1_cb(GtkWidget *widget, gpointer window);
void pref_check2_cb(GtkWidget *widget, gpointer window);
void pref_check3_cb(GtkWidget *widget, gpointer window);
void pref_check_source1_cb(GtkWidget *widget, gpointer window);
void pref_check_source2_cb(GtkWidget *widget, gpointer window);
void pref_check_source3_cb(GtkWidget *widget, gpointer window);
void pref_check_source4_cb(GtkWidget *widget, gpointer window);

GtkWidget *label_graph;
/*************************************************** GLOBAL VARIABLES */
// create a mutex
G_LOCK_DEFINE (value);

// variables to draw the grid and the initial window
gint xsize = 600;
gint ysize = 400;
gint xoff = 50;
gint yoff = 50;
// global array that holds the values to be drawn
gint valuea[100];
gint valueb[100];

gchar *graph_name="default_graph";

/********************************************************** FUNCTIONS */

/*!
 * \brief data thread invokes a data generator after a short timeout
 */
static gpointer data_thread (gpointer data)
{
  g_timeout_add(55, (GSourceFunc) data_handler, NULL);
  return NULL;
}
#if debug
/*Generate Data for Data for Drawing Example*/
static gboolean data_handler (gpointer data)
{
  static gint j;

  G_LOCK(value);
//  for(j=0;j<=100;j++)
//    valuea[j]=g_random_int ()%ysize;
//
  for(j=0;j<=100;j++)
  {
    valueb[j]=(gint)(sin(j*3.1415/180*5)*(ysize+yoff)/10)+ysize/2;
  }
  G_UNLOCK(value);
  return TRUE;
}
#endif
/*Generate Data fill array with the data for drawing the graph*/
static gboolean data_handler (gpointer data)
{
  static gint j;

  G_LOCK(value);
//  for(j=0;j<=100;j++)
//    valuea[j]=g_random_int ()%ysize;
//
  for(j=0;j<=100;j++)
  {
   // valueb[j]=(gint)(sin(j*3.1415/180*5)*(ysize+yoff)/10)+ysize/2;
   valueb[j]=(gint)(200);
  }
  G_UNLOCK(value);
  return TRUE;
}

/*!
 * \brief GUI thread only create a plot surface and periodically call a
 * gui_handler that redraws the plot after a short timeout
 */
static gpointer
gui_thread (gpointer data)
{
  GtkWidget *window;
  GtkWidget *drawing_area;
    /* Create the Widgets */
  GtkWidget *vbox, *menubar, *filemenu,*editmenu,*viewmenu,*helpmenu,
  *file, *edit, *view, *help,*pref_1,*toolbar,*help_Dialog,*about_Dialog,
  *new,*sep1,*sep2, *open,*save,*dialog, *save_as,*export,*export_diag, *quit, *sep, *tog_stat,*statusbar, *tog_tool;
  gdk_threads_enter();
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  label_graph = gtk_label_new(graph_name);
  
  
  /*
  GtkWidget *label_graph;
  label_graph = gtk_label_new(graph_name);*/
   /* Some nesseary inits */
  GtkAccelGroup *accel_group = NULL;
  //GtkToolItem *test_item;
  GtkToolItem *item_1;
  GtkToolItem *item_2;
  GtkToolItem *item_3;
  GtkToolItem *item_4;
  GtkToolItem *item_5; 
  /* Set the window title */
  gtk_window_set_title(GTK_WINDOW(window), "eStick Control Center V0.0-ecc");
  /* Add Window Icon max size 128x128 Pixel current 64x64 used */
  gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("window_symbol.png"));
  /* Default Window size at startup */
  gtk_window_set_default_size (GTK_WINDOW(window), xsize+xoff, ysize+yoff);
  /* Set Border With of the window in Pixels */
  gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    /* Create a new Vertical Box, FALSE= WIdgets have not equal space, 0 pixels between widgets */
  vbox = gtk_vbox_new(FALSE, 0);
  /* Add the Vertical Box to the main window */
  gtk_container_add(GTK_CONTAINER(window), vbox);
  drawing_area = gtk_drawing_area_new ();
  gtk_widget_set_size_request (drawing_area, xsize, ysize);
  
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
  /* Use the new point from the libary and the stock symbol */
  new = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
  g_signal_connect(G_OBJECT(new), "activate",G_CALLBACK(file_new),file_new);
  /*Connect signal to display the Message for the Save Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (new), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (new), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (new), "menuhint",(gpointer) "Create a New Log File");    
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
  g_signal_connect(G_OBJECT(save), "activate",G_CALLBACK(file_save),(gpointer) window);
  /*Connect signal to display the Message for the Save Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (save), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (save), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (save), "menuhint",(gpointer) "Save to specified File");
  /* Use the save_as point from the libary and the stock symbol */
  save_as = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, NULL);
  g_signal_connect(G_OBJECT(save_as), "activate",G_CALLBACK(file_save_as),(gpointer) window);
  /*Connect signal to display the Message for the Save_as Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (save_as), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (save_as), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (save_as), "menuhint",(gpointer) "Save to a new File");
  /* Create vertical seperator line in the file menu */
  sep2 = gtk_separator_menu_item_new();
  /* For the export function, no stock symbol exists, used instead a point with only a label*/
  export = gtk_menu_item_new_with_label("Export Log File");
  /*Connect signal to display the Message for the Export Menu Item in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (export), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (export), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (export), "menuhint",(gpointer) "Export Recorded File");
  /*Attach the callback function for the export dialog look at the function for more debug info*/
  //g_signal_connect(G_OBJECT(export), "activate",G_CALLBACK(export_diag),(gpointer) window);
  //g_signal_connect(G_OBJECT(export), "activate",G_CALLBACK(export_diag),export_diag);
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
  /*Attach the callback function for preferences dialog*/
  g_signal_connect(G_OBJECT(pref_1), "activate",G_CALLBACK(preferences),(gpointer) window);  
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
   /*Connect signal to view the help dialog*/
  g_signal_connect(G_OBJECT(help_Dialog), "activate",G_CALLBACK(show_help),(gpointer) window);
  /*Connect signal to display the Message for the help dialog in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (help_Dialog), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves this menu point*/ 
  g_signal_connect (G_OBJECT (help_Dialog), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (help_Dialog), "menuhint",(gpointer) "Get Online Help");
  about_Dialog= gtk_menu_item_new_with_label("About");
  /*Connect signal to view the about dialog*/
  g_signal_connect(G_OBJECT(about_Dialog), "activate",G_CALLBACK(show_about),show_about);
  /*for Testing Purposes only*/
  //g_signal_connect(G_OBJECT(about_Dialog), "activate",G_CALLBACK(error_ext3),(gpointer) window);
  
  /*Connect signal to display the Message for the about dialog in the statusbar, when the mouse cursors hovers over*/
  g_signal_connect (G_OBJECT (about_Dialog), "enter-notify-event",G_CALLBACK (statusbar_hint), statusbar);
  /*Remove Message from the statusbar when the mouse cursors leaves the about menu point*/ 
  g_signal_connect (G_OBJECT (about_Dialog), "leave-notify-event",G_CALLBACK (statusbar_hint), statusbar); 
  /*Attach the text for the statusbar to the Menu Item*/
  g_object_set_data (G_OBJECT (about_Dialog), "menuhint",(gpointer) "Shows the about Dialog of this Program");
   /* Add the File menu and the point content points to the menu bar */
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);
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
  //test_item = gtk_tool_button_new_from_stock(GTK_STOCK_PREFERENCES);
  item_1 = gtk_tool_button_new_from_stock(GTK_STOCK_EXECUTE);
  item_2 = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PREVIOUS);
  item_3 = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_NEXT);
  item_4 = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PAUSE);
  item_5 = gtk_tool_button_new_from_stock(GTK_STOCK_CONVERT);
  //gtk_toolbar_insert(GTK_TOOLBAR(toolbar), test_item, -1);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item_1, -2);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item_2, -3);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item_3, -4);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item_4, -5);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item_5, -6);

   /* Add the whole menubar at the beginning of the vbox  */
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
  /* Add the whole toolbar  at the beginning of the vbox after the menu bar */
  gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 5);
  /* Add the whole drawing area  at the beginning of the vbox after the toolbox bar */
  gtk_container_add (GTK_CONTAINER (vbox), drawing_area);
  /*Name of the data graph*/
 
  gtk_container_add (GTK_CONTAINER (vbox), label_graph);
  /* Add the statusbar to the end of the vbox */
  gtk_box_pack_end(GTK_BOX(vbox), statusbar, FALSE, TRUE, 0);
  
  /*Connect signal to view / hide the statusbar*/
  g_signal_connect(G_OBJECT(tog_stat), "activate",G_CALLBACK(toggle_statusbar), statusbar);
  /*Connect signal to view / hide the toolbar*/
  g_signal_connect(G_OBJECT(tog_tool), "activate",G_CALLBACK(toggle_toolbar), toolbar);
  /*Connect signal for the preferences Dialog*/
 // g_signal_connect(G_OBJECT(pref_1), "activate",G_CALLBACK(NULL), NULL);
  /* necessary for the (x) Button in the right corner */
   
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "expose_event",
	            G_CALLBACK (on_expose_event), NULL);
  gtk_widget_show_all (GTK_WIDGET (window));
  // periodically redraw the data
  g_timeout_add(100, (GSourceFunc) gui_handler, (gpointer) window);

  gtk_main ();
  gdk_threads_leave();
  return NULL;
}

/*!
 * \brief simply redraw the drawing area
 */
static gboolean
gui_handler (GtkWidget *widget)
{
  gtk_widget_queue_draw(widget);
  return TRUE;
}

/*!
 * \brief main only creates two threads
 */
int
main (int argc, char *argv[])
{
  GThread          *GuiThread;
  GThread          *DataThread;
  GError           *err1 = NULL ;
  GError           *err2 = NULL ;

  /*Widget for the Graph Drawing Area*/
  GtkWidget *darea; 
  /* Some nesseary inits */
  GtkAccelGroup *accel_group = NULL;  
  //GtkToolItem *test_item;
  GtkToolItem *item_1;
  GtkToolItem *item_2;
  GtkToolItem *item_3;
  GtkToolItem *item_4;
  GtkToolItem *item_5;
  // init thread support and GTK+
  if( !g_thread_supported() )
  {
	  /* For GTK always necessary */
    g_thread_init (NULL);
    gdk_threads_init ();
  }
  else
  {
     printf("g_thread NOT supported\n");
  }
  /*necessary init for GTK*/
  gtk_init (&argc, &argv);

  // create two threads: one for the GUI and DATA respectively
  if( (GuiThread = g_thread_create((GThreadFunc)gui_thread,
				   (void *)NULL, TRUE, &err1)) == NULL)
  {
     printf("Thread create failed: %s!!\n", err1->message );
     g_error_free ( err1 ) ;
  }

  if( (DataThread = g_thread_create((GThreadFunc)data_thread,
                                    (void *)NULL, TRUE, &err2)) == NULL)
  {
     printf("Thread create failed: %s!!\n", err2->message );
     g_error_free ( err2 ) ;
  }
  g_thread_join(DataThread);
  g_thread_join(GuiThread);
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
/*Online Help*/
/*######################################*/
void show_help(GtkWidget *widget, gpointer window)
{
  
  GtkWidget *dialog = gtk_about_dialog_new();
  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "Help");
  //gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "0.1"); 
  //gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"(c) Osman Jacob,Alois Taranetz,Tibor Elias");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "For Online Help please visit:");
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),"http://www.technikum-wien.at/studium/bachelor/elektronik/aktuelles___termine/?full=1080");

  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
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
/*File New Dialog*/
/*######################################*/
void file_new(GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog, *table, *new_file_name;
    GtkWidget *lbl1;
    gint result;

	dialog = gtk_dialog_new_with_buttons ("Create New Log File", NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK, GTK_RESPONSE_OK,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,NULL);
    
    gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);
    /* Create the entrie for the new file nam,e*/
    lbl1 = gtk_label_new ("Enter Name for the new Log File:");
    new_file_name = gtk_entry_new();

/* Retrieve the user's information for the default values. */
//gtk_entry_set_text (GTK_ENTRY (new_file_name), g_get_new_file_name_name());

table = gtk_table_new (1, 2, FALSE);
gtk_table_attach_defaults (GTK_TABLE(table),lbl1,0,1,0,1);
gtk_table_attach_defaults (GTK_TABLE(table),new_file_name,1,2,0,1);
gtk_table_set_row_spacings (GTK_TABLE (table), 5);
gtk_table_set_col_spacings (GTK_TABLE (table), 5);
gtk_container_set_border_width (GTK_CONTAINER (table), 5);
gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);
gtk_widget_show_all (dialog);
/* Run the dialog and output the data if the user clicks the OK button. */
result = gtk_dialog_run (GTK_DIALOG (dialog));
if (result == GTK_RESPONSE_OK)
{
g_print ("File Name: %s\n", gtk_entry_get_text (GTK_ENTRY (new_file_name)));
}
gtk_widget_destroy (dialog);
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
  gtk_file_filter_add_pattern (filter,"*.log");
  gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),filter);

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
static void file_save (GtkWidget *widget, GtkWindow *window)
{
   GtkWidget *dialog;
   gchar *filename;
   GtkFileFilter *filter;
   dialog = gtk_file_chooser_dialog_new ("Save File", window,
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                         GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                         NULL);
                                         
   gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "Recorded_Data.log");
   gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
   /*Set File Filter for File Open Dialog*/
  filter = gtk_file_filter_new ();
  gtk_file_filter_set_name(filter,"log");
  gtk_file_filter_add_pattern (filter,"*.log");
  gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),filter);
   gint result = gtk_dialog_run (GTK_DIALOG (dialog));
   if (result == GTK_RESPONSE_ACCEPT)
   {
     filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
     g_print ("Saved File As: %s\n", filename);
   }
   gtk_widget_destroy (dialog);
}
/*######################################*/
/*File Save As Dialog*/
/*######################################*/
static void file_save_as (GtkWidget *widget, GtkWindow *window)
{
   GtkWidget *dialog;
   gchar *filename;
   GtkFileFilter *filter;
   dialog = gtk_file_chooser_dialog_new ("Save File As ...", window,
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                         GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                         NULL);
   gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
   /*Set File Filter for File Open Dialog*/
  filter = gtk_file_filter_new ();
  gtk_file_filter_set_name(filter,"log");
  gtk_file_filter_add_pattern (filter,"*.log");
  gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),filter);
   gint result = gtk_dialog_run (GTK_DIALOG (dialog));
   if (result == GTK_RESPONSE_ACCEPT)
   {
     filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
     g_print ("Saved File As: %s\n", filename);
   }
   gtk_widget_destroy (dialog);
}
/*######################################*/
/*Export Dialog*/
/*Build the Export Dialog with an file chooser dialog*/
/*######################################*/


#if DEBUG
//But in about dialog for debugging purposes
//Always get:(main:5520): GLib-GObject-CRITICAL **: g_signal_connect_data: assertion `c_handler != NULL' failed
//When calling this function
//the problem must be something with the gsignal conect
void export_diag(GtkWidget *widget, gpointer data)
{
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("about_symbol.png", NULL);
  GtkWidget *dialog = gtk_about_dialog_new();
  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "eStick Control Center ecc");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "0.1"); 
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"(c) Osman Jacob,Alois Taranetz,Tibor Elias");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "A Programm to record and playback data from the eStick");
  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
  g_object_unref(pixbuf), pixbuf = NULL;
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}
#endif
/*###########################################################################*/
/*Preferences Dialog Modal Version*/
/*Modal means when this dialog is active the main window is locked/ inactive*/
/*##########################################################################*/
#if debug
/* Create a new GtkDialog that will tell the user that the button was clicked. */
static void preferences (GtkWidget *widget, GtkWindow *window)
{
GtkWidget *dialog, *label, *image, *hbox;
/* Create a new dialog with one OK button. */
dialog = gtk_dialog_new_with_buttons ("Information", window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_STOCK_OK, GTK_RESPONSE_OK,NULL);
gtk_dialog_set_has_separator (GTK_DIALOG (dialog), FALSE);
label = gtk_label_new ("The button was clicked!");
image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO,
GTK_ICON_SIZE_DIALOG);
hbox = gtk_hbox_new (FALSE, 5);
gtk_container_set_border_width (GTK_CONTAINER (hbox), 0);
gtk_box_pack_start_defaults (GTK_BOX (hbox), image);
gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
/* Pack the dialog content into the dialog's GtkVBox. */
gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox);
gtk_widget_show_all (dialog);
/* Create the dialog as modal and destroy it when a button is clicked. */
gtk_dialog_run (GTK_DIALOG (dialog));
gtk_widget_destroy (dialog);
}
#endif
/*######################################*/
/*Preferences Dialog Nonmodal Version*/
/*######################################*/
static void preferences (GtkWidget *widget, GtkWindow *window)
{
GtkWidget *dialog, *label,*label1, *image, *hbox, *table;
GtkWidget *check1, *check2, *check3;
GtkWidget *check_source1, *check_source2, *check_source3, *check_source4;
/* Create a nonmodal dialog with one OK button. */
dialog = gtk_dialog_new_with_buttons ("Preferences", window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_STOCK_OK, GTK_RESPONSE_OK,NULL);
gtk_dialog_set_has_separator (GTK_DIALOG (dialog), FALSE);
label = gtk_label_new ("Chose the Port you want to watch:");
label1 = gtk_label_new ("Chose the Source you want to watch:");

hbox = gtk_hbox_new (FALSE, 5);
gtk_container_set_border_width (GTK_CONTAINER (hbox), 0);

table = gtk_table_new(2, 5, FALSE);
gtk_table_set_row_spacings(GTK_TABLE(table), 0);
gtk_table_set_col_spacings(GTK_TABLE(table), 0);
/*Create the checkboxes*/
check1 = gtk_check_button_new_with_label("Port B");
check2 = gtk_check_button_new_with_label("Port C");
check3 = gtk_check_button_new_with_label("Port D");
check_source1 = gtk_check_button_new_with_label("R1");
check_source2 = gtk_check_button_new_with_label("R2");
check_source3 = gtk_check_button_new_with_label("R3");
check_source4 = gtk_check_button_new_with_label("R4");
/*Default States for the Checkboxes*/
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check1), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check2), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check3), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_source1), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_source2), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_source3), FALSE);
gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_source4), FALSE);
/*Bindings for the callbackfunctions of the checkboxes*/
g_signal_connect(check1, "clicked",G_CALLBACK(pref_check1_cb), (gpointer) window);
g_signal_connect(check2, "clicked",G_CALLBACK(pref_check2_cb), (gpointer) window);
g_signal_connect(check3, "clicked",G_CALLBACK(pref_check3_cb), (gpointer) window);
g_signal_connect(check_source1, "clicked",G_CALLBACK(pref_check_source1_cb), (gpointer) window);
g_signal_connect(check_source2, "clicked",G_CALLBACK(pref_check_source2_cb), (gpointer) window);
g_signal_connect(check_source3, "clicked",G_CALLBACK(pref_check_source3_cb), (gpointer) window);
g_signal_connect(check_source4, "clicked",G_CALLBACK(pref_check_source4_cb), (gpointer) window);
/*Remove the focus frame from the checkboxes*/
GTK_WIDGET_UNSET_FLAGS(check1, GTK_CAN_FOCUS);
GTK_WIDGET_UNSET_FLAGS(check2, GTK_CAN_FOCUS);
GTK_WIDGET_UNSET_FLAGS(check3, GTK_CAN_FOCUS);
GTK_WIDGET_UNSET_FLAGS(check_source1, GTK_CAN_FOCUS);
GTK_WIDGET_UNSET_FLAGS(check_source2, GTK_CAN_FOCUS);
GTK_WIDGET_UNSET_FLAGS(check_source3, GTK_CAN_FOCUS);
GTK_WIDGET_UNSET_FLAGS(check_source4, GTK_CAN_FOCUS);  
/*Add Elements to the Dialog*/
/*First Row*/
gtk_table_attach_defaults(GTK_TABLE(table), label,  0, 1, 0, 1 );
gtk_table_attach_defaults(GTK_TABLE(table), check1, 2, 3, 0, 1 );
gtk_table_attach_defaults(GTK_TABLE(table), check2, 3, 4, 0, 1 );
gtk_table_attach_defaults(GTK_TABLE(table), check3, 4, 5, 0, 1 );
/*Second Row*/
gtk_table_attach_defaults(GTK_TABLE(table), label1,        0, 1, 1, 2 );
gtk_table_attach_defaults(GTK_TABLE(table), check_source1, 1, 2, 1, 2 );
gtk_table_attach_defaults(GTK_TABLE(table), check_source2, 2, 3, 1, 2 );
gtk_table_attach_defaults(GTK_TABLE(table), check_source3, 3, 4, 1, 2 );
gtk_table_attach_defaults(GTK_TABLE(table), check_source4, 4, 5, 1, 2 );

gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (dialog)->vbox), table);
gtk_widget_show_all (dialog);
/* Call gtk_widget_destroy() when the dialog emits the response signal. */
g_signal_connect (G_OBJECT (dialog), "response",
G_CALLBACK (gtk_widget_destroy), NULL);

}
/*########## Callback functions for the preferences Dialog ############*/
void pref_check1_cb(GtkWidget *widget, gpointer window)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
       //gtk_label_set_text(label_graph,*graph_name);	
       graph_name="graph1";
  } else {
      
  }
}

void pref_check2_cb(GtkWidget *widget, gpointer window)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
      gtk_window_set_title(window, "GtkCheckButton");	
  } else {
      gtk_window_set_title(window, "");
  }
}

void pref_check3_cb(GtkWidget *widget, gpointer window)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
      gtk_window_set_title(window, "GtkCheckButton");	
  } else {
      gtk_window_set_title(window, "");
  }
}

void pref_check_source1_cb(GtkWidget *widget, gpointer window)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
      gtk_window_set_title(window, "GtkCheckButton");	
  } else {
      gtk_window_set_title(window, "");
  }
}

void pref_check_source2_cb(GtkWidget *widget, gpointer window)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
      gtk_window_set_title(window, "GtkCheckButton");	
  } else {
      gtk_window_set_title(window, "");
  }
}
void pref_check_source3_cb(GtkWidget *widget, gpointer window)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
      gtk_window_set_title(window, "GtkCheckButton");	
  } else {
      gtk_window_set_title(window, "");
  }
}
void pref_check_source4_cb(GtkWidget *widget, gpointer window)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
      gtk_window_set_title(window, "GtkCheckButton");	
  } else {
      gtk_window_set_title(window, "");
  }
}
/*##############################################################*/
 /* draw a grid using "GRID intervals" both in x and y direction*/
 /*##############################################################*/
gboolean draw_grid (cairo_t * cr)
{
  double width, height;
  int i;
  static const double dashed1[] = {3.0, 2.0};
  static int len1  = sizeof(dashed1) / sizeof(dashed1[0]);

  height = ysize;
  width = xsize;

  for (i = 0; i<=GRID; i++) {
    // we use the default color black
    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
    // use solid lines on the outside and dashes inside
    if (i!=0 && i!=GRID)
    {
      cairo_set_line_width (cr, 0.1);
      cairo_set_dash(cr, dashed1, len1, 0);
    }
    else
    {
      cairo_set_line_width (cr, 1.5);
      cairo_set_dash(cr, dashed1, 0, 0);
    }
    //draw y lines
    cairo_move_to (cr, i*width/GRID+xoff/2, yoff/2);
    cairo_line_to (cr, i*width/GRID+xoff/2, height+yoff/2);
    cairo_stroke (cr);

    //draw x lines
    cairo_move_to (cr, xoff/2,i*height/GRID+yoff/2);
    cairo_line_to (cr, width+xoff/2,i*height/GRID+yoff/2);
    cairo_stroke (cr);
  }
  return FALSE;
}
/*##############################################################*/
/*Redraw the graph when the size changes */
/*##############################################################*/
   static gboolean on_expose_event(GtkWidget *widget,
   GdkEventExpose *event,
   gpointer data)
  {
  xsize = widget->allocation.width-xoff;
  ysize = widget->allocation.height-yoff;
  draw_diagram(widget);
  return TRUE;
  }
/*##############################################################*/
 /* perform the actual drawing*/
/*##############################################################*/
gboolean draw_diagram (GtkWidget *widget)
{
  double width, height;
  int i;
  cairo_t *cr;

  width = xsize;
  height = ysize;
  // create a Cairo context for drawing
  cr = gdk_cairo_create (widget->window);
  // draw the grid
  draw_grid (cr);
  // set the line width
  cairo_set_line_width (cr, 0.75);

/*This grap is only for testing purposes
  // draw rthe random values from valuea[]
  // set the drawing color
  cairo_set_source_rgb (cr, 0.0, 0.5, 0.5);
  for (i = 0; i < 10*GRID; i++) {
    // move to the (x= x-axis / y= current data value)
    cairo_move_to (cr, i * (width/100)+xoff/2,ysize-valuea[i]+yoff/2);
    // draw a line to the next x-axis position and the next data value
    cairo_line_to (cr,(i+1)*(width/100)+xoff/2,ysize-valuea[i+1]+yoff/2);
    // draw
    cairo_stroke (cr);
  }*/

  // draw a second graph with data stored in valueb[]
  // set the drawing color
  cairo_set_source_rgb (cr, 0.5, 0.0, 0.5);
  for (i = 0; i < 10*GRID; i++) {
    cairo_move_to (cr, i * (width/100)+xoff/2,ysize-valueb[i]+yoff/2);
    cairo_line_to (cr,(i+1)*(width/100)+xoff/2,ysize-valueb[i+1]+yoff/2);
    cairo_stroke (cr);
  }

  cairo_destroy (cr);
  return FALSE;
}
/*################################################################*/
 /* Error Dialogs for the eStick not dirctly triggered by the GUI*/
/*###############################################################*/
/*EStick Not Connected*/
void error_ext1(GtkWidget *widget, gpointer window)
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(window,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Error EStick Not Connected");
  gtk_window_set_title(GTK_WINDOW(dialog), "Error");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

/*EStick Connection Error*/
void error_ext2(GtkWidget *widget, gpointer window)
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(window,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Error EStick Not Connected");
  gtk_window_set_title(GTK_WINDOW(dialog), "Error");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
/*Exception*/
void error_ext3(GtkWidget *widget, gpointer window)
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(window,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Exception Error");
  gtk_window_set_title(GTK_WINDOW(dialog), "Error");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
/*Not in use*/
void error_ext4(GtkWidget *widget, gpointer window)
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(window,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "<fill in Message>");
  gtk_window_set_title(GTK_WINDOW(dialog), "Error");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}


/*Not in use*/
void error_ext5(GtkWidget *widget, gpointer window)
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(window,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "<fill in Message>");
  gtk_window_set_title(GTK_WINDOW(dialog), "Error");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
/*! EOF */
