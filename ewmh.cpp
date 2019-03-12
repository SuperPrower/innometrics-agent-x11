#include "ewmh.h"

#include <QtDebug>

#include <xcb/xcb_event.h>

EWMH_Client::EWMH_Client(QObject *parent)
	: QThread(parent)
{
	this->xcb_conn = xcb_connect(NULL, &this->screen_nbr);
	this->ewmh_conn = (xcb_ewmh_connection_t*) malloc(sizeof(xcb_ewmh_connection_t));

	xcb_intern_atom_cookie_t* ewmh_cookie =
		xcb_ewmh_init_atoms(this->xcb_conn, this->ewmh_conn);

	if (xcb_ewmh_init_atoms_replies(ewmh_conn, ewmh_cookie, NULL) == 0) {
		qFatal("Could not initialize EWMH connection \n");
	}

	// TODO: check this with multi-monitor setup
	// qInfo() << this->screen_nbr;

	const xcb_setup_t *setup = xcb_get_setup(xcb_conn);
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

	screen = iter.data;

	/*
	// Number of monitors may be confusing, because there is things like
	// Xinerama or RandR.
	// Iterate over all screens and fetch data about them all
	while (iter.data != xcb_screen_end(iter).data) {
		xcb_screen_t *screen = iter.data;
		qInfo() << "Informations of screen " << screen->root << endl
			<< "  width.........:" << screen->width_in_pixels << endl
			<< "  height........:" << screen->height_in_pixels << endl
			<< "  white pixel...:" << screen->white_pixel << endl
			<< "  black pixel...:" << screen->black_pixel << endl;


		xcb_screen_next(&iter);
	}
	*/


}

EWMH_Client::~EWMH_Client() {
	xcb_ewmh_connection_wipe(ewmh_conn);
	free(ewmh_conn);
	xcb_disconnect(xcb_conn);

}

void EWMH_Client::run() {
	xcb_generic_event_t* ev;

	uint32_t event_mask[] = { XCB_EVENT_MASK_PROPERTY_CHANGE };
	xcb_change_window_attributes(xcb_conn, screen->root, XCB_CW_EVENT_MASK, &event_mask);

	xcb_flush(xcb_conn);
	while (true) {
		ev = xcb_wait_for_event(xcb_conn);

		if (ev == NULL) {
			fprintf(stderr, "ERROR while polling the event\n");
		}

		if (XCB_EVENT_RESPONSE_TYPE(ev) == XCB_PROPERTY_NOTIFY) {
			xcb_generic_error_t *error = NULL;

			auto pnev = (xcb_property_notify_event_t *) ev;
			if (pnev->atom == ewmh_conn->_NET_ACTIVE_WINDOW) {
				// get new active window
				unsigned int win = XCB_NONE;
				xcb_ewmh_get_active_window_reply(
					ewmh_conn,
					xcb_ewmh_get_active_window(ewmh_conn, screen_nbr),
					&win, &error
				);

				if (error) {
					fprintf(stderr, "ERROR: can't get active window: %d\n", error->error_code);
					// TODO: quit
				}

				QString wm_class = getWmClass(win);

				QString title = getWmName(win);
				if (title.isEmpty()) title = getVisibleName(win);
				if (title.isEmpty()) title = getIcccmWmName(win);

				emit this->newActiveWindow(wm_class, title);

			}
		}

		// cleanup event
		free(ev);
	}
}


QString EWMH_Client::getWmClass(unsigned int win){
	QString wmClass = "";
	xcb_icccm_get_wm_class_reply_t reply{};

	if (xcb_icccm_get_wm_class_reply(xcb_conn, xcb_icccm_get_wm_class(xcb_conn, win), &reply, NULL)) {
		wmClass = QString::fromUtf8(reply.class_name);
		// there is a low possibility that Class will be empty but Instance is not, but I'm still gonna check
		if (wmClass.isEmpty()) wmClass = QString::fromUtf8(reply.instance_name);
	}

	xcb_icccm_get_wm_class_reply_wipe(&reply);

	return wmClass;
}

QString EWMH_Client::getWmName(unsigned int win){
	xcb_ewmh_get_utf8_strings_reply_t utf8_reply;
	if (xcb_ewmh_get_wm_name_reply(ewmh_conn, xcb_ewmh_get_wm_name(ewmh_conn, win), &utf8_reply, NULL)) {
		return getEwmhReplyString(&utf8_reply);
	}
	return "";
}

QString EWMH_Client::getVisibleName(unsigned int win){
	xcb_ewmh_get_utf8_strings_reply_t utf8_reply;
	if (xcb_ewmh_get_wm_visible_name_reply(ewmh_conn, xcb_ewmh_get_wm_visible_name(ewmh_conn, win), &utf8_reply, NULL)) {
		return getEwmhReplyString(&utf8_reply);
	}
	return "";
}

QString EWMH_Client::getIcccmWmName(unsigned int win){
	xcb_icccm_get_text_property_reply_t icccm_reply;
	if (xcb_icccm_get_wm_name_reply(xcb_conn, xcb_icccm_get_wm_name(xcb_conn, win), &icccm_reply, nullptr)) {
		return getIcccmReplyString(&icccm_reply);
	}
	return "";

}

QString EWMH_Client::getEwmhReplyString (xcb_ewmh_get_utf8_strings_reply_t* reply) {
	QString title;
	QString str("");
	if (reply) {
		str = QString::fromUtf8(reply->strings, reply->strings_len);
		xcb_ewmh_get_utf8_strings_reply_wipe(reply);
	}

	return str;
};

QString EWMH_Client::getIcccmReplyString (xcb_icccm_get_text_property_reply_t* reply) {
	QString str("");
	if (reply) {
		str = QString::fromUtf8(reply->name, reply->name_len);
		xcb_icccm_get_text_property_reply_wipe(reply);
	}

	return str;
};
