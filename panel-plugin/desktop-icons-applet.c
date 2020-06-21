/*
* Copyright © 2020 misko_2083
*
* Distributed under terms of the GPL2 license.
* 
* gcc -Wall -s -shared -fPIC -g desktop-icons-applet.c -o desktop-icons-applet $(pkg-config --libs --cflags gtk+-3.0 libxfce4panel-2.0 libxfconf-0)
* mv libdicons.so /usr/lib/x86_64-linux-gnu/xfce4/panel/plugins/libdicons.so
*/

#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>
#include <xfconf/xfconf.h>


#define DEFAULT_ICON_NAME "emblem-desktop"
#define DEFAULT_TOOLTIP_MESSAGE "Show/Hide Desktop Icons"
#define DEFAULT_TITLE "dicons"
#define XFCE_PLUGIN_VERSION "0.1"

typedef struct _DiconsPlugin {
	XfcePanelPlugin *plugin;
	GtkWidget *button;
	GtkWidget *icon;
	GtkWidget *window;
	gchar *icon_name;
} DiconsPlugin;

static void
button_clicked (GtkWidget *button,
                 DiconsPlugin    *dicons);
static gboolean
on_popup_focus_out (GtkWidget *widget,
                    GdkEventFocus *event,
                    gpointer data);
                    
static gboolean
on_key_pressed (GtkWidget *widget,
                GdkEventKey *event,
                gpointer data);

static const char dicons_plugin_copyright[] =
"Copyright \xc2\xa9 2020 Miloš Pavlović\n";

static void dicons_about(XfcePanelPlugin *plugin)
{
	const gchar *auth[] = { "Miloš Pavlović", NULL };
	GdkPixbuf *icon;

	icon = xfce_panel_pixbuf_from_source("emblem-desktop", NULL, 32);
	gtk_show_about_dialog(NULL,
			      "logo", icon,
			      "license", xfce_get_license_text(XFCE_LICENSE_TEXT_GPL),
			      "version", XFCE_PLUGIN_VERSION,
			      "program-name", "dicons-applet",
			      "comments", _("Opens a configuration menu for desktop icons"),
			      "website", "https://github.com/Misko-2083",
			      "copyright", _(dicons_plugin_copyright),
			      "authors", auth,
			      NULL);
	if (icon)
		g_object_unref(G_OBJECT(icon));
}

static void
_quit_cb (GtkWidget *button, GtkWidget *window, gpointer data)
{
    (void)data; /* Avoid compiler warnings */
    
    gtk_widget_hide (window);
    
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
    
    return;
}

static gboolean
on_popup_focus_out (GtkWidget *widget,
                    GdkEventFocus *event,
                    gpointer data)
{
  gtk_widget_hide (widget);
  
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), FALSE);
  
  return TRUE;
}

static gboolean
on_key_pressed    (GtkWidget *widget,
                   GdkEventKey *event,
                   gpointer data)
{
    if (event->keyval == GDK_KEY_Escape){
        gtk_widget_hide (widget);
        
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data), FALSE);
        
        return TRUE;
    }
    return FALSE;
}

static gboolean
on_switch_home       (GtkWidget *widget,
                   gboolean *state,
                   gpointer user_data)
{
    XfconfChannel    *channel;

    xfconf_init(NULL);
    
    channel = xfconf_channel_get("xfce4-desktop");
    
    if (state)
        xfconf_channel_set_bool(channel, "/desktop-icons/file-icons/show-home", TRUE);
    else
        xfconf_channel_set_bool(channel, "/desktop-icons/file-icons/show-home", FALSE);
        
    xfconf_shutdown();
    
    return FALSE;
}

static gboolean
on_switch_trash       (GtkWidget *widget,
                   gboolean *state,
                   gpointer user_data)
{
	XfconfChannel    *channel;

	xfconf_init(NULL);
    
    channel = xfconf_channel_get("xfce4-desktop");
    
    if (state)
        xfconf_channel_set_bool(channel, "/desktop-icons/file-icons/show-trash", TRUE);
    else
        xfconf_channel_set_bool(channel, "/desktop-icons/file-icons/show-trash", FALSE);
        
    xfconf_shutdown();
        
    return FALSE;
}

static gboolean
on_switch_filesystem       (GtkWidget *widget,
                   gboolean *state,
                   gpointer user_data)
{
	XfconfChannel    *channel;

	xfconf_init(NULL);
    
    channel = xfconf_channel_get("xfce4-desktop");
    
    if (state)
        xfconf_channel_set_bool(channel, "/desktop-icons/file-icons/show-filesystem", TRUE);
    else
        xfconf_channel_set_bool(channel, "/desktop-icons/file-icons/show-filesystem", FALSE);
        
    xfconf_shutdown();
        
    return FALSE;
}

static gboolean
on_switch_removable       (GtkWidget *widget,
                   gboolean *state,
                   gpointer user_data)
{
	XfconfChannel    *channel;

	xfconf_init(NULL);
    
    channel = xfconf_channel_get("xfce4-desktop");
    
    if (state)
        xfconf_channel_set_bool(channel, "/desktop-icons/file-icons/show-removable", TRUE);
    else
        xfconf_channel_set_bool(channel, "/desktop-icons/file-icons/show-removable", FALSE);
        
    xfconf_shutdown();
        
    return FALSE;
}

static void button_clicked(GtkWidget *button,
                           DiconsPlugin    *dicons)
{
    gint  x, y;

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(dicons->button)))
  {
	  
	   xfce_panel_plugin_block_autohide(dicons->plugin, TRUE);

       if (GTK_IS_TOGGLE_BUTTON (button)) {
		     
	       xfce_panel_plugin_position_widget(XFCE_PANEL_PLUGIN (dicons->plugin),
                                             GTK_WIDGET(dicons->window),
                                             button, &x, &y);
	   } else {
		   
		   GdkDisplay *display = gdk_display_get_default();
		   
		   GdkSeat *seat = gdk_display_get_default_seat(display);
		   
		   GdkDevice *device = gdk_seat_get_pointer(seat);
		   
		   gdk_window_get_device_position(gdk_get_default_root_window(),
		                                  device, &x, &y, NULL);
       }

	   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dicons->button), TRUE);
	   
       if (!gtk_widget_get_mapped(dicons->window))
           gtk_widget_show_all(GTK_WIDGET(dicons->window));

       gtk_window_move (GTK_WINDOW(dicons->window), x, y);
       /* fix me: this function is called twice */
  } else {
	  
	   _quit_cb(dicons->button, dicons->window, NULL);
	   if (GTK_IS_TOGGLE_BUTTON (button))
	       gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);
	   
       xfce_panel_plugin_block_autohide(dicons->plugin, FALSE);
  }
}

static DiconsPlugin *dicons_init(XfcePanelPlugin *plugin)
{
    GtkWidget        *box;
    GtkWidget        *box_a;
    GtkWidget        *box_b;
    GtkWidget        *boxl;
    GtkWidget        *about_button;
    GtkWidget        *cancel_button;
    GtkWidget        *question;
    GtkWidget        *label_home;
    GtkWidget        *label_trash;
    GtkWidget        *label_filesystem;
    GtkWidget        *label_removable;
    GtkWidget        *image;
    GtkWidget        *switch_home;
    GtkWidget        *switch_trash;
    GtkWidget        *switch_filesystem;
    GtkWidget        *switch_removable;
    GdkWindow        *pwindow;
    XfconfChannel    *channel;
    
    	DiconsPlugin *dicons = g_slice_new0(DiconsPlugin);

	dicons->plugin = plugin;
	dicons->icon_name = g_strdup(DEFAULT_ICON_NAME);
	dicons->button = xfce_panel_create_toggle_button();
	xfce_panel_plugin_add_action_widget (XFCE_PANEL_PLUGIN (dicons->plugin),
	                                     dicons->button);
    
    gtk_init(NULL, NULL);

    dicons->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_type_hint (GTK_WINDOW(dicons->window),
                              GDK_WINDOW_TYPE_HINT_POPUP_MENU);
    gtk_widget_set_size_request(dicons->window, 250, 220);
    gtk_window_set_resizable (GTK_WINDOW(dicons->window), TRUE);
    gtk_window_set_keep_above (GTK_WINDOW (dicons->window), TRUE);
    gtk_window_stick (GTK_WINDOW (dicons->window));
    gtk_window_set_skip_taskbar_hint (GTK_WINDOW (dicons->window), TRUE);
    gtk_window_set_title (GTK_WINDOW (dicons->window), "Desktop Icons");
    gtk_widget_set_events (dicons->window, GDK_FOCUS_CHANGE_MASK);
    g_signal_connect (G_OBJECT (GTK_WINDOW (dicons->window)),
                    "focus-out-event",
                    G_CALLBACK (on_popup_focus_out),
                    dicons->button);
    gtk_widget_set_events (GTK_WIDGET(dicons->window), GDK_KEY_PRESS_MASK);
    g_signal_connect (G_OBJECT (GTK_WINDOW (dicons->window)),
                    "key-press-event",
                    G_CALLBACK (on_key_pressed),
                    dicons->button);
    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    box_a = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    box_b = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    boxl = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);

    gtk_box_set_homogeneous (GTK_BOX (box), TRUE);
    gtk_container_add(GTK_CONTAINER(dicons->window),boxl);
    image = gtk_image_new_from_icon_name (dicons->icon_name,
                                          GTK_ICON_SIZE_DND);
    question = gtk_label_new ("\nDesktop Icons");

    label_home = gtk_label_new ("Home");
    label_trash = gtk_label_new ("Trash");
    label_filesystem = gtk_label_new ("Filesystem");
    label_removable = gtk_label_new ("Removable");
    switch_home = gtk_switch_new ();
    switch_trash = gtk_switch_new ();
    switch_filesystem = gtk_switch_new ();
    switch_removable = gtk_switch_new ();
    
    g_signal_connect (G_OBJECT (switch_home),
                     "state-set",
                      G_CALLBACK (on_switch_home),
                      NULL);
    g_signal_connect (G_OBJECT (switch_trash),
                     "state-set",
                      G_CALLBACK (on_switch_trash),
                      NULL);
    g_signal_connect (G_OBJECT (switch_filesystem),
                     "state-set",
                      G_CALLBACK (on_switch_filesystem),
                      NULL);
    g_signal_connect (G_OBJECT (switch_removable),
                     "state-set",
                      G_CALLBACK (on_switch_removable),
                      NULL);
                      
    /*  set initial switches */
    xfconf_init(NULL);
    
    channel = xfconf_channel_get("xfce4-desktop");
    
    if (xfconf_channel_get_bool(channel, "/desktop-icons/file-icons/show-home", TRUE))
        gtk_switch_set_state (GTK_SWITCH(switch_home), TRUE);
    else
        gtk_switch_set_state (GTK_SWITCH(switch_home), FALSE);
        
    if (xfconf_channel_get_bool(channel, "/desktop-icons/file-icons/show-trash", TRUE))
        gtk_switch_set_state (GTK_SWITCH(switch_trash), TRUE);
    else
        gtk_switch_set_state (GTK_SWITCH(switch_trash), FALSE);
        
    if (xfconf_channel_get_bool(channel, "/desktop-icons/file-icons/show-filesystem", TRUE))
        gtk_switch_set_state (GTK_SWITCH(switch_filesystem), TRUE);
    else
        gtk_switch_set_state (GTK_SWITCH(switch_filesystem), FALSE);

    if (xfconf_channel_get_bool(channel, "/desktop-icons/file-icons/show-removable", TRUE))
        gtk_switch_set_state (GTK_SWITCH(switch_removable), TRUE);
    else
        gtk_switch_set_state (GTK_SWITCH(switch_removable), FALSE);

    xfconf_shutdown();

    gtk_box_pack_start (GTK_BOX(box_a), label_home, TRUE, TRUE, 10);
    gtk_box_pack_start (GTK_BOX(box_a), switch_home, FALSE, TRUE, 10);
    gtk_box_pack_start (GTK_BOX(box_a), label_trash, TRUE, TRUE, 10);
    gtk_box_pack_start (GTK_BOX(box_a), switch_trash, FALSE, TRUE, 10);
    gtk_box_pack_start (GTK_BOX(box_b), label_filesystem, TRUE, TRUE, 10);
    gtk_box_pack_start (GTK_BOX(box_b), switch_filesystem, FALSE, TRUE, 10);
    gtk_box_pack_start (GTK_BOX(box_b), label_removable, TRUE, TRUE, 10);
    gtk_box_pack_start (GTK_BOX(box_b), switch_removable, FALSE, TRUE, 10);

    about_button = gtk_button_new_with_label("About");
    cancel_button = gtk_button_new_with_label("Cancel");
    
    g_signal_connect(G_OBJECT(cancel_button),
                     "clicked",
                     G_CALLBACK(_quit_cb),
                     dicons->window);
    g_signal_connect(G_OBJECT(about_button),
                     "clicked",
                     G_CALLBACK(dicons_about),
                     plugin);

    gtk_box_pack_start (GTK_BOX(boxl), image, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(boxl), question, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX(boxl), box_a, FALSE, TRUE, 5);
    gtk_box_pack_start (GTK_BOX(boxl), box_b, FALSE, TRUE, 5);
    gtk_box_pack_start (GTK_BOX(box), about_button, TRUE, TRUE, 10);
    gtk_box_pack_start (GTK_BOX (box), cancel_button, TRUE, TRUE, 10);
    gtk_box_pack_end (GTK_BOX(boxl), box, TRUE, TRUE, 10);
    gtk_window_set_decorated (GTK_WINDOW(dicons->window), FALSE);
    gtk_widget_show(dicons->button);
    
    gtk_window_set_attached_to(GTK_WINDOW(dicons->window),
                               dicons->button);
    pwindow = gtk_widget_get_parent_window(dicons->button);
    
    gtk_window_set_transient_for(GTK_WINDOW(dicons->window),
                                 GTK_WINDOW(pwindow));

	g_signal_connect(G_OBJECT(dicons->button), "toggled",
			 G_CALLBACK(button_clicked), dicons);
	gtk_widget_set_tooltip_text (GTK_WIDGET(dicons->button),
				                 DEFAULT_TOOLTIP_MESSAGE);
	dicons->icon = xfce_panel_image_new_from_source(dicons->icon_name);
	gtk_widget_show(dicons->icon);
	gtk_container_add(GTK_CONTAINER(dicons->button), dicons->icon);
	
	return dicons;
}

static void dicons_free(XfcePanelPlugin *plugin, DiconsPlugin *dicons)
{
    gtk_widget_destroy(dicons->button);
    
    gtk_widget_destroy(dicons->icon);
    
    if (GTK_IS_WIDGET (dicons->window))
        gtk_widget_destroy(dicons->window);

    g_slice_free(DiconsPlugin, dicons);
}


static gboolean dicons_size_changed (XfcePanelPlugin *plugin,
                     gint             size,
                     DiconsPlugin    *dicons)
{
    size = size / xfce_panel_plugin_get_nrows(plugin);
    gtk_widget_set_size_request (GTK_WIDGET (plugin), size, size);
    return TRUE;
}


static void set_button_active (GtkToggleButton *button)
{
	 if (GTK_IS_TOGGLE_BUTTON(button)) {
	 
	     if (!gtk_toggle_button_get_active(button)) {
              gtk_toggle_button_set_active(button, TRUE);
             }
         else
             {
	          gtk_toggle_button_set_active(button, FALSE);
		     }
     }
}


static gboolean dicons_remote (XfcePanelPlugin *plugin,
                             gchar           *name,
                             GValue          *value,
                             DiconsPlugin    *dicons)
{
    g_return_val_if_fail (value == NULL || G_IS_VALUE (value), FALSE);
    
    if (strcmp (name, "popup") == 0
        && !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(dicons->button)) 
        &&  gtk_widget_get_visible (GTK_WIDGET (plugin)) )
        {
        if (value != NULL
            && G_VALUE_HOLDS_BOOLEAN (value)
            && g_value_get_boolean (value))
            {
			  set_button_active(GTK_TOGGLE_BUTTON(dicons->button));
                /* popup here at mouse pointer , where X is an internal id
                 * xfce4-panel --plugin-event=desktop-icons-applet-X:popup:bool:true
                 */
		      button_clicked(NULL, dicons);
            }
        else
            {	
				set_button_active(GTK_TOGGLE_BUTTON(dicons->button));
                /* popup here, where X is an internal id
                 * xfce4-panel --plugin-event=desktop-icons-applet-X:popup:bool:false
                 */
		      button_clicked(dicons->button, dicons);
            }
       return TRUE;

        }
    return FALSE;
}


static void dicons_construct(XfcePanelPlugin *plugin)
{
    DiconsPlugin *dicons;
    
    dicons = dicons_init(plugin);
    
    gtk_container_add(GTK_CONTAINER(plugin), dicons->button);
    
    xfce_panel_plugin_add_action_widget(plugin, dicons->button);
    
    xfce_panel_plugin_menu_show_about(plugin);
    
    g_signal_connect (G_OBJECT(plugin),
                     "free-data",
                     G_CALLBACK(dicons_free), dicons);
    g_signal_connect (G_OBJECT(plugin),
                     "size-changed",
                     G_CALLBACK(dicons_size_changed), dicons);
    g_signal_connect (G_OBJECT (plugin),
                     "remote-event",
                     G_CALLBACK(dicons_remote), dicons);
    g_signal_connect (G_OBJECT (plugin),
                     "about",
                    G_CALLBACK (dicons_about), dicons);
}
XFCE_PANEL_PLUGIN_REGISTER(dicons_construct);
