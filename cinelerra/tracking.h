#ifndef TRACKING_H
#define TRACKING_H

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "mwindow.inc"
#include "mwindowgui.inc"
#include "mutex.h"
#include "playbackengine.inc"
#include "thread.h"
#include "timer.h"

class Tracking : public Thread
{
public:
	Tracking(MWindow *mwindow);
	virtual ~Tracking();

	void create_objects();
	virtual int start_playback(double new_position);
	virtual int stop_playback();

// Called by the tracker to get the current position
	virtual PlaybackEngine* get_playback_engine();
	virtual double get_tracking_position();
// Update position displayed
	virtual void update_tracker(double position);
// Update meters
	virtual void update_meters(long position);
	virtual void stop_meters();
	int get_pixel(double position);

// Erase cursor if it's visible.  Called by start_playback
// Draw new cursor at last_position if invisible
	virtual void draw();



	void run();

// Values to return from playback_engine to update_meter .
// Use ArrayList to simplify module counting
	ArrayList<double> module_levels;
	int state;










	void show_playback_cursor(long position);
	int wait_for_startup();
	int view_follows_playback;
// Stop cursor loop when not playing
	Mutex pause_lock;
// Exclude changes in state during the main loop
	Mutex loop_lock;
// Exclude accesses to the cursor
	Mutex cursor_lock;
// Delay until startup
	Mutex startup_lock;
	MWindow *mwindow;
	MWindowGUI *gui;
	double last_position;
	int follow_loop;
	long current_offset;
	int reverse;
	int double_speed;
	Timer timer;
// Pixel of last drawn cursor
	int pixel;
// Cursor is visible
	int visible;
};

#endif