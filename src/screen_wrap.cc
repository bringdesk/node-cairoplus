
#include "common.h"

/* Interface */

static napi_value Screen_New(napi_env env, napi_callback_info info);
static void Screen_Finalize(napi_env env, void* finalize_data, void* finalize_hint);
static napi_value Screen_Clear(napi_env env, napi_callback_info info);

/* Implementation */

/* Property */

static napi_value Screen_GetWidth(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    size_t argc = 1;
    napi_value args[1];
    napi_value result = NULL;
    screen_t *screen = NULL;

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, NULL);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 0) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return NULL;
    }

    status = napi_unwrap(env, jsthis, (void **)&screen);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_unwrap\n");
        abort();
    }

    fprintf(stderr, "debug: GetWidth on %p\n", screen);

    status = napi_create_int32(env, screen->width, &result);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_create_int32\n");
        abort();
    }

    return result;
}

static napi_value Screen_GetHeight(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    size_t argc = 1;
    napi_value args[1];
    napi_value result = NULL;
    screen_t *screen = NULL;

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, NULL);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 0) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return NULL;
    }

    status = napi_unwrap(env, jsthis, (void **)&screen);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_unwrap\n");
        abort();
    }

    fprintf(stderr, "debug: GetHeight on %p\n", screen);

    status = napi_create_int32(env, screen->height, &result);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_create_int32\n");
        abort();
    }

    return result;
}

/* Methods */

static napi_value Screen_Clear(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    size_t argc = 1;
    napi_value args[1];
    screen_t *screen = NULL;

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 0) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }

    status = napi_unwrap(env, jsthis, (void **)&screen);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_unwrap\n");
        abort();
    }

    fprintf(stderr, "debug: Clear on %p\n", screen);

    screen_clear(screen);

    return NULL;
}

static napi_value Screen_SetColor(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    size_t argc = 4;
    napi_value args[4];
    screen_t *screen = NULL;
    double red = 0.0;
    double green = 1.0;
    double blue = 0.0;
    double alpha = 0.5;
    int err;

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 4) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_double(env, args[0], &red);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_double(env, args[1], &green);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_double(env, args[2], &blue);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_double(env, args[3], &alpha);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_unwrap(env, jsthis, (void **)&screen);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_unwrap\n");
        abort();
    }

    fprintf(stderr, "debug: SetColor(%f, %f, %f, %f) on %p\n", red, green, blue, alpha, screen);

    err = screen_set_color(screen, red, green, blue, alpha);
    if (err != 0) {
        napi_throw_type_error(env, nullptr, "Fail on SetColor");
        return nullptr;
    }

    return NULL;
}

static napi_value Screen_DrawRectangle(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    size_t argc = 4;
    napi_value args[4];
    screen_t *screen = NULL;
    double left = 0.0;
    double top = 0.0;
    double width = 100.0;
    double height = 100.0;
    int err;

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 4) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_double(env, args[0], &left);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_double(env, args[1], &top);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_double(env, args[2], &width);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_double(env, args[3], &height);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_unwrap(env, jsthis, (void **)&screen);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_unwrap\n");
        abort();
    }

    fprintf(stderr, "debug: DrawRectangle(%f, %f, %f, %f) on %p\n", left, top, width, height, screen);

    err = screen_draw_rectangle(screen, left, top, width, height);
    if (err != 0) {
        napi_throw_type_error(env, nullptr, "Fail on DrawRectangle");
        return nullptr;
    }

    return NULL;
}

static napi_value Screen_SelectFontFace(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    size_t argc = 1;
    napi_value args[1];
    screen_t *screen = NULL;
    char name[512] = {0};
    size_t name_len = 512;
    int err;

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 1) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_string_utf8(env, args[0], (char *)&name, 512 - 1, &name_len);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Invalid parameter");
        return nullptr;
    }

    status = napi_unwrap(env, jsthis, (void **)&screen);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_unwrap\n");
        abort();
    }

    fprintf(stderr, "debug: SetFontFace(%s) on %p\n", name, screen);

    err = screen_select_font_face(screen, (const char *)&name);
    if (err != 0) {
        napi_throw_type_error(env, nullptr, "Fail on DrawRectangle");
        return nullptr;
    }

    return NULL;
}

static napi_value Screen_SetFontSize(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    size_t argc = 1;
    napi_value args[1];
    screen_t *screen = NULL;
    double size = 64.0;
    int err;

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 1) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_double(env, args[0], &size);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_unwrap(env, jsthis, (void **)&screen);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_unwrap\n");
        abort();
    }

    fprintf(stderr, "debug: SetFontSize(%f) on %p\n", size, screen);

    err = screen_set_font_size(screen, size);
    if (err != 0) {
        napi_throw_type_error(env, nullptr, "Fail on SetFontSize");
        return nullptr;
    }

    return NULL;
}

static napi_value Screen_DrawText(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    size_t argc = 1;
    napi_value args[1];
    screen_t *screen = NULL;
    char msg[512] = { 0 };
    size_t msg_len = 0;
    int err;

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 1) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }

    status = napi_get_value_string_utf8(env, args[0], (char *)&msg, 512 - 1, &msg_len);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Invalid parameter");
        return nullptr;
    }
    status = napi_unwrap(env, jsthis, (void **)&screen);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_unwrap\n");
        abort();
    }

    fprintf(stderr, "debug: DrawText(%s) on %p\n", msg, screen);

    err = screen_show_text(screen, (char *)&msg);
    if (err != 0) {
        napi_throw_type_error(env, nullptr, "Fail on SetFontSize");
        return nullptr;
    }

    return NULL;
}

static napi_value Screen_DrawImage(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    size_t argc = 3;
    napi_value args[3];
    screen_t *screen = NULL;
    char image[512] = { 0 };
    size_t image_len = 0;
    int pos_x = 100;
    int pos_y = 100;
    int err;

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 3) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_string_utf8(env, args[0], (char *)&image, 512 - 1, &image_len);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Invalid parameter");
        return nullptr;
    }
    status = napi_get_value_int32(env, args[1], &pos_x);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Invalid parameter");
        return nullptr;
    }
    status = napi_get_value_int32(env, args[2], &pos_y);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Invalid parameter");
        return nullptr;
    }
    status = napi_unwrap(env, jsthis, (void **)&screen);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_unwrap\n");
        abort();
    }

    fprintf(stderr, "debug: DrawImage(%s, %u, %u) on %p\n", image, pos_x, pos_y, screen);

    err = screen_draw_image(screen, (char *)&image, pos_x, pos_y);
    if (err != 0) {
        napi_throw_type_error(env, nullptr, "Fail on DrawImage");
        return nullptr;
    }

    return NULL;
}

static napi_value Screen_MoveTo(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    size_t argc = 2;
    napi_value args[2];
    screen_t *screen = NULL;
    double posX = 100.0;
    double posY = 100.0;
    int err;

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 2) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_double(env, args[0], &posX);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_get_value_double(env, args[1], &posY);
    if (status != napi_ok) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }
    status = napi_unwrap(env, jsthis, (void **)&screen);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_unwrap\n");
        abort();
    }

    fprintf(stderr, "debug: MoveTo(%f, %f) on %p\n", posX, posY, screen);

    err = screen_move_to(screen, posX, posY);
    if (err != 0) {
        napi_throw_type_error(env, nullptr, "Fail on MoveTo");
        return nullptr;
    }

    return NULL;
}

static napi_value Screen_Swap(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    size_t argc = 1;
    napi_value args[1];
    screen_t *screen = NULL;
    int err;

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    if (status != napi_ok) {
        fprintf(stderr, "eroror: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 0) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }

    status = napi_unwrap(env, jsthis, (void **)&screen);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_unwrap\n");
        abort();
    }

    fprintf(stderr, "debug: Swap() on %p\n", screen);

    err = screen_swap(screen);
    if (err != 0) {
        napi_throw_type_error(env, nullptr, "Fail on SetFontSize");
        return nullptr;
    }

    return NULL;
}

/* Constructor and other */

static napi_value Screen_New(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value jsthis;
    screen_t *screen = NULL;
    size_t argc = 1;
    napi_value args[1];

    /* Constructor parameters */
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_get_cb_info\n");
        abort();
    }
    if (argc != 1) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments");
        return nullptr;
    }

    /* Create C screen instance */
    screen = screen_new();
    if (screen == NULL) {
        fprintf(stderr, "error: Fail on malloc\n");
        abort();
    }
    fprintf(stderr, "debug: Create screen in %p\n", screen);
    screen_init(screen);

    status = napi_wrap(env, jsthis, screen, Screen_Finalize, NULL, NULL);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_wrap\n");
        abort();
    }

    return jsthis;
}

static void Screen_Finalize(napi_env env, void* finalize_data, void* finalize_hint)
{

}

static napi_value create_addon(napi_env env)
{
    napi_status status;
    napi_value result;

    /* Create export object */
    NAPI_CALL(env, napi_create_object(env, &result));

    /* Create class wrapper */
    napi_property_descriptor properties[] = {
        /* Property */
        { "width", NULL, NULL, Screen_GetWidth, NULL, NULL, napi_default, NULL },
        { "height", NULL, NULL, Screen_GetHeight, NULL, NULL, napi_default, NULL },
        /* Methods */
        DECLARE_NAPI_METHOD("Clear", Screen_Clear),                   // 1
        DECLARE_NAPI_METHOD("SetColor", Screen_SetColor),             // 2
        DECLARE_NAPI_METHOD("DrawRectangle", Screen_DrawRectangle),   // 3
        DECLARE_NAPI_METHOD("SelectFontFace", Screen_SelectFontFace), // 4
        DECLARE_NAPI_METHOD("SetFontSize", Screen_SetFontSize),       // 5
        DECLARE_NAPI_METHOD("DrawText", Screen_DrawText),             // 6
        DECLARE_NAPI_METHOD("DrawImage", Screen_DrawImage),           // 7
        DECLARE_NAPI_METHOD("MoveTo", Screen_MoveTo),                 // 8
        DECLARE_NAPI_METHOD("Swap", Screen_Swap),                     // 9
    };
    napi_value cons;
    int propertyCount = sizeof(properties) / sizeof(napi_property_descriptor);
    status = napi_define_class(env, "Screen", NAPI_AUTO_LENGTH, Screen_New, NULL, propertyCount, properties, &cons);
    if (status != napi_ok) {
        fprintf(stderr, "error: Fail on napi_define_class\n");
        abort();
    }

//    status = napi_create_reference(env, cons, 1, &constructor);
//    if (status != napi_ok) {
//        abort();
//    }

    NAPI_CALL(env, napi_set_named_property(env, result, "Screen", cons));

    return result;
}

NAPI_MODULE_INIT()
{
    return create_addon(env);
}
