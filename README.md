## Current configuration sheet

### Project
**Embedded File & Media Server** on **Renesas Synergy SK-S7G2** using **ThreadX + NetX Duo + NetX HTTP Server + FileX + USB Mass Storage**.

---

### Hardware & memory
- **Board:** Renesas Synergy **SK-S7G2**
- **Storage:** USB stick (Mass Storage), mounted by **FileX**
- **Network:** Ethernet (**NetX Duo**)
- **Internal SRAM:** **640 KB**
- **Observed RAM usage (e² studio):** ~**588 KB used (91%)**
  - **.bss:** ~ **528 KB** (largest contributor; packet pools + static buffers)
  - **heap:** ~ **50 KB**
  - **stack:** ~ **19 KB**
- **External memories present on kit:** SDRAM shown available (**not currently used** for big buffers/pools)

---

### RTOS / threads / stacks
- **RTOS:** ThreadX
- **HTTP server module:** `g_http_server0` (NetX Duo HTTP Server)
- **Internal thread stack size:** **5120 bytes**
- **Internal thread priority:** **4**
- **Time slicing:** enabled (**2 ticks**)
- **Important rule:** upload/download copy buffers must be **static/global**, not **large locals**, to avoid stack overflow and “random” stalls/resets.

---

### NetX packet pools (current direction)
Target is **1 web client** (SRAM constrained).

- Prefer **one shared packet pool** when RAM is tight; multiple pools can push **.bss** too high.
- Key stability factor: **IP packet pool (RX path)**  
  Too small → login stalls / TCP resets during large transfers.

**TODO:** Update this section with your final pool count/size once you settle them.  
If you tell me your current **payload size + count**, I’ll fill exact numbers.

---

### HTTP server settings (from configurator)
- Multipart HTTP requests support: **Disabled**
- Server socket window size: **2048 bytes** (can be increased to **4096–8192** if packet pool allows)
- Timeouts: **10 s** (accept/receive/send/disconnect)
- Max header field size: **256 bytes**
- Max connections in queue: **5**
- User/pass max length: **20 / 20**
- Min packet size in pool: **600 bytes**

---

### Firmware API endpoints

#### Auth
- `GET  /api/auth_check`  
  Returns “authenticated” status / forces redirect behavior
- `GET  /api/auth_status`  
  Returns JSON like: `{"configured":true/false,"logged_in":true/false}`
- `POST /api/login`  
  Performs login, sets `g_logged_in`
- `POST /api/logout`  
  Clears login

> QSPI credential storage is currently deprioritized; auth can be simplified while file transfer is stabilized.

#### File management
- `GET  /api/files?path=/...` **or** `GET /api/files/<path>`  
  Returns JSON directory listing: `[{name,type,size}, ...]`
- `POST /api/delete?path=/...` **or** `POST /api/delete/<path>`  
  Deletes file/dir

#### Downloads (now with streaming support)
- `GET /api/download?file=/...` **or** `GET /api/download/<path>`  
  Returns file bytes  
  - Sends correct `Content-Type` based on extension
  - Uses `Content-Disposition: inline` for previewable types (pdf/images/text/video/mp4) else `attachment`
  - **HTTP Range support:** `Range: bytes=start-end` → **206 Partial Content**
  - Enables MP4 seeking/streaming
  - Uses file size from `FX_FILE` control block on **SSP 2.7.0**

#### Uploads
- `POST /api/upload?path=/...&name=...`  
  Single-request streaming upload (**fast path**)
- `POST /upload-chunk/<offset>/<eof>/<name>`  
  Chunked upload fallback (**stability path** for large files / limited packet pool)

---

### Web UI behavior

#### Pages & routing
- `/index.html` → login page
- `/app.html` → main file manager UI
- `/file_manager.js` → UI logic

#### File browsing
- Directory tree + list view (and optional grid toggle if enabled)
- Clicking a directory navigates
- Clicking a file opens a modal preview when supported

#### Preview support (current policy)

**Preview in modal:**
- Images: `.png`, `.jpg`, `.jpeg`
- PDFs: `.pdf`
- Text-like: `.txt`, `.log`, `.json`, `.csv`, `.c`, `.h`, etc.
- Video: `.mp4` via `<video src="/api/download/...">` (Range enables seeking)

**Download-only (no preview):**
- RAW photos: `.NEF`
- Archives: `.zip`
- Video: `.avi` (browser support unreliable)

#### Modal preview notes (known pitfalls fixed)
- Modal must start hidden: `class="preview-modal hidden"`
- JS must toggle the `hidden` class (not `preview-hidden`)
- Remove leftover “preview panel” CSS to avoid overrides
- Center modal with flexbox (`display:flex` + `align-items/justify-content`)
