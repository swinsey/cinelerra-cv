#ifndef COMMONRENDER_H
#define COMMONRENDER_H

#include "cache.inc"
#include "virtualconsole.inc"
#include "module.inc"
#include "mutex.h"
#include "mwindow.inc"
#include "renderengine.inc"
#include "thread.h"
#include "track.inc"

class CommonRender : public Thread
{
public:
	CommonRender(RenderEngine *renderengine);
	virtual ~CommonRender();

	virtual void arm_command();
	virtual int get_total_tracks() { return 0; };
	virtual Module* new_module(Track *track) { return 0; };
	void delete_vconsole();
	void create_modules();
	void reset_parameters();
// Build the virtual console at the current position
	virtual void build_virtual_console();
	virtual VirtualConsole* new_vconsole_object() { return 0; };
	virtual void init_output_buffers() {};
	void start_plugins();
	int test_reconfigure(long position, long &length);

	void evaluate_current_position();
	void start_command();
	virtual int restart_playback();
	virtual void run();

	RenderEngine *renderengine;
// Virtual console
	VirtualConsole *vconsole;
// Native units position in project used for all functions
	long current_position;       
	Mutex start_lock;
// flag for normally completed playback
	int done;       
// Flag for interrupted playback
	int interrupt;
// flag for last buffer to be played back
	int last_playback;  
// if this media type is being rendered asynchronously by threads
	int asynchronous;     
// Module for every track to dispatch plugins in
// Maintain module structures here instead of reusing the EDL so 
// plugins persist if the EDL is replaced.
// Modules persist after vconsole is restarted
	int total_modules;
	Module **modules;
	int data_type;
// If a VirtualConsole was created need to start plugins
	int restart_plugins;
	
	






	CommonRender(MWindow *mwindow, RenderEngine *renderengine);

// clean up rendering
	int virtual stop_rendering() {};
	int wait_for_completion();
	virtual int wait_device_completion() {};
// renders to a device when there's a device
	virtual int process_buffer(long input_len, long input_position) {};

	virtual int get_datatype() {};
// test region against loop boundaries
	int get_boundaries(long &current_render_length);
// test region for playback automation changes
	int get_automation(long &current_render_length, int data_type);
// advance the buffer position depending on the loop status
	int advance_position(long current_render_length);

// convert to and from the native units of the render engine
	virtual long tounits(double position, int round);
	virtual double fromunits(long position);
	virtual long get_render_length(long current_render_length) {};

	MWindow *mwindow;

	long input_length;           // frames/samples to read from disk at a time

protected:
// make sure automation agrees with playable tracks
// automatically tests direction of playback
// return 1 if it doesn't
	int test_automation_before(long &current_render_length, int data_type);
	int test_automation_after(long &current_render_length, int data_type);
};


#endif