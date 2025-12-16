#include "fx_api.h"
#include "web_assets.h"

/* If you want to overwrite existing files on each boot, set to 1. */
#ifndef WEBUI_FORCE_OVERWRITE
#define WEBUI_FORCE_OVERWRITE (1)
#endif

/* Change this if your HTTP server serves from a different folder. */
#ifndef WEBUI_ROOT_DIR
#define WEBUI_ROOT_DIR "/"
#endif

static UINT ensure_dir(FX_MEDIA * media, CHAR * dir_path)
{
    UINT st = fx_directory_create(media, dir_path);
    if (st == FX_ALREADY_CREATED) return FX_SUCCESS;
    return st;
}

static UINT file_exists(FX_MEDIA * media, CHAR * path)
{
    FX_FILE f;
    UINT st = fx_file_open(media, &f, path, FX_OPEN_FOR_READ);
    if (st == FX_SUCCESS) { fx_file_close(&f); return FX_SUCCESS; }
    return st;
}

static UINT write_file(FX_MEDIA * media, CHAR * path, const UCHAR * data, ULONG len)
{
    FX_FILE f;
    UINT st;
    st = fx_file_create(media, path);
    if ((st != FX_SUCCESS) && (st != FX_ALREADY_CREATED)) return st;
    st = fx_file_open(media, &f, path, FX_OPEN_FOR_WRITE);
    if (st != FX_SUCCESS) return st;
    st = fx_file_truncate(&f, 0);
    if (st != FX_SUCCESS) { (void)fx_file_close(&f); return st; }
    st = fx_file_write(&f, (VOID*)data, len);
    (void)fx_file_close(&f);
    if (st != FX_SUCCESS) return st;
    return fx_media_flush(media);
}

static UINT provision_one(FX_MEDIA * media, CHAR * path, const UCHAR * data, ULONG len)
{
#if (WEBUI_FORCE_OVERWRITE == 0)
    UINT st = file_exists(media, path);
    if (st == FX_NOT_FOUND) return write_file(media, path, data, len);
    return (st == FX_SUCCESS) ? FX_SUCCESS : st;
#else
    return write_file(media, path, data, len);
#endif
}

/* Call once after fx_media_open() succeeds (USB mounted) and before using HTTP server. */
UINT webui_provision_to_usb(FX_MEDIA * media)
{
    UINT st;

#if (WEBUI_FORCE_OVERWRITE == 0)
    st = ensure_dir(media, (CHAR*)WEBUI_ROOT_DIR);
    if (st != FX_SUCCESS) return st;
#endif

    st = provision_one(media, (CHAR*)WEBUI_ROOT_DIR "index.html", WEB_INDEX_HTML, (ULONG)WEB_INDEX_HTML_LEN);
    if (st != FX_SUCCESS) return st;
    st = provision_one(media, (CHAR*)WEBUI_ROOT_DIR "app.html", WEB_APP_HTML, (ULONG)WEB_APP_HTML_LEN);
    if (st != FX_SUCCESS) return st;
    st = provision_one(media, (CHAR*)WEBUI_ROOT_DIR "file_manager.js", WEB_FILE_MANAGER_JS, (ULONG)WEB_FILE_MANAGER_JS_LEN);
    if (st != FX_SUCCESS) return st;
    return FX_SUCCESS;
}
