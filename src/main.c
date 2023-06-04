#include "main.h"
#include "init.h"

int main(int argc, char *argv[])
{
	struct global_context gctx = {
		.cam.fovmin = 10 * GLM_PIf / 180.0f,
		.cam.fovmax = 140 * GLM_PIf / 180.0f,
		.cam.fov = 100 * GLM_PIf / 180.0f,
		.ch1.fov_deg = 360,
		.projection = false,
		.interface_mult = 1};
	/* GUI */
	SDL_GLContext glContext;

	/* SDL setup */
	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/* Window size doesn't really matter here, since the HTML canvas determines
	   the size. But still, it is recommended to keep a default size */
	gctx.win = SDL_CreateWindow("Frost-O-Rama",
								SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								800, 600,
								SDL_WINDOW_OPENGL |
									SDL_WINDOW_SHOWN |
									SDL_WINDOW_RESIZABLE); /* |
								   SDL_WINDOW_ALLOW_HIGHDPI */
	glContext = SDL_GL_CreateContext(gctx.win);

	/* OpenGL setup */
	glClearColor(0, 0, 0, 1);
	/* Prevents headaches when loading NPOT textures */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	gctx.ctx = nk_sdl_init(gctx.win);

	gctx.ch1.img = load_texture("res/img/room.jpg", gctx.ch1.img);
	gctx.cam.fov = glm_rad(100);
	gctx.cam.cam_rotation[1] = 1.5;
	gctx.ch1.crop.top = 0;
	gctx.ch1.crop.bot = 0;
	gctx.ch1.crop.left = 0;
	gctx.ch1.crop.right = 0;
	gctx.ch1.fov_deg = 360;

	gctx.interface_mult = 1.4;

	gctx.ctx->style.scrollv.rounding_cursor = 12 * gctx.interface_mult;
	gctx.ctx->style.scrollv.rounding = 12 * gctx.interface_mult;
	gctx.ctx->style.property.rounding = 12 * gctx.interface_mult;
	gctx.ctx->style.window.scrollbar_size = nk_vec2(24 * gctx.interface_mult, 24 * gctx.interface_mult);
	init_fonts(&gctx);
	init_shaders(&gctx);

	emscripten_set_main_loop_arg(MainLoop, (void *)&gctx, 0, nk_true);

	nk_sdl_shutdown();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(gctx.win);
	SDL_Quit();
	return 0;
}
