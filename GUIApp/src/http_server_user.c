/*
 * http_server_user.c
 *
 * User HTTP server callbacks for SK-S7G2 file browser + uploads.
 */

#include "network_thread.h"
#include "tx_api.h"
#include "nx_api.h"
#include "nxd_http_server.h"
#include "fx_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hmi_state.h"
#include "led_control/led.h"
#include "auth_manager.h"

#include <common_resource.h>

#define MAX_PATH_LEN                (255u)
#define MAX_NAME_CHARS              (14u)      /* your tuned value */
#define HTTP_UPLOAD_BUFFER_SIZE     (8192u)    //(4096u)
#define HTTP_UPLOAD_CHUNK_MAX_SIZE  (65536u)   //(16384u)

#define UPLOAD_TIMEOUT_MAX_RETRIES  (5u)
#define UPLOAD_TIMEOUT_SLEEP_TICKS  (100u)

#define FOLDER_ICON_PATH            "/rsc/folder_icon.png"
#define FILE_ICON_PATH              "/rsc/file_ico.png"

/* ------------- Simple authentication ----------------- */
#define ADMIN_USERNAME              "admin"
#define ADMIN_DEFAULT_PASSWORD      "admin"   /* used only if QSPI auth backend is unavailable */


static UINT g_logged_in = 0;       /* 0 = not logged in, 1 = logged in */

/* USB FileX media provided by the USB thread (must be defined there). */
extern FX_MEDIA * g_usb_media;

/* -----------------------------------------------------------------------
 *  Static buffers to reduce stack usage in HTTP callback thread
 * ----------------------------------------------------------------------- */
static CHAR g_header_buf[256];


/* ---------------- Upload session (single upload at a time) --------------- */
static UINT   g_upload_active = 0u;
static ULONG  g_upload_next_offset = 0u;
static CHAR   g_upload_path[MAX_PATH_LEN + 1];
static CHAR   g_upload_name[MAX_NAME_CHARS + 1];
static FX_FILE g_upload_file;
static UCHAR g_upload_copybuf[HTTP_UPLOAD_BUFFER_SIZE];


/* ---------- Local helpers ---------- */

static UINT http_send_data(NX_HTTP_SERVER * server_ptr, const CHAR * data, UINT length);
static UINT http_send_error(NX_HTTP_SERVER * server_ptr, const CHAR * msg);
static void  url_decode(const CHAR * src, CHAR * dst, UINT dst_size);

static UINT http_handle_download(NX_HTTP_SERVER * server_ptr, const CHAR * file_path, NX_PACKET * packet_ptr);
static UINT http_send_static_file(NX_HTTP_SERVER * server_ptr, const CHAR * path_on_usb);
static UINT http_handle_upload_chunk(NX_HTTP_SERVER * server_ptr, CHAR * resource, NX_PACKET * packet_ptr);
static UINT http_handle_upload_stream(NX_HTTP_SERVER * server_ptr, CHAR * resource, NX_PACKET * packet_ptr);
static UINT http_send_usb_stats(NX_HTTP_SERVER * server_ptr);

static const CHAR * http_guess_mime(const CHAR * path)
{
    const CHAR * ext = strrchr(path, '.');
    if (!ext) return "application/octet-stream";

    /* Note: ext includes '.' */
    if (strcmp(ext, ".pdf") == 0) return "application/pdf";

    if (strcmp(ext, ".mp4") == 0) return "video/mp4";
    if (strcmp(ext, ".avi") == 0) return "video/x-msvideo";
    if (strcmp(ext, ".zip") == 0) return "application/zip";

    /* NEF raw */
    if (strcmp(ext, ".nef") == 0) return "application/octet-stream";

    if (strcmp(ext, ".png") == 0) return "image/png";
    if ((strcmp(ext, ".jpg") == 0) || (strcmp(ext, ".jpeg") == 0)) return "image/jpeg";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    if (strcmp(ext, ".webp") == 0) return "image/webp";
    if (strcmp(ext, ".bmp") == 0) return "image/bmp";

    if ((strcmp(ext, ".txt") == 0) || (strcmp(ext, ".log") == 0) ||
        (strcmp(ext, ".csv") == 0) || (strcmp(ext, ".json") == 0) ||
        (strcmp(ext, ".xml") == 0) || (strcmp(ext, ".c") == 0)   ||
        (strcmp(ext, ".h") == 0)   || (strcmp(ext, ".cpp") == 0) ||
        (strcmp(ext, ".hpp") == 0) || (strcmp(ext, ".md") == 0)  ||
        (strcmp(ext, ".ini") == 0) || (strcmp(ext, ".cfg") == 0) ||
        (strcmp(ext, ".yml") == 0) || (strcmp(ext, ".yaml") == 0))
    {
        return "text/plain; charset=utf-8";
    }

    return "application/octet-stream";
}

static UINT http_is_inline_type(const CHAR * mime)
{
    if (!mime) return 0u;
    if (strncmp(mime, "image/", 6) == 0) return 1u;
    if (strncmp(mime, "text/", 5) == 0)  return 1u;
    if (strcmp(mime, "application/pdf") == 0) return 1u;
    if (strcmp(mime, "video/mp4") == 0) return 1u;

    return 0u;
}

static const CHAR * http_basename(const CHAR * path)
{
    const CHAR * p = strrchr(path, '/');
    return (p != NX_NULL) ? (p + 1) : path;
}


/* Send raw bytes to the client from the callback using NetX HTTP extended API. */
static UINT http_send_data(NX_HTTP_SERVER * server_ptr, const CHAR * data, UINT length)
{
    hmi_set_status_led(LED_ON, LED_OFF, LED_ON);
    return nx_http_server_callback_data_send(server_ptr, (VOID *) data, (ULONG) length);
}

static UINT http_query_from_request_packet(NX_PACKET *packet_ptr,
                                           CHAR      *query_out,
                                           UINT       query_out_len)
{
    /* Read the beginning of the HTTP request (request line + some headers). */
    CHAR  buf[256];
    ULONG copied = 0;

    if ((query_out == NX_NULL) || (query_out_len < 2u))
    {
        return NX_NOT_SUCCESSFUL;
    }

    query_out[0] = '\0';

    if (nx_packet_data_extract_offset(packet_ptr,
                                      0u,
                                      buf,
                                      (ULONG)(sizeof(buf) - 1u),
                                      &copied) != NX_SUCCESS)
    {
        return NX_NOT_SUCCESSFUL;
    }

    buf[copied] = '\0';

    /* Trim to the first line only. */
    CHAR *eol = strstr(buf, "\r\n");
    if (eol != NX_NULL)
    {
        *eol = '\0';
    }

    /* Expect: "POST <uri> HTTP/1.x" */
    CHAR *sp1 = strchr(buf, ' ');
    if (sp1 == NX_NULL) return NX_NOT_SUCCESSFUL;

    CHAR *sp2 = strchr(sp1 + 1, ' ');
    if (sp2 == NX_NULL) return NX_NOT_SUCCESSFUL;

    *sp2 = '\0';                 /* now (sp1+1) is the full URI */
    CHAR *uri = sp1 + 1;

    CHAR *q = strchr(uri, '?');
    if (q == NX_NULL) return NX_NOT_SUCCESSFUL;

    /* Copy including leading '?' so existing parsing "query+1" keeps working. */
    UINT i = 0u;
    while (q[i] && (i < (query_out_len - 1u)))
    {
        query_out[i] = q[i];
        i++;
    }
    query_out[i] = '\0';

    return NX_SUCCESS;
}

/* Send a simple HTTP 302 redirect to the given location. */
static UINT http_send_redirect(NX_HTTP_SERVER * server_ptr, const CHAR * location)
{
    CHAR header[192];
    UINT header_len = (UINT) snprintf(header, sizeof(header),
                                      "HTTP/1.0 302 Found\r\n"
                                      "Location: %s\r\n"
                                      "Connection: close\r\n"
                                      "\r\n",
                                      location);
    if (header_len > sizeof(header))
    {
        header_len = sizeof(header);
    }

    http_send_data(server_ptr, header, header_len);
    return NX_HTTP_CALLBACK_COMPLETED;
}


/* Simple helper to send a plain-text error response. */
static UINT http_send_error(NX_HTTP_SERVER * server_ptr, const CHAR * msg)
{
    if (!msg)
    {
        msg = "Internal error\r\n";
    }

    CHAR header[192];
    UINT header_len = (UINT) snprintf(header, sizeof(header),
                                      "HTTP/1.0 400 Bad Request\r\n"
                                      "Content-Type: text/plain\r\n"
                                      "Content-Length: %u\r\n"
                                      "Connection: close\r\n"
                                      "\r\n",
                                      (unsigned) strlen(msg));

    if ((INT)header_len < 0) header_len = 0;
    if (header_len >= sizeof(header)) header_len = sizeof(header) - 1;

    http_send_data(server_ptr, header, header_len);
    http_send_data(server_ptr, msg, (UINT) strlen(msg));
    return NX_HTTP_CALLBACK_COMPLETED;
}


/* URL decode helper ------------------------------------------------------- */

static int hex_nibble(CHAR c)
{
    if ((c >= '0') && (c <= '9'))
    {
        return (int) (c - '0');
    }
    if ((c >= 'A') && (c <= 'F'))
    {
        return (int) (c - 'A' + 10);
    }
    if ((c >= 'a') && (c <= 'f'))
    {
        return (int) (c - 'a' + 10);
    }
    return -1;
}

static void url_decode(const CHAR * src, CHAR * dst, UINT dst_size)
{
    UINT di = 0u;
    UINT si = 0u;

    if (dst_size == 0u)
    {
        return;
    }

    while ((src[si] != '\0') && (di < (dst_size - 1u)))
    {
        if (src[si] == '%')
        {
            int hi = hex_nibble(src[si + 1u]);
            int lo = hex_nibble(src[si + 2u]);
            if ((hi >= 0) && (lo >= 0))
            {
                dst[di++] = (CHAR) ((hi << 4) | lo);
                si += 3u;
                continue;
            }
        }
        else if (src[si] == '+')
        {
            dst[di++] = ' ';
            si++;
            continue;
        }

        dst[di++] = src[si++];
    }

    dst[di] = '\0';
}

/* Serve static icon file from USB (/rsc/...) ----------------------------- */

static UINT http_send_static_file(NX_HTTP_SERVER * server_ptr, const CHAR * path_on_usb)
{
    if (g_usb_media == FX_NULL)
    {
        return http_send_error(server_ptr, "USB not mounted\r\n");
    }

    UINT    fx_status;
    FX_FILE file;

    fx_status = fx_file_open(g_usb_media, &file, (CHAR *) path_on_usb, FX_OPEN_FOR_READ);
    if (fx_status != FX_SUCCESS)
    {
        return http_send_error(server_ptr, "Static file not found\r\n");
    }

    const CHAR * content_type = "application/octet-stream";
    const CHAR * ext          = strrchr(path_on_usb, '.');
    if (ext)
    {
        if (strcmp(ext, ".png") == 0)
        {
            content_type = "image/png";
        }
        else if ((strcmp(ext, ".jpg") == 0) || (strcmp(ext, ".jpeg") == 0))
        {
            content_type = "image/jpeg";
        }
        else if (strcmp(ext, ".gif") == 0)
        {
            content_type = "image/gif";
        }
        else if ((strcmp(ext, ".html") == 0) || (strcmp(ext, ".htm") == 0))
        {
            content_type = "text/html";
        }
        else if (strcmp(ext, ".js") == 0)
        {
            content_type = "application/javascript";
        }
        else if (strcmp(ext, ".css") == 0)
        {
            content_type = "text/css";
        }
    }

    UINT header_len = (UINT) snprintf(g_header_buf, sizeof(g_header_buf),
                                      "HTTP/1.0 200 OK\r\n"
                                      "Content-Type: %s\r\n"
                                      "Connection: close\r\n"
                                      "\r\n",
                                      content_type);
    if (header_len > sizeof(g_header_buf))
    {
        header_len = sizeof(g_header_buf);
    }

    http_send_data(server_ptr, g_header_buf, header_len);

    UCHAR buffer[512];
    ULONG actual = 0u;

    do
    {
        fx_status = fx_file_read(&file, buffer, sizeof(buffer), &actual);
        if ((fx_status != FX_SUCCESS) && (fx_status != FX_END_OF_FILE))
        {
            break;
        }

        if (actual > 0u)
        {
            http_send_data(server_ptr, (CHAR *) buffer, (UINT) actual);
        }
    } while (fx_status != FX_END_OF_FILE);

    fx_file_close(&file);

    return NX_HTTP_CALLBACK_COMPLETED;
}

/* Returns percent * 100 (two decimals), e.g. 2667 = 26.67% */
static ULONG http_percent_x100_u64(ULONG64 used, ULONG64 total)
{
    if (total == 0ULL) return 0u;

    /* Avoid overflow: split quotient + remainder */
    ULONG64 q = used / total;
    ULONG64 r = used % total;

    ULONG64 pct_x100 = q * 10000ULL + (r * 10000ULL) / total;

    if (pct_x100 > 10000ULL) pct_x100 = 10000ULL; /* clamp to 100.00% */
    return (ULONG)pct_x100;
}


static UINT http_send_usb_stats(NX_HTTP_SERVER * server_ptr)
{
    if ((g_usb_media == FX_NULL) || (g_usb_media->fx_media_id != FX_MEDIA_ID))
    {
        return http_send_error(server_ptr, "USB not mounted\r\n");
    }

    /* FileX gives free bytes as ULONG (32-bit) on many SSP versions */
    ULONG free32 = 0u;
    UINT fx = fx_media_space_available(g_usb_media, &free32);
    if (fx != FX_SUCCESS)
    {
        return http_send_error(server_ptr, "fx_media_space_available failed\r\n");
    }

    ULONG64 total =
        (ULONG64)g_usb_media->fx_media_total_sectors *
        (ULONG64)g_usb_media->fx_media_bytes_per_sector;

    ULONG64 freeb = (ULONG64)free32;
    if (freeb > total) freeb = total;

    ULONG64 used  = total - freeb;

    ULONG used_pct_x100 = http_percent_x100_u64(used, total);
    ULONG used_pct_int  = used_pct_x100 / 100u; /* 0..100 */

    CHAR body[220];
    UINT body_len = (UINT)snprintf(body, sizeof(body),
        "{\"total\":%llu,\"free\":%llu,\"used\":%llu,\"used_pct\":%lu,\"used_pct_x100\":%lu}",
        (unsigned long long)total,
        (unsigned long long)freeb,
        (unsigned long long)used,
        (unsigned long)used_pct_int,
        (unsigned long)used_pct_x100);

    CHAR header[200];
    UINT header_len = (UINT)snprintf(header, sizeof(header),
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %u\r\n"
        "Connection: close\r\n"
        "\r\n",
        (unsigned)body_len);

    if (header_len >= sizeof(header)) header_len = (UINT)(sizeof(header) - 1u);

    http_send_data(server_ptr, header, header_len);
    http_send_data(server_ptr, body, body_len);
    return NX_HTTP_CALLBACK_COMPLETED;
}

/* Walk a URL-style path ("/", "/UI/", "/UI/rsc/emptyfolder/") and
 * set the FileX default directory accordingly by stepping through
 * each component with fx_directory_default_set().
 *
 * Returns NX_SUCCESS on success, or the FileX status if any step fails.
 */
static UINT filex_set_directory_from_url_path(const CHAR * url_path)
{
    CHAR decoded[MAX_PATH_LEN + 1];
    const CHAR * p;
    UINT status;

    /* Decode %xx etc. */
    url_decode(url_path ? url_path : "", decoded, sizeof(decoded));

    /* Always start from root. */
    status = fx_directory_default_set(g_usb_media, FX_NULL);
    if (status != FX_SUCCESS)
    {
        return status;
    }

    p = decoded;

    /* Empty or "/" means root – nothing more to do. */
    if (p[0] == '\0' || (p[0] == '/' && p[1] == '\0'))
    {
        return NX_SUCCESS;
    }

    /* Skip leading slashes. */
    while (*p == '/')
    {
        p++;
    }

    while (*p != '\0')
    {
        CHAR segment[FX_MAX_LONG_NAME_LEN + 1];
        UINT seg_len = 0;

        /* Collect the next path component until '/' or end. */
        while ((*p != '\0') && (*p != '/') && (seg_len < FX_MAX_LONG_NAME_LEN))
        {
            segment[seg_len++] = *p++;
        }
        segment[seg_len] = '\0';

        if (seg_len > 0)
        {
            /* Go one level deeper (relative to current default dir). */
            status = fx_directory_default_set(g_usb_media, segment);
            if (status != FX_SUCCESS)
            {
                return status;
            }
        }

        /* Skip one or more '/' between components. */
        while (*p == '/')
        {
            p++;
        }
    }

    return NX_SUCCESS;
}

static UINT http_handle_upload_stream(NX_HTTP_SERVER * server_ptr,
                                      CHAR           * resource,
                                      NX_PACKET      * packet_ptr)
{
    if ((g_usb_media == FX_NULL) || (g_usb_media->fx_media_id != FX_MEDIA_ID))
    {
        return http_send_error(server_ptr, "USB not mounted\r\n");
    }

    if (g_upload_active) {
        (void)fx_file_close(&g_upload_file);
        g_upload_active = 0u;
    }


    UINT  fx_status;
    UINT  http_status;

    /* -------------------- 1) Parse query params -------------------- */
    const CHAR * query = strchr(resource, '?');

    /* Defaults */
    CHAR path[MAX_PATH_LEN + 1];
    CHAR name[MAX_NAME_CHARS + 1];
    path[0] = '\0';
    name[0] = '\0';

    CHAR query_buf[200];

    /* NetX may strip query from 'resource'. If so, recover it from the request packet. */
    if (query == NX_NULL)
    {
        if (http_query_from_request_packet(packet_ptr, query_buf, sizeof(query_buf)) == NX_SUCCESS)
        {
            query = query_buf; /* query_buf starts with '?' */
        }
    }

    if (query == NX_NULL)
    {
        return http_send_error(server_ptr, "UploadStream failed (stage 1: missing query)\r\n");
    }

    /* keep your existing query parsing that fills path/name */
    CHAR path_raw[MAX_PATH_LEN + 1];
    CHAR name_raw[MAX_NAME_CHARS + 1];

    path_raw[0] = '\0';
    name_raw[0] = '\0';
    path[0]     = '\0';
    name[0]     = '\0';

    /* path=... */
    {
        const CHAR * p = strstr(query + 1, "path=");
        if (p != NX_NULL)
        {
            p += 5; /* skip "path=" */
            UINT i = 0u;
            while (*p && (*p != '&') && (i < MAX_PATH_LEN))
            {
                path_raw[i++] = *p++;
            }
            path_raw[i] = '\0';
            url_decode(path_raw, path, sizeof(path));
        }
    }

    /* name=... */
    {
        const CHAR * p = strstr(query + 1, "name=");
        if (p != NX_NULL)
        {
            p += 5; /* skip "name=" */
            UINT i = 0u;
            while (*p && (*p != '&') && (i < MAX_NAME_CHARS))
            {
                name_raw[i++] = *p++;
            }
            name_raw[i] = '\0';
            url_decode(name_raw, name, sizeof(name));
        }
    }

    if (name[0] == '\0')
    {
        return http_send_error(server_ptr, "UploadStream failed (stage 1: missing name)\r\n");
    }

    /* Basic safety: name must be a plain filename. */
    if (strchr(name, '/') || strchr(name, '\\') || strstr(name, ".."))
    {
        return http_send_error(server_ptr, "UploadStream failed (stage 1: bad name)\r\n");
    }

    if (path[0] == '\0')
    {
        /* If client omitted path, default to root. */
        strcpy(path, "/");
    }

    /* -------------------- 2) Select target directory -------------------- */
    fx_status = filex_set_directory_from_url_path(path);
    if (fx_status != FX_SUCCESS)
    {
        CHAR msg[160];
        snprintf(msg, sizeof(msg),
                 "UploadStream failed (stage 2: bad path \"%s\", fx=0x%04X)\r\n",
                 path, fx_status);
        return http_send_error(server_ptr, msg);
    }

    /* -------------------- 3) Create/truncate file -------------------- */
    fx_status = fx_file_create(g_usb_media, name);
    if (fx_status == FX_ALREADY_CREATED)
    {
        (void) fx_file_delete(g_usb_media, name);
        fx_status = fx_file_create(g_usb_media, name);
    }

    if (fx_status != FX_SUCCESS)
    {
        fx_directory_default_set(g_usb_media, FX_NULL);
        CHAR msg[160];
        snprintf(msg, sizeof(msg),
                 "UploadStream failed (stage 3: create \"%s\", fx=0x%04X)\r\n",
                 name, fx_status);
        return http_send_error(server_ptr, msg);
    }

    FX_FILE file;
    fx_status = fx_file_open(g_usb_media, &file, name, FX_OPEN_FOR_WRITE);
    if (fx_status != FX_SUCCESS)
    {
        fx_directory_default_set(g_usb_media, FX_NULL);
        CHAR msg[160];
        snprintf(msg, sizeof(msg),
                 "UploadStream failed (stage 4: open \"%s\", fx=0x%04X)\r\n",
                 name, fx_status);
        return http_send_error(server_ptr, msg);
    }

    /* -------------------- 4) Read POST body + write sequentially -------- */
    ULONG content_length = 0u;
    http_status = nx_http_server_content_length_get_extended(packet_ptr, &content_length);
    if (http_status != NX_SUCCESS)
    {
        (void) fx_file_close(&file);
        (void) fx_file_delete(g_usb_media, name);
        fx_directory_default_set(g_usb_media, FX_NULL);
        return http_send_error(server_ptr, "UploadStream failed (stage 5: no content-length)\r\n");
    }

    if (content_length == 0u)
    {
        (void) fx_file_close(&file);
        (void) fx_file_delete(g_usb_media, name);
        fx_directory_default_set(g_usb_media, FX_NULL);
        return http_send_error(server_ptr, "UploadStream failed (stage 5: empty body)\r\n");
    }

    /* IMPORTANT: make this static to avoid blowing the HTTP thread stack. */
    UCHAR *buffer = g_upload_copybuf;

    ULONG body_offset       = 0u;
    ULONG bytes_written     = 0u;
    UINT  timeout_retries   = 0u;

    while (body_offset < content_length)
    {
        ULONG remaining = content_length - body_offset;
        ULONG want      = (remaining > HTTP_UPLOAD_BUFFER_SIZE) ? HTTP_UPLOAD_BUFFER_SIZE : remaining;

        UINT actual = 0u;
        http_status = nx_http_server_content_get_extended(server_ptr,
                                                          packet_ptr,
                                                          body_offset,
                                                          (CHAR *) buffer,
                                                          want,
                                                          &actual);

        if (http_status == NX_HTTP_TIMEOUT)
        {
            /* No new packet yet: wait a bit and retry same offset */
            if (++timeout_retries > UPLOAD_TIMEOUT_MAX_RETRIES)
            {
                (void) fx_file_close(&file);
                (void) fx_file_delete(g_usb_media, name);
                (void) fx_directory_default_set(g_usb_media, FX_NULL);
                return http_send_error(server_ptr, "UploadStream failed (stage 5: timeout)\r\n");
            }

            tx_thread_sleep(UPLOAD_TIMEOUT_SLEEP_TICKS);
            continue;
        }

        timeout_retries = 0u; /* got progress or a real status */

        if ((http_status != NX_SUCCESS) && (http_status != NX_HTTP_DATA_END))
        {
            (void) fx_file_close(&file);
            (void) fx_file_delete(g_usb_media, name);
            (void) fx_directory_default_set(g_usb_media, FX_NULL);
            return http_send_error(server_ptr, "UploadStream failed (stage 5: read error)\r\n");
        }

        if (actual == 0u)
        {
            if (++timeout_retries > UPLOAD_TIMEOUT_MAX_RETRIES)
            {
                (void)fx_file_close(&file);
                (void)fx_file_delete(g_usb_media, name);
                (void)fx_directory_default_set(g_usb_media, FX_NULL);
                return http_send_error(server_ptr, "UploadStream failed (stage 5: stalled)\r\n");
            }
            tx_thread_sleep(UPLOAD_TIMEOUT_SLEEP_TICKS);
            continue;
        }

        fx_status = fx_file_write(&file, buffer, actual);
        if (fx_status != FX_SUCCESS)
        {
            (void) fx_file_close(&file);
            (void) fx_file_delete(g_usb_media, name);
            (void) fx_directory_default_set(g_usb_media, FX_NULL);
            return http_send_error(server_ptr, "UploadStream failed (stage 5: write error)\r\n");
        }

        body_offset   += actual;
        bytes_written += actual;

        if (http_status == NX_HTTP_DATA_END)
        {
            break;
        }
    }

    (void) fx_file_close(&file);

    /* Flush once at end (big speed improvement vs per-chunk flush). */
    (void) fx_media_flush(g_usb_media);

    (void) fx_directory_default_set(g_usb_media, FX_NULL);

    /* -------------------- 5) Send tiny OK response --------------------- */
    CHAR body[128];
    UINT body_len = (UINT) snprintf(body, sizeof(body),
                                    "Upload OK (len=%lu, wrote=%lu)\r\n",
                                    (unsigned long) content_length,
                                    (unsigned long) bytes_written);

    CHAR resp_header[160];
    UINT resp_header_len = (UINT) snprintf(resp_header, sizeof(resp_header),
                                           "HTTP/1.0 200 OK\r\n"
                                           "Content-Type: text/plain\r\n"
                                           "Content-Length: %u\r\n"
                                           "Connection: close\r\n"
                                           "\r\n",
                                           body_len);

    http_send_data(server_ptr, resp_header, resp_header_len);
    http_send_data(server_ptr, body, body_len);

    return NX_HTTP_CALLBACK_COMPLETED;
}


/* Chunked upload handler -------------------------------------------------- */
static UINT http_handle_upload_chunk(NX_HTTP_SERVER * server_ptr,
                                     CHAR           * resource,
                                     NX_PACKET      * packet_ptr)
{
    if ((g_usb_media == FX_NULL) || (g_usb_media->fx_media_id != FX_MEDIA_ID))
    {
        return http_send_error(server_ptr, "USB not mounted\r\n");
    }

    UINT  fx_status;
    UINT  http_status;
    ULONG bytes_written = 0;

    /* --- 0) Check prefix ------------------------------------------------ */
    const CHAR prefix[] = "/upload-chunk";
    UINT       prefix_len = (UINT) strlen(prefix);

    if (strncmp(resource, prefix, prefix_len) != 0)
    {
        return http_send_error(server_ptr, "UploadChunk failed (stage 0: bad URL)\r\n");
    }

    const CHAR * p = resource + prefix_len;   /* points to "" or "/..." */

    if (*p == '/')
    {
        p++;   /* skip leading '/' */
    }

    if (*p == '\0')
    {
        return http_send_error(server_ptr, "UploadChunk failed (stage 0: missing offset)\r\n");
    }

    /* --- 1) Extract offset ---------------------------------------------- */
    CHAR        offset_str[16];
    const CHAR * offset_start = p;
    const CHAR * slash1       = strchr(offset_start, '/');
    if (!slash1)
    {
        return http_send_error(server_ptr, "UploadChunk failed (stage 1: missing eof)\r\n");
    }
    size_t offset_len = (size_t) (slash1 - offset_start);
    if (offset_len >= sizeof(offset_str))
    {
        offset_len = sizeof(offset_str) - 1u;
    }
    memcpy(offset_str, offset_start, offset_len);
    offset_str[offset_len] = '\0';

    ULONG offset = strtoul(offset_str, NX_NULL, 10);

    /* --- 2) Extract eof flag -------------------------------------------- */
    const CHAR * eof_start = slash1 + 1;
    const CHAR * slash2    = strchr(eof_start, '/');
    if (!slash2)
    {
        return http_send_error(server_ptr, "UploadChunk failed (stage 1: missing path/filename)\r\n");
    }

    CHAR   eof_str[8];
    size_t eof_len = (size_t) (slash2 - eof_start);
    if (eof_len >= sizeof(eof_str))
    {
        eof_len = sizeof(eof_str) - 1u;
    }
    memcpy(eof_str, eof_start, eof_len);
    eof_str[eof_len] = '\0';

    UINT eof = (UINT) strtoul(eof_str, NX_NULL, 10);

    /* --- 3) Remaining = path + filename or just filename ---------------- */
    const CHAR * rest = slash2 + 1;
    if (*rest == '\0')
    {
        return http_send_error(server_ptr, "UploadChunk failed (stage 1: missing name)\r\n");
    }

    CHAR path [MAX_PATH_LEN   + 1];
    CHAR name [MAX_NAME_CHARS + 1];

    const CHAR * last_slash = strrchr(rest, '/');
    if (last_slash)
    {
        /* path part */
        size_t path_len = (size_t) (last_slash - rest);
        if (path_len > MAX_PATH_LEN)
        {
            path_len = MAX_PATH_LEN;
        }
        memcpy(path, rest, path_len);
        path[path_len] = '\0';

        /* filename (URL-decoded) */
        url_decode(last_slash + 1, name, sizeof(name));
    }
    else
    {
        path[0] = '\0';
        url_decode(rest, name, sizeof(name));
    }

    if (name[0] == '\0')
    {
        return http_send_error(server_ptr, "UploadChunk failed (stage 1: empty name)\r\n");
    }

    /* --- 4) Select target directory ------------------------------------- */
    if (path[0] == '\0')
    {
        fx_status = fx_directory_default_set(g_usb_media, FX_NULL);   /* root */
    }
    else
    {
        fx_status = fx_directory_default_set(g_usb_media, path);
    }

    if (fx_status != FX_SUCCESS)
    {
        CHAR msg[160];
        snprintf(msg, sizeof(msg),
                 "UploadChunk failed (stage 2: bad path \"%s\", fx=0x%04X)\r\n",
                 path, fx_status);
        return http_send_error(server_ptr, msg);
    }

    /* On first chunk (offset == 0) create/truncate the file. */
    if (offset == 0u)
    {
        fx_status = fx_file_create(g_usb_media, name);
        if (fx_status == FX_ALREADY_CREATED)
        {
            (void) fx_file_delete(g_usb_media, name);
            fx_status = fx_file_create(g_usb_media, name);
        }

        if (fx_status != FX_SUCCESS)
        {
            fx_directory_default_set(g_usb_media, FX_NULL);
            CHAR msg[160];
            snprintf(msg, sizeof(msg),
                     "UploadChunk failed (stage 3: create \"%s\", fx=0x%04X)\r\n",
                     name, fx_status);
            return http_send_error(server_ptr, msg);
        }
    }

    /* --- 5) Open once, then append sequentially ------------------------------ */

    /* First chunk starts a new session. */
    if (offset == 0u)
    {
        /* If a previous upload was left active, close it. */
        if (g_upload_active)
        {
            (void) fx_file_close(&g_upload_file);
            g_upload_active = 0u;
        }

        /* Create/truncate file like you already do (keep your stage 3). */

        /* Open once */
        fx_status = fx_file_open(g_usb_media, &g_upload_file, name, FX_OPEN_FOR_WRITE);
        if (fx_status != FX_SUCCESS)
        {
            fx_directory_default_set(g_usb_media, FX_NULL);
            return http_send_error(server_ptr, "UploadChunk failed (stage 4: open)\r\n");
        }

        g_upload_active = 1u;
        g_upload_next_offset = 0u;
        strncpy(g_upload_path, path, sizeof(g_upload_path) - 1u);
        g_upload_path[sizeof(g_upload_path) - 1u] = '\0';
        strncpy(g_upload_name, name, sizeof(g_upload_name) - 1u);
        g_upload_name[sizeof(g_upload_name) - 1u] = '\0';
    }
    else
    {
        /* Must match current session */
        if (!g_upload_active ||
            (strcmp(g_upload_name, name) != 0) ||
            (strcmp(g_upload_path, path) != 0) ||
            (offset != g_upload_next_offset))
        {
            fx_directory_default_set(g_usb_media, FX_NULL);
            return http_send_error(server_ptr, "UploadChunk failed: out-of-order chunk\r\n");
        }
    }

    /* --- 6) Read ONLY the POST body using NetX HTTP API ----------------- */
    ULONG content_length = 0;
    http_status = nx_http_server_content_length_get_extended(packet_ptr, &content_length);
    if (http_status != NX_SUCCESS)
    {
        fx_file_close(&g_upload_file);
        fx_directory_default_set(g_usb_media, FX_NULL);
        return http_send_error(server_ptr, "UploadChunk failed (stage 5: no content-length)\r\n");
    }

    if ((content_length == 0u) || (content_length > HTTP_UPLOAD_CHUNK_MAX_SIZE))
    {
        fx_file_close(&g_upload_file);
        fx_directory_default_set(g_usb_media, FX_NULL);
        return http_send_error(server_ptr, "UploadChunk failed (stage 5: bad content length)\r\n");
    }

    UCHAR * buffer = g_upload_copybuf;
    ULONG body_offset   = 0;
    bytes_written       = 0;

    while (body_offset < content_length)
    {
        ULONG remaining = content_length - body_offset;
        ULONG want      = (remaining > HTTP_UPLOAD_BUFFER_SIZE)
                          ? HTTP_UPLOAD_BUFFER_SIZE
                          : remaining;

        UINT actual = 0u;
        http_status = nx_http_server_content_get_extended(server_ptr,
                                                          packet_ptr,
                                                          body_offset,
                                                          (CHAR *) buffer,
                                                          want,
                                                          &actual);
        if ((http_status != NX_SUCCESS) && (http_status != NX_HTTP_DATA_END))
        {
            fx_file_close(&g_upload_file);
            fx_directory_default_set(g_usb_media, FX_NULL);
            return http_send_error(server_ptr, "UploadChunk failed (stage 5: read error)\r\n");
        }

        if (actual == 0u)
        {
            break;
        }

        fx_status = fx_file_write(&g_upload_file, buffer, actual);
        if (fx_status != FX_SUCCESS)
        {
            fx_file_close(&g_upload_file);
            fx_directory_default_set(g_usb_media, FX_NULL);
            return http_send_error(server_ptr, "UploadChunk failed (stage 5: write error)\r\n");
        }

        body_offset   += actual;
        bytes_written += actual;

        if (http_status == NX_HTTP_DATA_END)
        {
            break;
        }
    }
    g_upload_next_offset += bytes_written;

    if (eof == 1u && g_upload_active)
    {
        (void) fx_file_close(&g_upload_file);
        (void) fx_media_flush(g_usb_media);
        g_upload_active = 0u;
    }
    (void) fx_directory_default_set(g_usb_media, FX_NULL);

    /* --- 7) Send tiny OK response --------------------------------------- */
    CHAR body[128];
    UINT body_len = (UINT) snprintf(body, sizeof(body),
                                    "Chunk OK (offset=%lu, len=%lu, eof=%u)\r\n",
                                    offset, bytes_written, eof);

    CHAR resp_header[160];
    UINT resp_header_len = (UINT) snprintf(resp_header, sizeof(resp_header),
                                           "HTTP/1.0 200 OK\r\n"
                                           "Content-Type: text/plain\r\n"
                                           "Content-Length: %u\r\n"
                                           "Connection: %s\r\n"
                                           "\r\n",
                                           body_len, eof != 1u ? "keep-alive" :  "close");

    http_send_data(server_ptr, resp_header, resp_header_len);
    http_send_data(server_ptr, body, body_len);

    return NX_HTTP_CALLBACK_COMPLETED;
}

/* Send JSON array describing the contents of a directory on USB.
 * URL path examples:
 *   "/"           -> root
 *   "/UI/"        -> directory "UI" at root
 *   "/UI/Logs"    -> "Logs" subfolder inside "UI"
 */
static UINT http_send_json_directory(NX_HTTP_SERVER * server_ptr, const CHAR * url_path)
{
    if ((g_usb_media == FX_NULL) || (g_usb_media->fx_media_id != FX_MEDIA_ID))
    {
        return http_send_error(server_ptr, "USB not mounted\r\n");
    }

    /* Set FileX default dir according to URL path. */
    UINT fx_status = filex_set_directory_from_url_path(url_path);
    if (fx_status != NX_SUCCESS)
    {
        return http_send_error(server_ptr, "Invalid path\r\n");
    }

    /* HTTP header for streamed JSON. */
    CHAR header[128];
    UINT header_len = (UINT) snprintf(header, sizeof(header),
                                      "HTTP/1.0 200 OK\r\n"
                                      "Content-Type: application/json\r\n"
                                      "Connection: close\r\n"
                                      "\r\n");

    if ((INT)header_len < 0) header_len = 0;
    if (header_len >= sizeof(header)) header_len = sizeof(header) - 1;

    http_send_data(server_ptr, header, header_len);

    /* JSON object start: {"path":"...","entries":[ */
    CHAR safe_path[MAX_PATH_LEN + 1];
    {
        UINT i = 0;
        for (; url_path[i] != '\0' && i < (sizeof(safe_path) - 1); i++)
        {
            CHAR c = url_path[i];
            safe_path[i] = (c == '\"' || c == '\\') ? '_' : c;
        }
        safe_path[i] = '\0';
    }

    CHAR json_prefix[256];
    UINT json_prefix_len = (UINT) snprintf(json_prefix, sizeof(json_prefix),
                                           "{\"path\":\"%s\",\"entries\":[",
                                           safe_path[0] ? safe_path : "/");
    if (json_prefix_len > sizeof(json_prefix)) json_prefix_len = sizeof(json_prefix);
    http_send_data(server_ptr, json_prefix, json_prefix_len);

    CHAR  entry_name[FX_MAX_LONG_NAME_LEN + 1];
    UINT  attributes = 0;
    ULONG size       = 0;
    UINT  first      = 1;

    fx_status = fx_directory_first_entry_find(g_usb_media, entry_name);
    while (fx_status == FX_SUCCESS)
    {
        if ((entry_name[0] != '\0') &&
            (strcmp(entry_name, ".") != 0) &&
            (strcmp(entry_name, "..") != 0) &&
            (strcmp(entry_name, "app.html") != 0) &&
            (strcmp(entry_name, "index.html") != 0) &&
            (strcmp(entry_name, "file_manager.js") != 0) &&
            (strcmp(entry_name, "System Volume Information") != 0))
        {
            attributes = 0;
            size       = 0;

            (void) fx_directory_information_get(g_usb_media,
                                                entry_name,
                                                &attributes,
                                                &size,
                                                FX_NULL, FX_NULL, FX_NULL,
                                                FX_NULL, FX_NULL, FX_NULL);

            /* IMPORTANT: JS expects lowercase "dir"/"file" */
            const CHAR * type_str = (attributes & FX_DIRECTORY) ? "dir" : "file";

            /* JSON-safe name (basic escaping). */
            CHAR safe_name[FX_MAX_LONG_NAME_LEN + 1];
            UINT i = 0;
            for (; entry_name[i] != '\0' && i < (sizeof(safe_name) - 1); i++)
            {
                CHAR c = entry_name[i];
                safe_name[i] = (c == '\"' || c == '\\') ? '_' : c;
            }
            safe_name[i] = '\0';

            CHAR item_buf[256];
            UINT item_len = (UINT) snprintf(item_buf, sizeof(item_buf),
                                            "%s{\"name\":\"%s\",\"type\":\"%s\",\"size\":%lu}",
                                            first ? "" : ",",
                                            safe_name,
                                            type_str,
                                            (unsigned long) size);
            if (item_len > sizeof(item_buf))
            {
                item_len = sizeof(item_buf);
            }

            http_send_data(server_ptr, item_buf, item_len);
            first = 0;
        }

        fx_status = fx_directory_next_entry_find(g_usb_media, entry_name);
    }

    /* JSON object end */
    static const CHAR json_end[] = "]}\r\n";
    http_send_data(server_ptr, json_end, (UINT) strlen(json_end));

    /* Go back to root for safety. */
    (void) fx_directory_default_set(g_usb_media, FX_NULL);

    return NX_HTTP_CALLBACK_COMPLETED;
}




/* Render a small login page (GET /login or failed login) */
static UINT render_login_form(NX_HTTP_SERVER *server_ptr, const CHAR *message)
{
    /* ---------------- HTTP header ---------------- */
    CHAR header[128];
    UINT header_len = (UINT) snprintf(header, sizeof(header),
                                      "HTTP/1.0 200 OK\r\n"
                                      "Content-Type: text/html\r\n"
                                      "Connection: close\r\n"
                                      "\r\n");
    if (header_len > sizeof(header))
    {
        header_len = sizeof(header);
    }
    http_send_data(server_ptr, header, header_len);

    /* ---------------- HTML body ------------------ */
    const CHAR * msg = (message && message[0]) ? message : "";

    /* Split in prefix + message + suffix to avoid a huge snprintf. */
    static const CHAR body_prefix[] =
        "<!DOCTYPE html>\r\n"
        "<html><head><title>Login</title></head>\r\n"
        "<body style=\"font-family:sans-serif;background:#f4f4f9;\">\r\n"
        "<div style=\"max-width:400px;margin:40px auto;padding:20px;background:#fff;"
        "border-radius:8px;box-shadow:0 2px 4px rgba(0,0,0,0.1);\">\r\n"
        "<h2>Login</h2>\r\n"
        "<p style=\"color:#c00;\">";

    static const CHAR body_suffix[] =
        "</p>\r\n"
        "<form method=\"POST\" action=\"/login\">\r\n"
        "  <div>Username:<br><input name=\"username\" style=\"width:100%%;\"></div>\r\n"
        "  <div style=\"margin-top:8px;\">Password:<br>"
        "      <input type=\"password\" name=\"password\" style=\"width:100%%;\"></div>\r\n"
        "  <div style=\"margin-top:12px;\">"
        "      <button type=\"submit\">Login</button>"
        "      <a href=\"/\" style=\"margin-left:8px;\">Cancel</a>"
        "  </div>\r\n"
        "</form>\r\n"
        "</div></body></html>\r\n";

    http_send_data(server_ptr, body_prefix, (UINT)strlen(body_prefix));
    http_send_data(server_ptr, msg, (UINT)strlen(msg));
    http_send_data(server_ptr, body_suffix, (UINT)strlen(body_suffix));

    return NX_HTTP_CALLBACK_COMPLETED;
}


/* Very small helper: extract value of key "username=" or "password=" from form body */
/* Extract value of key "username=" or "password=" from form body. */
static void form_get_value(const CHAR * body, const CHAR * key, CHAR * out, UINT out_size)
{
    const CHAR * p = strstr(body, key);
    UINT i = 0;

    if (!p || out_size == 0)
    {
        if (out_size > 0) { out[0] = '\0'; }
        return;
    }

    p += strlen(key);  /* skip "username=" or "password=" */

    while (*p && *p != '&' && i < (out_size - 1))
    {
        if (*p == '+')
        {
            out[i++] = ' ';
        }
        else
        {
            out[i++] = *p;
        }
        p++;
    }
    out[i] = '\0';
}

/* GET /api/auth_check : 200 if logged in, 401 otherwise. */
static UINT http_handle_auth_check(NX_HTTP_SERVER * server_ptr)
{
    if (!g_logged_in)
    {
        static const CHAR body[] = "Unauthorized";
        CHAR header[128];
        UINT header_len = (UINT) snprintf(header, sizeof(header),
                                          "HTTP/1.0 401 Unauthorized\r\n"
                                          "Content-Type: text/plain\r\n"
                                          "Content-Length: %u\r\n"
                                          "Connection: close\r\n"
                                          "\r\n",
                                          (unsigned) strlen(body));
        if (header_len > sizeof(header))
        {
            header_len = sizeof(header);
        }

        http_send_data(server_ptr, header, header_len);
        http_send_data(server_ptr, body, (UINT) strlen(body));
        return NX_HTTP_CALLBACK_COMPLETED;
    }
    else
    {
        static const CHAR json[] = "{\"status\":\"ok\"}";
        CHAR header[128];
        UINT header_len = (UINT) snprintf(header, sizeof(header),
                                          "HTTP/1.0 200 OK\r\n"
                                          "Content-Type: application/json\r\n"
                                          "Content-Length: %u\r\n"
                                          "Connection: close\r\n"
                                          "\r\n",
                                          (unsigned) strlen(json));
        if (header_len > sizeof(header))
        {
            header_len = sizeof(header);
        }

        http_send_data(server_ptr, header, header_len);
        http_send_data(server_ptr, json, (UINT) strlen(json));
        return NX_HTTP_CALLBACK_COMPLETED;
    }
}

/* POST /api/logout : clear login flag. */
static UINT http_handle_logout(NX_HTTP_SERVER * server_ptr)
{
    g_logged_in = 0u;

    static const CHAR json[] = "{\"status\":\"logged_out\"}";
    CHAR header[128];
    UINT header_len = (UINT) snprintf(header, sizeof(header),
                                      "HTTP/1.0 200 OK\r\n"
                                      "Content-Type: application/json\r\n"
                                      "Content-Length: %u\r\n"
                                      "Connection: close\r\n"
                                      "\r\n",
                                      (unsigned) strlen(json));
    if (header_len > sizeof(header))
    {
        header_len = sizeof(header);
    }

    http_send_data(server_ptr, header, header_len);
    http_send_data(server_ptr, json, (UINT) strlen(json));

    return NX_HTTP_CALLBACK_COMPLETED;
}



/* Handle POST /api/login */
static UINT http_handle_login(NX_HTTP_SERVER * server_ptr, NX_PACKET * packet_ptr)
{
    ULONG content_length = 0;
    UINT  status;

    status = nx_http_server_content_length_get_extended(packet_ptr, &content_length);
    if ((status != NX_SUCCESS) || (content_length == 0u) || (content_length > 256u))
    {
        /* Bad form data -> just go back to login page. */
        return http_send_redirect(server_ptr, "/index.html");
    }

    CHAR body[257];
    ULONG offset = 0;
    ULONG total  = 0;

    while ((offset < content_length) && (total < (sizeof(body) - 1u)))
    {
        ULONG to_read = content_length - offset;
        if (to_read > (sizeof(body) - 1u - total))
        {
            to_read = sizeof(body) - 1u - total;
        }

        UINT actual = 0u;
        status = nx_http_server_content_get_extended(server_ptr,
                                                     packet_ptr,
                                                     offset,
                                                     &body[total],
                                                     to_read,
                                                     &actual);
        if ((status != NX_SUCCESS) && (status != NX_HTTP_DATA_END))
        {
            return http_send_redirect(server_ptr, "/index.html");
        }

        if (actual == 0u)
        {
            break;
        }

        offset += actual;
        total  += actual;

        if (status == NX_HTTP_DATA_END)
        {
            break;
        }
    }

    body[total] = '\0';

    CHAR username[32];
    CHAR password[32];

    form_get_value(body, "username=", username, sizeof(username));
    form_get_value(body, "password=", password, sizeof(password));

    /* Use QSPI-backed auth if available.
     * Provisioning rule: if no password record exists yet, the first successful login SETS the password in QSPI.
     */
    int init_rc = auth_init();
    int prov_rc = auth_is_provisioned();

    if (init_rc == 0 && prov_rc == 0)
    {
        /* First-time provisioning: username must be admin, password becomes the new stored password. */
        if ((strcmp(username, ADMIN_USERNAME) == 0) && (password[0] != '\0'))
        {
            if (auth_set_admin_password(password) == 0)
            {
                g_logged_in = 1u;
                return http_send_redirect(server_ptr, "/app.html");
            }
        }

        g_logged_in = 0u;
        return http_send_redirect(server_ptr, "/index.html");
    }
    else if (init_rc == 0 && prov_rc > 0)
    {
        int ok = auth_verify_credentials(username, password);
        if (ok == 1)
        {
            g_logged_in = 1u;
            return http_send_redirect(server_ptr, "/app.html");
        }

        g_logged_in = 0u;
        return http_send_redirect(server_ptr, "/index.html");
    }
    else
    {
        /* Auth backend unavailable -> fallback to firmware default password (optional).
         * If you don't want this fallback, delete this branch and always fail.
         */
        if ((strcmp(username, ADMIN_USERNAME) == 0) && (strcmp(password, ADMIN_DEFAULT_PASSWORD) == 0))
        {
            g_logged_in = 1u;
            return http_send_redirect(server_ptr, "/app.html");
        }

        g_logged_in = 0u;
        return http_send_redirect(server_ptr, "/index.html");
    }
}

/* ------------------------ HTTP header parsing (from packet) ------------------------ */

static int http_ascii_tolower(int c)
{
    if ((c >= 'A') && (c <= 'Z')) return (c - 'A' + 'a');
    return c;
}

static int http_ascii_strncasecmp(const CHAR *a, const CHAR *b, UINT n)
{
    for (UINT i = 0; i < n; i++)
    {
        CHAR ca = a[i];
        CHAR cb = b[i];
        if (ca == '\0' || cb == '\0') return (ca == cb) ? 0 : (ca ? 1 : -1);
        int da = http_ascii_tolower((int)ca);
        int db = http_ascii_tolower((int)cb);
        if (da != db) return (da < db) ? -1 : 1;
    }
    return 0;
}

/* Extract header value from the raw HTTP request stored in packet_ptr.
   Example: key="Range:" -> returns "bytes=0-1023" (without leading spaces). */
static UINT http_header_value_from_packet(NX_PACKET *packet_ptr,
                                          const CHAR *key,
                                          CHAR *out,
                                          UINT out_len)
{
    if (!packet_ptr || !key || !out || out_len < 2u) return NX_NOT_SUCCESSFUL;

    out[0] = '\0';

    /* Grab first bytes; request line + headers should be here. */
    CHAR  buf[1024];
    ULONG copied = 0;
    if (nx_packet_data_extract_offset(packet_ptr, 0u, buf, (ULONG)(sizeof(buf) - 1u), &copied) != NX_SUCCESS)
        return NX_NOT_SUCCESSFUL;

    buf[copied] = '\0';

    /* Scan line by line */
    const UINT key_len = (UINT)strlen(key);
    CHAR *p = buf;

    while (*p)
    {
        /* Find end of line */
        CHAR *eol = strstr(p, "\r\n");
        if (!eol) break;

        /* Empty line => end of headers */
        if (eol == p) break;

        /* Does this line start with key? (case-insensitive) */
        if ((UINT)(eol - p) >= key_len && http_ascii_strncasecmp(p, key, key_len) == 0)
        {
            CHAR *v = p + key_len;
            while (*v == ' ' || *v == '\t') v++;

            /* Copy value */
            UINT i = 0u;
            while (v[i] && (v + i) < eol && i < (out_len - 1u))
            {
                out[i] = v[i];
                i++;
            }
            out[i] = '\0';
            return NX_SUCCESS;
        }

        p = eol + 2; /* next line */
    }

    return NX_NOT_SUCCESSFUL;
}

/* Parse "bytes=start-end" or "bytes=start-" or "bytes=-suffix" (suffix not implemented here).
   Returns NX_SUCCESS if a valid range is found and outputs start/end inclusive. */
static UINT http_parse_range_bytes(const CHAR *range_val,
                                   ULONG file_size,
                                   ULONG *out_start,
                                   ULONG *out_end)
{
    if (!range_val || !out_start || !out_end) return NX_NOT_SUCCESSFUL;

    /* Expect "bytes=" */
    const CHAR *p = range_val;
    if (http_ascii_strncasecmp(p, "bytes=", 6u) != 0) return NX_NOT_SUCCESSFUL;
    p += 6;

    /* We implement: start-end OR start-  (no multi-range, no suffix-range) */
    if (*p == '-') return NX_NOT_SUCCESSFUL; /* suffix form "-N" not supported */

    /* Parse start */
    unsigned long long start = 0ULL;
    if (*p < '0' || *p > '9') return NX_NOT_SUCCESSFUL;
    while (*p >= '0' && *p <= '9')
    {
        start = start * 10ULL + (unsigned)(*p - '0');
        p++;
    }

    if (*p != '-') return NX_NOT_SUCCESSFUL;
    p++;

    unsigned long long end = 0ULL;
    if (*p == '\0')
    {
        /* "start-" means until EOF */
        end = (file_size > 0u) ? (unsigned long long)(file_size - 1u) : 0ULL;
    }
    else
    {
        if (*p < '0' || *p > '9') return NX_NOT_SUCCESSFUL;
        while (*p >= '0' && *p <= '9')
        {
            end = end * 10ULL + (unsigned)(*p - '0');
            p++;
        }
    }

    if (start >= (unsigned long long)file_size) return NX_NOT_SUCCESSFUL;
    if (end   >= (unsigned long long)file_size) end = (unsigned long long)(file_size - 1u);
    if (end < start) return NX_NOT_SUCCESSFUL;

    *out_start = (ULONG)start;
    *out_end   = (ULONG)end;
    return NX_SUCCESS;
}


/* Send a file from USB as a download (binary stream). */
static UINT http_handle_download(NX_HTTP_SERVER * server_ptr,
                                 const CHAR     * file_path,
                                 NX_PACKET      * packet_ptr)
{
    if ((g_usb_media == FX_NULL) || (g_usb_media->fx_media_id != FX_MEDIA_ID))
    {
        return http_send_error(server_ptr, "USB not mounted\r\n");
    }

    if ((file_path == NX_NULL) || (file_path[0] == '\0') || (strcmp(file_path, "/") == 0))
    {
        return http_send_error(server_ptr, "No file specified\r\n");
    }

    /* ---- Split directory + filename from URL path (e.g. "/UI/a.mp4") ---- */
    CHAR dir_path[MAX_PATH_LEN + 1];
    dir_path[0] = '\0';

    const CHAR *name = http_basename(file_path);
    if ((name == NX_NULL) || (name[0] == '\0'))
    {
        return http_send_error(server_ptr, "Bad file path\r\n");
    }

    /* Build dir path (everything before last '/') */
    {
        const CHAR *last = strrchr(file_path, '/');
        if (last == NX_NULL || last == file_path)
        {
            /* root directory */
            strcpy(dir_path, "/");
        }
        else
        {
            UINT len = (UINT)(last - file_path);
            if (len > MAX_PATH_LEN) len = MAX_PATH_LEN;
            memcpy(dir_path, file_path, len);
            dir_path[len] = '\0';
        }
    }

    /* Set FileX current directory */
    UINT fx_status = filex_set_directory_from_url_path(dir_path);
    if (fx_status != FX_SUCCESS)
    {
        CHAR msg[160];
        snprintf(msg, sizeof(msg),
                 "Download failed: bad path \"%s\" (fx=0x%04X)\r\n",
                 dir_path, fx_status);
        return http_send_error(server_ptr, msg);
    }

    /* ---- Open file ---- */
    FX_FILE file;
    fx_status = fx_file_open(g_usb_media, &file, (CHAR *)name, FX_OPEN_FOR_READ);
    if (fx_status != FX_SUCCESS)
    {
        fx_directory_default_set(g_usb_media, FX_NULL);
        return http_send_error(server_ptr, "Download failed: file open\r\n");
    }

    /* ---- Get file size ---- */
    /* FileX SSP 2.7.0: size is available in the FX_FILE control block after open */
    ULONG file_size = (ULONG)file.fx_file_current_file_size;

    if (file_size == 0u)
    {
        /* Send headers (200 OK, length 0) then close */
    }


    /* ---- Range header (optional) ---- */
    CHAR range_val[80];
    UINT has_range = 0u;
    ULONG range_start = 0u;
    ULONG range_end   = 0u;

    if (http_header_value_from_packet(packet_ptr, "Range:", range_val, sizeof(range_val)) == NX_SUCCESS)
    {
        if (http_parse_range_bytes(range_val, file_size, &range_start, &range_end) == NX_SUCCESS)
        {
            has_range = 1u;
        }
        else
        {
            /* Invalid range -> 416 */
            const unsigned long long total = (unsigned long long)file_size;

            CHAR hdr[200];
            UINT hdr_len = (UINT)snprintf(hdr, sizeof(hdr),
                "HTTP/1.1 416 Range Not Satisfiable\r\n"
                "Content-Range: bytes */%llu\r\n"
                "Connection: close\r\n"
                "\r\n",
                total);

            if (hdr_len >= sizeof(hdr)) hdr_len = (UINT)(sizeof(hdr) - 1u);
            http_send_data(server_ptr, hdr, hdr_len);

            (void)fx_file_close(&file);
            fx_directory_default_set(g_usb_media, FX_NULL);
            return NX_HTTP_CALLBACK_COMPLETED;
        }
    }

    /* ---- Decide mime + disposition ---- */
    /* Use full path for mime sniffing (extension is enough) */
    const CHAR *mime = http_guess_mime(file_path);
    const CHAR *disp = http_is_inline_type(mime) ? "inline" : "attachment";

    /* ---- Build and send HTTP headers ---- */
    ULONG send_start = 0u;
    ULONG send_end   = (file_size > 0u) ? (file_size - 1u) : 0u;

    CHAR header[360];
    UINT header_len = 0u;

    if (has_range)
    {
        send_start = range_start;
        send_end   = range_end;

        const unsigned long long total = (unsigned long long)file_size;
        const unsigned long long s = (unsigned long long)send_start;
        const unsigned long long e = (unsigned long long)send_end;
        const unsigned long long clen = (e - s + 1ULL);

        header_len = (UINT)snprintf(header, sizeof(header),
            "HTTP/1.1 206 Partial Content\r\n"
            "Content-Type: %s\r\n"
            "Content-Disposition: %s; filename=\"%s\"\r\n"
            "Accept-Ranges: bytes\r\n"
            "Content-Range: bytes %llu-%llu/%llu\r\n"
            "Content-Length: %llu\r\n"
            "Connection: close\r\n"
            "\r\n",
            mime, disp, name, s, e, total, clen);
    }
    else
    {
        header_len = (UINT)snprintf(header, sizeof(header),
            "HTTP/1.0 200 OK\r\n"
            "Content-Type: %s\r\n"
            "Content-Disposition: %s; filename=\"%s\"\r\n"
            "Accept-Ranges: bytes\r\n"
            "Content-Length: %u\r\n"
            "Connection: close\r\n"
            "\r\n",
            mime, disp, name, (unsigned)file_size);
    }

    if (header_len >= sizeof(header)) header_len = (UINT)(sizeof(header) - 1u);
    http_send_data(server_ptr, header, header_len);

    /* ---- Seek to start position ---- */
    fx_status = fx_file_seek(&file, send_start);
    if (fx_status != FX_SUCCESS)
    {
        (void)fx_file_close(&file);
        fx_directory_default_set(g_usb_media, FX_NULL);
        return http_send_error(server_ptr, "Download failed: seek\r\n");
    }

    /* ---- Stream payload ---- */
    static UCHAR g_download_buf[2048u];   /* keep off stack */
    ULONG remaining = 0u;

    if (file_size == 0u)
    {
        remaining = 0u;
    }
    else if (send_end >= send_start)
    {
        remaining = (send_end - send_start + 1u);
    }

    while (remaining > 0u)
    {
        ULONG want = remaining;
        if (want > (ULONG)sizeof(g_download_buf))
        {
            want = (ULONG)sizeof(g_download_buf);
        }

        ULONG actual = 0u;
        fx_status = fx_file_read(&file, g_download_buf, want, &actual);
        if ((fx_status != FX_SUCCESS) || (actual == 0u))
        {
            break;
        }

        http_send_data(server_ptr, (CHAR *)g_download_buf, (UINT)actual);
        remaining -= actual;
    }

    (void)fx_file_close(&file);
    fx_directory_default_set(g_usb_media, FX_NULL);
    return NX_HTTP_CALLBACK_COMPLETED;
}


/* Delete a file or (empty) directory on USB. */
static UINT http_handle_delete(NX_HTTP_SERVER * server_ptr, const CHAR * url_path_param)
{
    if ((g_usb_media == FX_NULL) || (g_usb_media->fx_media_id != FX_MEDIA_ID))
    {
        return http_send_error(server_ptr, "USB not mounted\r\n");
    }

    CHAR decoded[MAX_PATH_LEN + 1];
    CHAR cleaned[MAX_PATH_LEN + 1];
    CHAR parent_url[MAX_PATH_LEN + 1];
    CHAR name_only[FX_MAX_LONG_NAME_LEN + 1];

    /* Decode URL path, e.g. "/UI/rsc/emptyfolder" or "/usb_test.txt" */
    url_decode(url_path_param ? url_path_param : "", decoded, sizeof(decoded));

    if (decoded[0] == '\0' || strcmp(decoded, "/") == 0)
    {
        return http_send_error(server_ptr, "No valid path specified\r\n");
    }

    /* Copy and strip a trailing '/' if present (but keep leading '/'). */
    UINT len = 0;
    while ((len < MAX_PATH_LEN) && (decoded[len] != '\0'))
    {
        cleaned[len] = decoded[len];
        len++;
    }
    if (len > 1 && cleaned[len - 1] == '/')
    {
        len--;
    }
    cleaned[len] = '\0';

    /* Split cleaned into parent_url + name_only. */
    CHAR * last_slash = strrchr(cleaned, '/');
    if (last_slash == NULL)
    {
        /* No slash at all: item directly under root, e.g. "usb_test.txt". */
        strcpy(parent_url, "/");
        strncpy(name_only, cleaned, FX_MAX_LONG_NAME_LEN);
        name_only[FX_MAX_LONG_NAME_LEN] = '\0';
    }
    else if (last_slash == cleaned)
    {
        /* Path like "/usb_test.txt". */
        strcpy(parent_url, "/");
        strncpy(name_only, last_slash + 1, FX_MAX_LONG_NAME_LEN);
        name_only[FX_MAX_LONG_NAME_LEN] = '\0';
    }
    else
    {
        /* Path like "/UI/emptyfolder" or "/UI/rsc/foo". */
        size_t parent_len = (size_t)(last_slash - cleaned);
        if (parent_len > MAX_PATH_LEN)
        {
            parent_len = MAX_PATH_LEN;
        }
        memcpy(parent_url, cleaned, parent_len);
        parent_url[parent_len] = '\0';

        strncpy(name_only, last_slash + 1, FX_MAX_LONG_NAME_LEN);
        name_only[FX_MAX_LONG_NAME_LEN] = '\0';
    }

    if (name_only[0] == '\0')
    {
        return http_send_error(server_ptr, "Invalid name\r\n");
    }

    /* Get attributes (file vs directory) relative to parent. */
    UINT  attributes = 0;
    ULONG size       = 0;
    UINT  fx_status;

    /* Set default dir to parent. */
    fx_status = filex_set_directory_from_url_path(parent_url);
    if (fx_status != NX_SUCCESS)
    {
        return http_send_error(server_ptr, "Cannot change directory\r\n");
    }

    fx_status = fx_directory_information_get(g_usb_media,
                                             name_only,
                                             &attributes,
                                             &size,
                                             FX_NULL, FX_NULL, FX_NULL,
                                             FX_NULL, FX_NULL, FX_NULL);
    if (fx_status != FX_SUCCESS)
    {
        (void) fx_directory_default_set(g_usb_media, FX_NULL);
        return http_send_error(server_ptr, "Path not found\r\n");
    }

    /* Delete either file or (empty) directory. */
    if (attributes & FX_DIRECTORY)
    {
        fx_status = fx_directory_delete(g_usb_media, name_only);
    }
    else
    {
        fx_status = fx_file_delete(g_usb_media, name_only);
    }

    (void) fx_directory_default_set(g_usb_media, FX_NULL);

    if (fx_status != FX_SUCCESS)
    {
        CHAR msg[64];
        snprintf(msg, sizeof(msg), "Delete failed (0x%04X)\r\n", fx_status);
        msg[sizeof(msg) - 1] = '\0';
        return http_send_error(server_ptr, msg);
    }

    /* Success: 200 OK with small body. */
    static const CHAR ok_body[] = "OK";
    CHAR header[128];
    UINT header_len = (UINT) snprintf(header, sizeof(header),
                                      "HTTP/1.0 200 OK\r\n"
                                      "Content-Type: text/plain\r\n"
                                      "Content-Length: %u\r\n"
                                      "Connection: close\r\n"
                                      "\r\n",
                                      (unsigned) strlen(ok_body));
    if (header_len > sizeof(header))
    {
        header_len = sizeof(header);
    }
    http_send_data(server_ptr, header, header_len);
    http_send_data(server_ptr, ok_body, (UINT) strlen(ok_body));

    return NX_HTTP_CALLBACK_COMPLETED;
}




/* Main NetX HTTP callback ------------------------------------------------- */

UINT http_request_notify(NX_HTTP_SERVER * server_ptr,
                         UINT             request_type,
                         CHAR           * resource,
                         NX_PACKET      * packet_ptr)
{
    if (request_type == NX_HTTP_SERVER_GET_REQUEST)
    {
        /* --- API: auth_check --- */
        if (strncmp(resource, "/api/auth_check", 15) == 0)
        {
            return http_handle_auth_check(server_ptr);
        }


        /* --- API: auth_status --- */
        if (strncmp(resource, "/api/auth_status", 16) == 0)
        {
            /* Try to read if configured (does NOT change login state) */
            UINT configured = (UINT) auth_is_provisioned();
            CHAR body[128];
            UINT body_len = (UINT) snprintf(body, sizeof(body),
                                            "{\"configured\":%s,\"logged_in\":%s}",
                                            configured ? "true" : "false",
                                            g_logged_in ? "true" : "false");

            CHAR header[192];
            UINT header_len = (UINT) snprintf(header, sizeof(header),
                                              "HTTP/1.0 200 OK\r\n"
                                              "Content-Type: application/json\r\n"
                                              "Content-Length: %u\r\n"
                                              "Connection: close\r\n"
                                              "\r\n",
                                              (unsigned) body_len);
            http_send_data(server_ptr, header, header_len);
            http_send_data(server_ptr, body, body_len);
            return NX_HTTP_CALLBACK_COMPLETED;
        }

        /* --- API: files listing --- */
        if (strncmp(resource, "/api/files", 10) == 0)
        {
            if (!g_logged_in)
            {
                return http_handle_auth_check(server_ptr);
            }

            CHAR url_path[MAX_PATH_LEN + 1];
            url_path[0] = '\0';

            /* 1) Try query ?path=... */
            const CHAR * query = strchr(resource, '?');
            if (query != NX_NULL)
            {
                const CHAR * p = strstr(query + 1, "path=");
                if (p != NX_NULL)
                {
                    p += 5; /* skip "path=" */
                    CHAR raw[MAX_PATH_LEN + 1];
                    UINT i = 0;
                    while (*p && *p != '&' && i < MAX_PATH_LEN)
                    {
                        raw[i++] = *p++;
                    }
                    raw[i] = '\0';
                    url_decode(raw, url_path, sizeof(url_path));
                }
            }

            /* 2) Else: accept suffix /api/files/<path> */
            if (url_path[0] == '\0')
            {
                const CHAR * p = resource + 10; /* after "/api/files" */
                if (p[0] == '\0' || p[0] == '?')
                {
                    strcpy(url_path, "/");
                }
                else
                {
                    CHAR raw[MAX_PATH_LEN + 1];
                    UINT i = 0;
                    while (p[i] && p[i] != '?' && i < MAX_PATH_LEN)
                    {
                        raw[i] = p[i];
                        i++;
                    }
                    raw[i] = '\0';
                    url_decode(raw, url_path, sizeof(url_path));
                    if (url_path[0] == '\0') strcpy(url_path, "/");
                }
            }

            return http_send_json_directory(server_ptr, url_path);
        }


        /* --- API: download --- */
        if (strncmp(resource, "/api/download", 13) == 0)
        {
            if (!g_logged_in)
            {
                return http_handle_auth_check(server_ptr);
            }

            CHAR file_path[MAX_PATH_LEN + 1];
            file_path[0] = '\0';

            /* 1) Try query ?file=... (old mode) */
            const CHAR * query = strchr(resource, '?');
            if (query != NX_NULL)
            {
                const CHAR * p = strstr(query + 1, "file=");
                if (p != NX_NULL)
                {
                    p += 5; /* skip "file=" */
                    CHAR raw[MAX_PATH_LEN + 1];
                    UINT i = 0;
                    while (*p && *p != '&' && i < MAX_PATH_LEN)
                    {
                        raw[i++] = *p++;
                    }
                    raw[i] = '\0';
                    url_decode(raw, file_path, sizeof(file_path));
                }
            }

            /* 2) Else: accept suffix /api/download/<path> (JS mode) */
            if (file_path[0] == '\0')
            {
                const CHAR * p = resource + 13; /* after "/api/download" */
                if (p[0] == '\0' || p[0] == '?')
                {
                    return http_send_error(server_ptr, "No file specified\r\n");
                }

                CHAR raw[MAX_PATH_LEN + 1];
                UINT i = 0;
                while (p[i] && p[i] != '?' && i < MAX_PATH_LEN)
                {
                    raw[i] = p[i];
                    i++;
                }
                raw[i] = '\0';
                url_decode(raw, file_path, sizeof(file_path));
            }

            if (file_path[0] == '\0')
            {
                return http_send_error(server_ptr, "No file specified\r\n");
            }

            return http_handle_download(server_ptr, file_path, packet_ptr);
        }

        /* ---- Serve frontend files from USB root ---- */

        if ((strcmp(resource, "/index.html") == 0) ||
            (strcmp(resource, "/index") == 0))
        {
            /* index.html located at USB root */
            return http_send_static_file(server_ptr, "index.html");
        }

        if ((strcmp(resource, "/app.html") == 0) ||
            (strcmp(resource, "/app") == 0))
        {
            if (!g_logged_in)
            {
                return http_send_static_file(server_ptr, "index.html"); // or http_handle_auth_check
            }
            return http_send_static_file(server_ptr, "app.html");
        }


        if (strcmp(resource, "/file_manager.js") == 0)
        {
            /* JS located at USB root */
            return http_send_static_file(server_ptr, "file_manager.js");
        }

        /* ---- Existing routes follow ---- */
        if (strncmp(resource, "/rsc/", 5) == 0)
        {
            const CHAR * path_on_usb = resource + 1;  /* drop leading '/' */
            return http_send_static_file(server_ptr, path_on_usb);
        }

        if (strcmp(resource, "/login") == 0)
        {
            return render_login_form(server_ptr, "");
        }

        if (strcmp(resource, "/logout") == 0)
        {
            g_logged_in = 0;
            return http_send_error(server_ptr, "Logged out.\r\n");
        }

        if (strcmp(resource, "/") == 0)
        {
            /* Keep current directory listing for now */
            return http_send_static_file(server_ptr, "index.html");
        }

        /* --- API: usb stats (used/free/total) --- */
        if (strncmp(resource, "/api/usb_stats", 14) == 0)
        {
            if (!g_logged_in)
            {
                return http_handle_auth_check(server_ptr);
            }
            return http_send_usb_stats(server_ptr);
        }
    }
    else if (request_type == NX_HTTP_SERVER_POST_REQUEST)
    {
        /* --- API: login --- */
        if (strcmp(resource, "/api/login") == 0)
        {
            return http_handle_login(server_ptr, packet_ptr);
        }

        /* --- API: logout --- */
        if (strcmp(resource, "/api/logout") == 0)
        {
            return http_handle_logout(server_ptr);
        }

        /* --- API: delete (JS uses POST now) --- */
        if (strncmp(resource, "/api/delete", 11) == 0)
        {
            if (!g_logged_in)
            {
                return http_handle_auth_check(server_ptr);
            }

            CHAR path_param[MAX_PATH_LEN + 1];
            path_param[0] = '\0';

            /* 1) Try query ?path=... (old mode) */
            const CHAR * query = strchr(resource, '?');
            if (query != NX_NULL)
            {
                const CHAR * p = strstr(query + 1, "path=");
                if (p != NX_NULL)
                {
                    p += 5; /* skip "path=" */
                    CHAR raw[MAX_PATH_LEN + 1];
                    UINT i = 0;
                    while (*p && *p != '&' && i < MAX_PATH_LEN)
                    {
                        raw[i++] = *p++;
                    }
                    raw[i] = '\0';
                    url_decode(raw, path_param, sizeof(path_param));
                }
            }

            /* 2) Else: accept suffix /api/delete/<path> (JS mode) */
            if (path_param[0] == '\0')
            {
                const CHAR * p = resource + 11; /* after "/api/delete" */
                if (p[0] == '\0' || p[0] == '?')
                {
                    return http_send_error(server_ptr, "No path specified\r\n");
                }

                CHAR raw[MAX_PATH_LEN + 1];
                UINT i = 0;
                while (p[i] && p[i] != '?' && i < MAX_PATH_LEN)
                {
                    raw[i] = p[i];
                    i++;
                }
                raw[i] = '\0';
                url_decode(raw, path_param, sizeof(path_param));
            }

            if (path_param[0] == '\0')
            {
                return http_send_error(server_ptr, "No path specified\r\n");
            }

            return http_handle_delete(server_ptr, path_param);
        }

        if (strncmp(resource, "/api/upload", 11) == 0)
        {
            if (!g_logged_in)
            {
                return http_handle_auth_check(server_ptr);
            }
            return http_handle_upload_stream(server_ptr, resource, packet_ptr);
        }


        /* existing POST routes, e.g. /upload-chunk, /login (old) ... */
        if (strncmp(resource, "/upload-chunk", 13) == 0)
        {
            if (!g_logged_in)
            {
                return http_send_error(server_ptr, "Not logged in.\r\n");
            }
            return http_handle_upload_chunk(server_ptr, resource, packet_ptr);
        }
    }

    NX_PARAMETER_NOT_USED(packet_ptr);
    return NX_SUCCESS;
}

