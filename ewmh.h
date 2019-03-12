#ifndef EWMH_H
#define EWMH_H

#include <QThread>

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>

/* Describes a class that listens to EWMH events and emits signal when active window changes */

class EWMH_Client : public QThread {
	Q_OBJECT;
public:
	EWMH_Client(QObject *parent = nullptr);
	~EWMH_Client();

	void run();

private:
	int 			screen_nbr;
	xcb_connection_t	*xcb_conn;
	xcb_ewmh_connection_t	*ewmh_conn;

	xcb_screen_t		*screen;

	QString getWmClass(unsigned int win);
	QString getWmName(unsigned int win);
	QString getVisibleName(unsigned int win);
	QString getIcccmWmName(unsigned int win);

	// Helper functions to get string from answer
	QString getEwmhReplyString (xcb_ewmh_get_utf8_strings_reply_t* reply);
	QString getIcccmReplyString (xcb_icccm_get_text_property_reply_t* reply);


signals:
	void newActiveWindow(QString className, QString windowName);
};

#endif /* EWMH_H */
