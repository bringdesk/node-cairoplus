
#include "common.h"


screen_t *screen_new(void)
{
    screen_t *scr = NULL;

    scr = (screen_t *)malloc(sizeof(*scr));
    if (scr == NULL) {
        goto out;
    }
    scr->fbcr = NULL;
    scr->cache_cr = NULL;
    scr->cache = false;

  out:

    return scr;
}

int screen_init(screen_t *scr)
{
    int rc;

    // Open the file for reading and writing
    scr->fb_fd = open("/dev/fb0", O_RDWR);
    if (scr->fb_fd == -1) {
        perror("Error: cannot open framebuffer device");
        rc = 1;
        goto out;
    }

    scr->tty_fd = open("/dev/tty0", O_RDWR);
    if (scr->tty_fd == -1) {
        perror("Error: cannot open framebuffer device");
        rc = 1;
        goto out;
    }

    // Get variable screen information
    if (ioctl(scr->fb_fd, FBIOGET_VSCREENINFO, &scr->fb_vinfo) == -1) {
        perror("Error: reading variable information");
        rc = 1;
        goto out;
    }

    // Set virtual display size double the width for double buffering
//    device->fb_vinfo.xoffset = 0;
//    device->fb_vinfo.yoffset = 0;
//    device->fb_vinfo.yres_virtual = device->fb_vinfo.yres; // * 2
//    device->fb_vinfo.xres_virtual = device->fb_vinfo.xres; // * 2
//    if (ioctl(device->fb_fd, FBIOPUT_VSCREENINFO, &device->fb_vinfo)) {
//	perror("Error setting variable screen info from fb");
//	goto handle_ioctl_error;
//    }

    // Get fixed screen information
    if (ioctl(scr->fb_fd, FBIOGET_FSCREENINFO, &scr->fb_finfo) == -1) {
        perror("Error: reading fixed information");
        rc = 1;
        goto out;
    }

    // Figure out the size of the screen in bytes
//    scr->fb_screensize = scr->fb_vinfo.xres * scr->fb_vinfo.yres * scr->fb_vinfo.bits_per_pixel / 8;
    scr->fb_screensize = scr->fb_finfo.smem_len;

#ifndef NDEBUG
    fprintf(stderr, "Screen size: %d\n", scr->fb_screensize);
#endif

    // Map the device to memory 
    scr->fb_data = (unsigned char *)mmap(0, scr->fb_screensize, PROT_READ | PROT_WRITE, MAP_SHARED, scr->fb_fd, 0);
    if ((int)scr->fb_data == -1) {
        perror("Error: failed to map framebuffer device to memory");
        rc = 1;
        goto out;
    }

    /* Show debug information */
    fprintf(stderr, "Xres: %d\n", scr->fb_vinfo.xres);
    fprintf(stderr, "Yres: %d\n", scr->fb_vinfo.yres);
    fprintf(stderr, "XresV: %d\n", scr->fb_vinfo.xres_virtual);
    fprintf(stderr, "YresV: %d\n", scr->fb_vinfo.yres_virtual);
    fprintf(stderr, "BPP: %d\n", scr->fb_vinfo.bits_per_pixel);

    /* Set screen size */
    scr->width = scr->fb_vinfo.xres;
    scr->height = scr->fb_vinfo.yres;

    // Select image format mode
    if (scr->fb_vinfo.bits_per_pixel == 32) {
        scr->format = CAIRO_FORMAT_ARGB32;
        scr->format = CAIRO_FORMAT_RGB24;
        //scr->format = CAIRO_FORMAT_RGB30;
    } else {
        scr->format = CAIRO_FORMAT_RGB16_565;
    }

    int stride = cairo_format_stride_for_width(scr->format, scr->fb_vinfo.xres);
    stride = scr->fb_vinfo.xres * scr->fb_vinfo.bits_per_pixel / 8 + 32 + 8;
    stride = scr->fb_finfo.line_length;// / (device->fb_vinfo.bits_per_pixel >> 3);

#ifndef NDEBUG
    fprintf(stderr, "Stride: %d\n", stride);
#endif

    // Create the cairo surface which will be used to draw to
    scr->fbsurface = cairo_image_surface_create_for_data(scr->fb_data,
	    scr->format,
	    scr->fb_vinfo.xres,
	    scr->fb_vinfo.yres,
	    stride);

    if (-1 == ioctl(scr->tty_fd, KDSETMODE, KD_GRAPHICS)) {
        perror("Error: cannot switch console in graphics mode");
        rc = 1;
        goto out;
    }

    scr->fbcr = cairo_create(scr->fbsurface);

    scr->cache_surface = cairo_image_surface_create ( scr->format, scr->fb_vinfo.xres, scr->fb_vinfo.yres );

    scr->cache_cr = cairo_create(scr->cache_surface);

    scr->cache = true;

    rc = 0;

  out:

    return rc;
}

int screen_clear(screen_t *scr)
{
    cairo_t *cr = scr->cache ? scr->cache_cr : scr->fbcr;
    int rc;

    if (scr == NULL) {
        rc = 1;
        goto out;
    }
    cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cr);

    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

    rc = 0;
  out:
    return rc;
}

int screen_move_to(screen_t *scr, double pos_x, double pos_y)
{
    cairo_t *cr = scr->cache ? scr->cache_cr : scr->fbcr;
    int rc;
    cairo_move_to(cr, pos_x, pos_y);
    rc = 0;
    return rc;
}

#if defined(CAIRO_HAS_PNG_FUNCTIONS) && (CAIRO_HAS_PNG_FUNCTIONS == 1)

int screen_draw_image(screen_t *scr, const char *image, int pos_x, int pos_y)
{
    cairo_t *cr = scr->cache ? scr->cache_cr : scr->fbcr;
    int rc;
    cairo_surface_t *cs = NULL;
    cs = cairo_image_surface_create_from_png (image);
    if (cs == NULL) {
        rc = 1;
        goto out;
    }
    cairo_set_source_surface(cr, cs, pos_x, pos_y);
    cairo_paint(cr);
    rc = 0;
  out:
    if (cs != NULL) {
        cairo_surface_destroy(cs);
        cs = NULL;
    }
    return rc;
}

#endif

int screen_select_font_face(screen_t *scr, const char *name)
{
    cairo_t *cr = scr->cache ? scr->cache_cr : scr->fbcr;
    int rc;
    cairo_select_font_face(cr, name, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    rc = 0;
    return rc;
}

int screen_set_font_size(screen_t *scr, double size)
{
    cairo_t *cr = scr->cache ? scr->cache_cr : scr->fbcr;
    int rc;
    cairo_set_font_size(cr, size);
    rc = 0;
    return rc;
}

int screen_set_color(screen_t *scr, double r, double g, double b, double a)
{
    cairo_t *cr = scr->cache ? scr->cache_cr : scr->fbcr;
    int rc;
    cairo_set_source_rgba (cr, r, g, b, a);
    rc = 0;
    return rc;
}

//int screen_draw_circle()
//{
//    cairo_t *cr = scr->cache ? scr->cache_cr : scr->fbcr;
//    int pos_x = lua_tointeger(L, 1);
//    int pos_y = lua_tointeger(L, 1);
//    int size = lua_tointeger(L, 1);
//    cairo_t *cr = ctx->scr->cr;
//    cairo_identity_matrix(cr);
//    cairo_arc(cr, 0, 0, size, 0, 2 * M_PI);
//    cairo_translate(cr, pos_x, pos_y);
//    cairo_stroke_preserve(cr);
//    cairo_fill(cr);
//    return rc;
//}

int screen_draw_rectangle(screen_t *scr, double pos_x, double pos_y, double width, double height)
{
    cairo_t *cr = scr->cache ? scr->cache_cr : scr->fbcr;
    int rc;

    cairo_rectangle (cr, pos_x, pos_y, width, height);
    cairo_fill (cr);

    rc = 0;
    return rc;
}

int screen_show_text(screen_t *scr, const char *msg)
{
    cairo_t *cr = scr->cache ? scr->cache_cr : scr->fbcr;
    int rc;
    cairo_show_text(cr, msg);
    rc = 0;
    return rc;
}

int screen_swap(screen_t *scr)
{
    int rc;
    cairo_set_source_surface(scr->fbcr, scr->cache_surface, 0, 0);
    cairo_paint(scr->fbcr);
    rc = 0;
    return rc;
}

/**
 * Destroy a cairo surface
 */
void screen_fini(screen_t *scr)
{
    cairo_destroy(scr->fbcr);

    munmap(scr->fb_data, scr->fb_screensize);
    close(scr->fb_fd);
}



void screen_free(screen_t *scr)
{
    free(scr);
}
