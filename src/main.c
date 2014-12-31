/*************************************************************************/
/* Copyright (C) 2014 matias <mati86dl@gmail.com>                        */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

#include <mate-panel-applet.h>
#include <libmpris2client/libmpris2client.h>
#include <libintl.h>
#include <string.h>

#include "../config.h"
#include "applet.h"

/*
 * Mpris2
 */
static void
mpris2_button_prev (GtkWidget    *widget,
                    Mpris2Client *mpris2)
{
	if (!mpris2_client_is_connected(mpris2))
		return;

	mpris2_client_prev (mpris2);
}

static void
mpris2_button_play_pause (GtkWidget    *widget,
                          Mpris2Client *mpris2)
{
	if (!mpris2_client_is_connected(mpris2))
		mpris2_client_auto_set_player (mpris2);

	mpris2_client_play_pause (mpris2);
}

static void
mpris2_button_stop (GtkWidget    *widget,
                    Mpris2Client *mpris2)
{
	if (!mpris2_client_is_connected(mpris2))
		return;

	mpris2_client_stop (mpris2);
}

static void
mpris2_button_next (GtkWidget    *widget,
                    Mpris2Client *mpris2)
{
	if (!mpris2_client_is_connected(mpris2))
		return;

	mpris2_client_next (mpris2);
}

static void
mpris2_connection (Mpris2Client            *mpris2,
                   gboolean                 connected,
                   MultimediaControlApplet *applet)
{
	gtk_widget_set_tooltip_text (GTK_WIDGET(applet->applet),
	                             connected ?
	                             mpris2_client_get_player_identity (mpris2) :
	                             "Click to detect the player");
}

static void
mpris2_playback_status (Mpris2Client            *mpris2,
                        PlaybackStatus           playback_status,
                        MultimediaControlApplet *applet)
{
	switch (playback_status) {
		case PLAYING:
			gtk_widget_set_sensitive (applet->prev_button, TRUE);
			gtk_button_set_label (GTK_BUTTON(applet->play_button),
			                      GTK_STOCK_MEDIA_PAUSE);
			gtk_widget_set_sensitive (applet->stop_button, TRUE);
			gtk_widget_set_sensitive (applet->next_button, TRUE);
			break;
		case PAUSED:
			gtk_button_set_label (GTK_BUTTON(applet->play_button),
			                      GTK_STOCK_MEDIA_PLAY);
			break;
		case STOPPED:
		default:
			gtk_widget_set_sensitive (applet->prev_button, FALSE);
			gtk_button_set_label (GTK_BUTTON(applet->play_button),
			                      GTK_STOCK_MEDIA_PLAY);
			gtk_widget_set_sensitive (applet->stop_button, FALSE);
			gtk_widget_set_sensitive (applet->next_button, FALSE);
			break;
	}
}

/*
 * Applet construction and signals.
 */

static void
applet_destroy (MatePanelApplet         *applet_widget,
                MultimediaControlApplet *applet)
{
	g_assert(applet);

	g_object_unref (applet->mpris2);

	g_free(applet);

	return;
}

static gboolean
multimedia_control_applet_factory (MatePanelApplet *applet_widget,
                                   const gchar     *iid,
                                   gpointer         data)
{
	MultimediaControlApplet *applet;
	GtkWidget *box, *button;

	if (strcmp (iid, APPLET_ID) != 0)
		return FALSE;

	applet = g_malloc0(sizeof(MultimediaControlApplet));

	applet->applet = applet_widget;

	/*
	 * Mpris2
	 */
	applet->mpris2 = mpris2_client_new ();
	g_signal_connect (G_OBJECT (applet->mpris2), "connection",
	                  G_CALLBACK(mpris2_connection), applet);
	g_signal_connect (G_OBJECT (applet->mpris2), "playback-status",
	                  G_CALLBACK(mpris2_playback_status), applet);

	/*
	 * Widgets
	 */
	box = gtk_hbox_new(FALSE, 0);

	button = GTK_WIDGET(gtk_button_new_from_stock(GTK_STOCK_MEDIA_PREVIOUS));
	gtk_button_set_relief (GTK_BUTTON(button), GTK_RELIEF_NONE);
	g_signal_connect (G_OBJECT(button), "clicked",
	                  G_CALLBACK(mpris2_button_prev), applet->mpris2);
	gtk_box_pack_start (GTK_BOX(box), GTK_WIDGET(button),
	                    FALSE, FALSE, 0);
	applet->prev_button = button;

	button = GTK_WIDGET(gtk_button_new_from_stock(GTK_STOCK_MEDIA_PLAY));
	gtk_button_set_relief (GTK_BUTTON(button), GTK_RELIEF_NONE);
	g_signal_connect (G_OBJECT(button), "clicked",
	                  G_CALLBACK(mpris2_button_play_pause), applet->mpris2);
	gtk_box_pack_start (GTK_BOX(box), GTK_WIDGET(button),
	                    FALSE, FALSE, 0);
	applet->play_button = button;

	button = GTK_WIDGET(gtk_button_new_from_stock(GTK_STOCK_MEDIA_STOP));
	gtk_button_set_relief (GTK_BUTTON(button), GTK_RELIEF_NONE);
	g_signal_connect (G_OBJECT(button), "clicked",
	                  G_CALLBACK(mpris2_button_stop), applet->mpris2);
	gtk_box_pack_start (GTK_BOX(box), GTK_WIDGET(button),
	                    FALSE, FALSE, 0);
	applet->stop_button = button;

	button = GTK_WIDGET(gtk_button_new_from_stock(GTK_STOCK_MEDIA_NEXT));
	gtk_button_set_relief (GTK_BUTTON(button), GTK_RELIEF_NONE);
	g_signal_connect (G_OBJECT(button), "clicked",
	                  G_CALLBACK(mpris2_button_next), applet->mpris2);
	gtk_box_pack_start (GTK_BOX(box), GTK_WIDGET(button),
	                    FALSE, FALSE, 0);
	applet->next_button = button;

	g_signal_connect (G_OBJECT(applet_widget), "destroy",
	                  G_CALLBACK(applet_destroy), (gpointer)applet);

	gtk_container_add(GTK_CONTAINER(applet_widget), box);

	gtk_widget_show_all(GTK_WIDGET(applet_widget));

	/*
	 * Check if a player is running and upate.
	 */
	mpris2_client_auto_set_player (applet->mpris2);

	mpris2_connection (applet->mpris2,
	                   mpris2_client_is_connected(applet->mpris2),
	                   applet);
	mpris2_playback_status (applet->mpris2,
	                        mpris2_client_get_playback_status(applet->mpris2),
	                        applet);

	return TRUE;
}

MATE_PANEL_APPLET_OUT_PROCESS_FACTORY (APPLET_FACTORY, PANEL_TYPE_APPLET, APPLET_NAME, multimedia_control_applet_factory, NULL)
