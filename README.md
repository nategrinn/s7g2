Project recap
Setup

Target / board

Renesas Synergy SK-S7G2 (S7G2 MCU, on-chip SRAM ~640 KB, plus external memories available on the kit).
Ethernet enabled, running NetX Duo HTTP Server.
USB Mass Storage (USB stick) mounted and accessed via FileX.
Optional QSPI was explored for credential storage, but you decided to deprioritize it for now.

Software stack
ThreadX (RTOS)
NetX Duo (TCP/IP)
NetX HTTP Server (web server)
FileX (file system on USB)

Your custom firmware endpoints: auth, file listing, download, upload (stream + chunk).

Threads / stacks (key points)
g_http_server0 internal thread stack is currently 5120 bytes (from your screenshot).
Critical rule learned: large temporary buffers (upload/download copy buffers) must not be allocated on the stack in request handlers, otherwise you get stack overflow → corrupted behavior (login stalls, resets).
Upload copy buffers should be static/global (g_upload_copybuf[]).
Download streaming uses a static buffer (e.g. 2–4 KB) to keep stack low.

NetX packet pools
You experimented with two packet pools (IP + HTTP), which pushed SRAM usage up (you saw ~91% RAM used, dominated by .bss).
Conclusion: for now, 1 web client is the realistic target to keep SRAM under control.

Most important: IP packet pool size/count impacts stability (login, receive path, uploads). HTTP server pool is TX-oriented “comfort” but not the real upload bottleneck.

Current situation

What works
USB stick mounts correctly (FileX sees files/dirs).
HTTP server runs and your UI loads.
/api/files returns valid JSON listings.
You added a UI “viewer” concept and a modal popup.

What was fixed recently
Directory entries now show correctly as directories (navigation works).
Back button no longer returns to login (history handling fixed).

Upload path improved:
Added /api/upload streaming endpoint.
Kept /upload-chunk as fallback.

Download path improved:
Added MIME handling (Content-Type, inline vs attachment).

You’re now adding HTTP Range support for /api/download (for MP4 seek/stream).

Current issues observed

Modal popup appearing after login and not centered:

Root causes were:

HTML class typo (preview-moadal vs preview-modal)

JS toggling wrong class (preview-hidden vs hidden)

leftover CSS from old “preview panel” conflicting with modal styles

MP4 preview needs Range for proper seeking; without Range it may play but not seek well.

What to do next (recommended order)

Finish / verify HTTP Range support on /api/download

Change download handler signature to accept packet_ptr.

Parse Range: bytes=... from request bytes (packet).

Serve:

200 OK for full file

206 Partial Content + Content-Range for ranges

416 for invalid ranges

In SSP 2.7.0, use file.fx_file_current_file_size instead of fx_file_size_get().

Stabilize RAM + networking for 1 client

Stick to one packet pool if SRAM is tight.

Keep buffers off stack; keep copy buffers small and static.

Monitor packet availability during upload/download (debug prints or counters).

Finalize the modal viewer

Fix the HTML class typo + JS class toggle mismatch.

Replace modal positioning with flex centering.

Remove duplicated “preview panel” CSS to avoid overrides.

Keep preview rules:

Preview: png/jpg/pdf/text + mp4 (direct <video src="/api/download/...">)

Download-only: nef/zip/avi

Improve uploads once streaming is stable

Prefer /api/upload for small/medium files.

Use chunked upload for large files if SRAM/packet pools are still tight.

Avoid huge HTTP_UPLOAD_BUFFER_SIZE on stack.

(Later) Authentication/QSPI

Once file transfers + UI are stable, revisit credential storage and robust login.

For now keep auth simple (even plain text) to reduce failure modes.

If you want, I can write a short “current configuration sheet” you can paste into your project README (packet pool sizes, stacks, endpoints list, UI behavior) based on your exact current values.

Proceed please.
Current configuration sheet (paste into README)
Project

Embedded File & Media Server on Renesas Synergy SK-S7G2 using ThreadX + NetX Duo + NetX HTTP Server + FileX + USB Mass Storage.

Hardware & memory

Board: Renesas Synergy SK-S7G2

Storage: USB stick (Mass Storage), mounted by FileX

Network: Ethernet (NetX Duo)

Internal SRAM: 640 KB

Observed RAM usage (e² studio): ~588 KB used (91%)

.bss ~ 528 KB (largest contributor; packet pools + static buffers)

heap ~ 50 KB

stack ~ 19 KB

External memories present on kit: SDRAM shown available (not currently used for big buffers/pools)

RTOS / threads / stacks

RTOS: ThreadX

HTTP server module: g_http_server0 (NetX Duo HTTP Server)

Internal thread stack size: 5120 bytes

Internal thread priority: 4

Time slicing: enabled (2 ticks)

Important rule: upload/download copy buffers must be static/global, not large locals, to avoid stack overflow and “random” stalls/resets.

NetX packet pools (current direction)

Target is 1 web client (SRAM constrained).

Prefer one shared packet pool when RAM is tight; multiple pools can push .bss too high.

Key stability factor: IP packet pool (RX path). Too small → login stalls / TCP resets during large transfers.

(Update this section with your final pool count/size once you settle them. If you tell me your current “payload size + count”, I’ll fill exact numbers.)

HTTP server settings (from configurator)

Multipart HTTP requests support: Disabled

Server socket window size: 2048 bytes (can be increased to 4096–8192 if packet pool allows)

Timeouts: 10 s (accept/receive/send/disconnect)

Max header field size: 256 bytes

Max connections in queue: 5

User/pass max length: 20 / 20

Min packet size in pool: 600 bytes

Firmware API endpoints
Auth

GET /api/auth_check

Returns “authenticated” status / forces redirect behavior

GET /api/auth_status

Returns JSON like: {"configured":true/false,"logged_in":true/false}

POST /api/login

Performs login, sets g_logged_in

POST /api/logout

Clears login

(QSPI credential storage is currently deprioritized; auth can be simplified while file transfer is stabilized.)

File management

GET /api/files?path=/... or GET /api/files/<path>

Returns JSON directory listing: [{name,type,size}, ...]

POST /api/delete?path=/... or POST /api/delete/<path>

Deletes file/dir

Downloads (now with streaming support)

GET /api/download?file=/... or GET /api/download/<path>

Returns file bytes

Sends correct Content-Type based on extension

Uses Content-Disposition: inline for previewable types (pdf/images/text/video/mp4) else attachment

HTTP Range support: Range: bytes=start-end → 206 Partial Content

Enables MP4 seeking/streaming

Uses file size from FX_FILE control block on SSP 2.7.0

Uploads

POST /api/upload?path=/...&name=...

Single-request streaming upload (fast path)

POST /upload-chunk/<offset>/<eof>/<name>

Chunked upload fallback (stability path for large files / limited packet pool)

Web UI behavior
Pages & routing

/index.html → login page

/app.html → main file manager UI

/file_manager.js → UI logic

File browsing

Directory tree + list view (and optional grid toggle if enabled).

Clicking a directory navigates.

Clicking a file opens a modal preview when supported.

Preview support (current policy)

Preview in modal:

Images: .png, .jpg, .jpeg

PDFs: .pdf

Text-like: .txt, .log, .json, .csv, .c, .h, etc.

Video: .mp4 via <video src="/api/download/..."> (Range enables seeking)

Download-only (no preview):

RAW photos: .NEF

Archives: .zip

Video: .avi (browser support unreliable)

Modal preview notes (known pitfalls fixed)

Modal must start hidden: class="preview-modal hidden"

JS must toggle the hidden class (not preview-hidden)

Remove leftover “preview panel” CSS to avoid overrides

Center modal with flexbox (display:flex + align-items/justify-content)

Current priorities / next steps

Finish verifying Range on /api/download with MP4 seeking tests.

Stabilize memory/network for 1 client (avoid packet starvation; keep buffers static and small).

Finalize modal preview (centered, opens only on click, closes via ESC/backdrop).

Upload performance tuning

Prefer /api/upload for small/medium files

Keep chunked upload for very large files until RAM is comfortable

Revisit QSPI credential storage after transfer/UI stability is proven.
