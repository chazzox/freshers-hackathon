#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "utils.h"

// This always runs headless and contextless
int main() {
    int i = 0;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	bool running = true;

	// Initialize allegro
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro.\n");
		return 1;
	}

	// Initialize the timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Failed to create timer.\n");
		return 1;
	}

	// Create the display
	display = al_create_display(640, 480);
	if (!display) {
		fprintf(stderr, "Failed to create display.\n");
		return 1;
	}

	// Create the event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "Failed to create event queue.");
		return 1;
	}

	// Register event sources
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// Display a black screen
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	// Start the timer
	al_start_timer(timer);

	// Game loop
    bool redraw = true;
	while (running) {
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;

		// Initialize timeout
		al_init_timeout(&timeout, 0.06);

		// Fetch the event (if one exists)
		bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

		// Handle the event
		if (get_event) {
			switch (event.type) {
				case ALLEGRO_EVENT_TIMER:
                    redraw = true;
                    break;
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					running = false;
					break;
				default:
					fprintf(stderr, "Unsupported event received: %d\n", event.type);
					break;
			}
		}

		// Check if we need to redraw
		if (redraw && al_is_event_queue_empty(event_queue)) {
			// Redraw
			al_clear_to_color(al_map_rgb(i % 256, i % 256, i % 256));
			al_flip_display();
            i++;
            redraw = false;
		}
	}

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
