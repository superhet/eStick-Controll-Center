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
