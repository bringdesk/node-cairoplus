
#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif

typedef struct screen_t screen_t;
struct screen_t {

    cairo_surface_t *cache_surface;
    cairo_t *cache_cr;
    bool cache;

    cairo_surface_t *fbsurface;
    cairo_t *fbcr;

    int fb_fd;
    int tty_fd;

    unsigned char *fb_data;
    long fb_screensize;
    struct fb_var_screeninfo fb_vinfo;
    struct fb_fix_screeninfo fb_finfo;
    cairo_format_t format;

    int width;
    int height;

};

screen_t *screen_new();
int screen_init(screen_t *scr);
int screen_clear(screen_t *scr);
int screen_move_to(screen_t *scr, double pos_x, double pos_y);
int screen_select_font_face(screen_t *scr, const char *name);
int screen_draw_image(screen_t *scr, const char *image, int pos_x, int pos_y);
int screen_set_font_size(screen_t *scr, double size);
int screen_draw_rectangle(screen_t *src, double pos_x, double pos_y, double width, double height);
int screen_set_color(screen_t *scr, double r, double g, double b, double a);
int screen_show_text(screen_t *scr, const char *msg);
int screen_swap(screen_t *scr);
void screen_fini(screen_t * scr);
void screen_free(screen_t * scr);

#ifdef __cplusplus
}
#endif
