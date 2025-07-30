#include "aura.h"
#include "utils/window.h"
#include "utils/logging.h"
#include "utils/allocator.h"

#include <stdint.h>

uint32_t image[256][256] = {0};

static uint32_t make_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
}

static void fill_image() {
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            uint8_t r = x;
            uint8_t g = y;
            uint8_t b = (x + y) / 2;
            uint8_t a = 255;

            image[x][y] = make_rgba(r, g, b, a);
        }
    }
}

aura_vertex_t vertices[3] = {
	{ .position = vec2(0, 0), .color = color(1, 0, 0, 1)},
	{ .position = vec2(300, 0), .color = color(0, 0, 1, 1) },
	{ .position = vec2(0, 300), .color = color(0, 1, 0, 1) },
};


int main(void){
	window_t win = window_create("window name", 800, 600);
	aura_context_t ctx = aura_init(win.SDL3_window);
	fill_image();

	while (window_open(&win)) {
		aura_render(&ctx);
		window_update(&win);
		aura_clear(&ctx, color(1, 1, 1, 1));
		
		aura_texture_t* tex = aura_add_texture(&ctx, 256, 256, image);
		aura_geometry(&ctx, NULL, vertices, 3, NULL, 0);
		aura_debug_text_fmt(&ctx, vec2(10, 10), color(1, 0, 0, 1), "Hello world %u", 123);
		aura_debug_text(&ctx, "second Hello world", vec2(10, 30), color(0, 0, 0, 1));
		aura_line(&ctx, vec2(0, 0), mouse_get_position(&win), color(0, 0, 0, 1));
		aura_rectangle(&ctx, rectangle(0, 0, 100, mouse_get_position(&win).y), color(1, 0, 0, 0.1f));
		aura_texture_sprite(&ctx, tex, rectangle(300, 300, 256, 256), rectangle(0, 0, 256, 256), 18, vec2(tex->width/2.0f, tex->height/2.0f), false, false);

		uint32_t width = 400, height = 200;
		//aura_rounded_rectangle(&ctx, rectangle(200, 200, width, height), 15, color(.2, .2, .2, 1));
		aura_rounded_rectangle_corners(&ctx, rectangle(200, 200, width, height), 15, 15, 5, 5, color(.1, .1, .1, 1));


		if (key_just_down(&win, KEY_ESCAPE)) window_close(&win);
	}

	aura_deinit(&ctx);
	window_destroy(&win);
	return 0;
}
