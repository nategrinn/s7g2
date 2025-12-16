// file_manager.js
// Front-end for SecureFile Dashboard (USB file manager)

// ----------------------
// Globals & utilities
// ----------------------
let currentPath = "/";          // Always starts with leading "/", no trailing "/" (except root)
const API_BASE = "/api";

function fmtBytes(n) {
    const b = Number(n);
    const KB = 1000, MB = KB*1000, GB = MB*1000;

    if(b >= GB) return `${(b / GB).toFixed(0)} GB`;
    if(b >= MB) return `${(b / MB).toFixed(0)} MB`;
    if(b >= KB) return `${(b / KB).toFixed(0)} KB`;
    return `${b} B`;
}

async function refreshUsbUsage() {
    try {
        const r = await fetch('/api/usb_stats', { cache: 'no-store' });
        if (!r.ok) return;
        const s = await r.json();

        const pctEl = document.getElementById('usb-used-pct');
        const subEl = document.getElementById('usb-used-bytes');
        const donutEl = document.getElementById('usb-donut');
        if (!pctEl || !subEl || !donutEl) return;

        pctEl.textContent = `${s.used_pct}%`;
        subEl.textContent = `${fmtBytes(s.used)} / ${fmtBytes(s.total)}`;
        donutEl.style.setProperty('--p', String(s.used_pct));
    } catch (e) {
        // ignore (USB might be removed, etc.)
    }
}

let viewMode = localStorage.getItem('viewMode') || 'grid';

function applyViewMode() {
    const ul = document.getElementById('file-listing');
    const btn = document.getElementById('view-toggle-btn');
    if (!ul || !btn) return;

    ul.classList.toggle('grid', viewMode === 'grid');
    btn.innerHTML = (viewMode === 'grid')
        ? '<i class="fas fa-list"></i>'
        : '<i class="fas fa-th"></i>';
}

function toggleViewMode() {
    viewMode = (viewMode === 'grid') ? 'list' : 'grid';
    localStorage.setItem('viewMode', viewMode);
    applyViewMode();
}

function normalizePath(path) {
    if (!path) return "/";
    // Ensure leading slash
    if (!path.startsWith("/")) path = "/" + path;
    // Remove trailing slash (except root)
    while (path.length > 1 && path.endsWith("/")) {
        path = path.slice(0, -1);
    }
    return path;
}

function encodePathForEndpoint(path) {
    const norm = normalizePath(path || "/");
    if (norm === "/") return "";
    // Encode each segment but keep slashes
    return norm.split("/").map((seg, i) => (i === 0 ? "" : encodeURIComponent(seg))).join("/");
}


// ----------------------
// URL + History helpers (hard-lock Back at app root)
// ----------------------
function getUrlPath() {
    try {
        const url = new URL(window.location.href);
        const p = url.searchParams.get("path");
        return normalizePath(p || "/");
    } catch (_) {
        return "/";
    }
}

function setUrlPath(path, { replace = false, sentinel = false } = {}) {
    const norm = normalizePath(path || "/");
    const url = new URL(window.location.href);
    url.searchParams.set("path", norm);

    const state = { path: norm, __sentinel: !!sentinel };

    if (replace) {
        history.replaceState(state, "", url.toString());
    } else {
        history.pushState(state, "", url.toString());
    }
}

function joinPath(base, name) {
    base = normalizePath(base);
    if (base === "/") return "/" + name;
    return base + "/" + name;
}

function basename(path) {
    path = normalizePath(path);
    if (path === "/") return "/";
    const idx = path.lastIndexOf("/");
    return idx >= 0 ? path.substring(idx + 1) : path;
}

// ----------------------
// Preview helpers
// ----------------------
let previewObjectUrl = null;
let previewCurrentPath = null;

function extLower(name) {
    const i = name.lastIndexOf(".");
    return (i >= 0) ? name.slice(i + 1).toLowerCase() : "";
}

function guessMimeByExt(name) {
    const ext = extLower(name);
    if (ext === "pdf") return "application/pdf";
    if (ext === "mp4") return "video/mp4";
    if (ext === "avi") return "video/x-msvideo";
    if (ext === "zip") return "application/zip";
    if (ext === "nef") return "application/octet-stream"; // RAW, treat as download-only

    if (["png"].includes(ext)) return "image/png";
    if (["jpg", "jpeg"].includes(ext)) return "image/jpeg";
    if (["gif"].includes(ext)) return "image/gif";
    if (["webp"].includes(ext)) return "image/webp";
    if (["bmp"].includes(ext)) return "image/bmp";

    // treat many things as text
    if (["txt", "log", "csv", "json", "xml", "html", "htm", "css", "js", "c", "h", "cpp", "hpp", "md", "ini", "cfg", "yml", "yaml"].includes(ext)) {
        return "text/plain; charset=utf-8";
    }
    return "application/octet-stream";
}

function isPreviewable(name) {
    const ext = extLower(name);
    if (ext === "mp4") return true;
    if (ext === "pdf") return true;
    // keep these as download-only for now
    if (ext === "avi" || ext === "zip" || ext === "nef") return false;
    if (["png", "jpg", "jpeg", "gif", "webp", "bmp"].includes(ext)) return true;
    if (["txt", "log", "csv", "json", "xml", "html", "htm", "css", "js", "c", "h", "cpp", "hpp", "md", "ini", "cfg", "yml", "yaml"].includes(ext)) return true;
    return false;
}

function showPreviewModal(show) {
    if (!els.previewModal) return;
    els.previewModal.classList.toggle("hidden", !show);

    // optional: prevent background scrolling when modal open
    document.body.classList.toggle("modal-open", !!show);
}

function closePreview() {
    previewCurrentPath = null;

    if (previewObjectUrl) {
        URL.revokeObjectURL(previewObjectUrl);
        previewObjectUrl = null;
    }
    if (els.previewTitle) els.previewTitle.textContent = "Preview";
    if (els.previewBody) els.previewBody.innerHTML = "";
    showPreviewModal(false);
}

async function openPreview(fullPath, fileName) {
    if (!els.previewBody || !els.previewTitle) return;

    // Reset previous preview URL / state
    closePreview();

    previewCurrentPath = fullPath;
    els.previewTitle.textContent = fileName;

    showPreviewModal(true);
    els.previewBody.innerHTML = `<div style="color:#6c757d;">Loading preview…</div>`;

    const suffix = encodePathForEndpoint(fullPath);
    const ext = extLower(fileName);

    // ✅ STREAM MP4 directly (avoid downloading full file into a blob)
    if (ext === "mp4") {
        const url = `${API_BASE}/download${suffix}`;
        els.previewBody.innerHTML = `<video controls preload="metadata" src="${url}"></video>`;
        return;
    }

    // For non-preview types you can either show "no preview" or trigger download.
    // Example: NEF/ZIP/AVI => no preview.
    if (ext === "nef" || ext === "zip" || ext === "avi") {
        els.previewBody.innerHTML = `<div style="color:#6c757d;">No preview available for this file type.</div>`;
        return;
    }

    // Fetch the file (only for pdf/images/text)
    const res = await fetch(`${API_BASE}/download${suffix}`);
    if (!res.ok) {
        throw new Error(`Preview fetch failed: HTTP ${res.status}`);
    }

    const rawBlob = await res.blob();
    const mime = guessMimeByExt(fileName);

    // Fix missing/too-generic types by wrapping the blob
    const blob =
        (!rawBlob.type || rawBlob.type === "application/octet-stream")
            ? rawBlob.slice(0, rawBlob.size, mime)
            : rawBlob;

    const isText =
        mime.startsWith("text/") ||
        ["txt", "log", "csv", "json", "xml", "c", "h", "cpp", "hpp", "md", "ini", "cfg", "yml", "yaml", "js", "css", "html", "htm"].includes(ext);

    // Create object URL only when needed
    previewObjectUrl = URL.createObjectURL(blob);

    if (mime === "application/pdf") {
        els.previewBody.innerHTML = `<iframe src="${previewObjectUrl}"></iframe>`;
        return;
    }

    if (mime.startsWith("image/")) {
        els.previewBody.innerHTML = `<img alt="" src="${previewObjectUrl}">`;
        return;
    }

    if (isText) {
        const MAX = 256 * 1024;
        const slice = blob.size > MAX ? blob.slice(0, MAX) : blob;
        const text = await slice.text();
        const note = blob.size > MAX ? `\n\n--- Preview truncated to ${MAX} bytes ---\n` : "";
        els.previewBody.innerHTML = `<pre></pre>`;
        els.previewBody.querySelector("pre").textContent = text + note;
        return;
    }

    // Fallback
    els.previewBody.innerHTML = `<div style="color:#6c757d;">No preview available for this file type.</div>`;
}

// ----------------------
// DOM helpers
// ----------------------
const els = {
    currentPathDisplay: document.getElementById("current-path-display"),
    fileList: document.getElementById("file-listing"),
    emptyMsg: document.getElementById("empty-folder-message"),
    searchInput: document.getElementById("search-files"),
    newFolderBtn: document.getElementById("new-folder-btn"),
    uploadBtn: document.getElementById("upload-btn"),
    logoutBtn: document.getElementById("logout-btn"),
    directoryTree: document.getElementById("directory-tree"),

    previewModal: document.getElementById("preview-modal"),
    previewBackdrop: document.getElementById("preview-backdrop"),
    previewTitle: document.getElementById("preview-title"),
    previewBody: document.getElementById("preview-body"),
    previewCloseBtn: document.getElementById("preview-close-btn"),
    previewDownloadBtn: document.getElementById("preview-download-btn"),

};

function setCurrentPathDisplay() {
    // Show with trailing "/" purely for display
    els.currentPathDisplay.textContent =
        currentPath === "/" ? "/" : currentPath + "/";
}

// Simple toast-ish alert – can be replaced later with nicer UI
function showError(msg) {
    console.error(msg);
    alert(msg);
}

function uploadProgressShow() {
    const box = document.getElementById("upload-progress");
    const bar = document.getElementById("upload-progress-bar");
    if (!box || !bar) return;

    bar.style.width = "0%";
    box.style.display = "block";
}

function uploadProgressUpdate(sentBytes, totalBytes) {
    const bar = document.getElementById("upload-progress-bar");
    if (!bar || !totalBytes) return;
    const pct = Math.min(100, Math.floor((sentBytes * 100) / totalBytes));
    bar.style.width = pct + "%";
}

function uploadProgressHide() {
    const box = document.getElementById("upload-progress");
    if (!box) return;
    // small delay so user sees 100%
    setTimeout(() => { box.style.display = "none"; }, 500);
}

function sanitizeFilenameForDevice(name) {
    // Remove path fragments and unsafe chars
    name = (name || "").split(/[\/\\]/).pop().replace(/[^A-Za-z0-9._-]/g, "_");

    const max = 14;

    // Keep extension if possible
    const dot = name.lastIndexOf(".");
    if (dot > 0 && dot < name.length - 1) {
        const base = name.slice(0, dot);
        const ext = name.slice(dot); // includes '.'

        const keepBase = Math.max(1, max - ext.length);
        return base.slice(0, keepBase) + ext.slice(0, max - keepBase);
    }

    return name.slice(0, max);
}

const MAX_NAME_CHARS = 14;

function makeSafeUploadName(original) {
    let name = (original || "").split(/[\/\\]/).pop();

    // strip accents (éà -> ea)
    name = name.normalize("NFD").replace(/[\u0300-\u036f]/g, "");

    // replace unsafe chars including spaces
    name = name.replace(/[^A-Za-z0-9._-]/g, "_");

    // enforce 14 chars while preserving extension if possible
    if (name.length <= MAX_NAME_CHARS) return name;

    const dot = name.lastIndexOf(".");
    if (dot > 0 && dot < name.length - 1) {
        const base = name.slice(0, dot);
        const ext = name.slice(dot);
        const baseMax = Math.max(1, MAX_NAME_CHARS - ext.length);
        return base.slice(0, baseMax) + ext;
    }
    return name.slice(0, MAX_NAME_CHARS);
}

async function apiRename(fromFullPath, toNameOnly) {
    const body = `from=${encodeURIComponent(fromFullPath)}&to=${encodeURIComponent(toNameOnly)}`;
    const res = await fetch("/api/rename", {
        method: "POST",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
        body
    });
    if (!res.ok) {
        const txt = await res.text().catch(() => "");
        throw new Error(`Rename failed (HTTP ${res.status}) ${txt}`);
    }
}

// ----------------------
// Backend calls
// ----------------------
async function apiAuthCheck() {
    const res = await fetch(`${API_BASE}/auth_check`, { cache: "no-store" });

    if (res.status === 401) {
        // Not logged in
        const path = window.location.pathname || "";

        // If we are already on the login page, don't keep redirecting
        const onLogin =
            path.endsWith("/index.html") ||
            path === "/" || path === "";

        if (!onLogin) {
            // We are on app.html (or something else) → go to login
            window.location.href = "/index.html";
        }

        throw new Error("Not authenticated");
    }

    if (!res.ok) {
        throw new Error("Auth check failed");
    }

    return res.json();
}

// Wrapper that redirects to login on 401 for all API calls
async function authFetch(url, options) {
    const res = await fetch(url, options);
    if (res.status === 401) {
        // Session expired / not logged in → go back to login page
        window.location.href = "/index.html";
        throw new Error("Not authenticated");
    }
    return res;
}

async function apiList(path) {
    const suffix = encodePathForEndpoint(path || "/");
    const res = await fetch(`${API_BASE}/files${suffix}`, { cache: "no-store" });
    if (!res.ok) {
        throw new Error(`Failed to list directory: HTTP ${res.status}`);
    }
    return res.json(); // { path: "/...", entries: [ {name,type,size}, ... ] }
}

async function apiDownload(fullPath) {
    const suffix = encodePathForEndpoint(fullPath);
    const res = await fetch(`${API_BASE}/download${suffix}`);
    if (!res.ok) {
        throw new Error(`Download failed: HTTP ${res.status}`);
    }
    const blob = await res.blob();
    const a = document.createElement("a");
    a.href = URL.createObjectURL(blob);
    a.download = basename(fullPath);
    document.body.appendChild(a);
    a.click();
    a.remove();
    setTimeout(() => URL.revokeObjectURL(a.href), 1000);
}

async function apiDelete(fullPath) {
    const suffix = encodePathForEndpoint(fullPath);
    const res = await fetch(`${API_BASE}/delete${suffix}`, {
        method: "POST"
    });
    if (!res.ok) {
        throw new Error(`Delete failed: HTTP ${res.status}`);
    }
    const data = await res.json().catch(() => ({}));
    if (data && data.error) {
        throw new Error(data.error);
    }
}

// Chunked upload to /upload-chunk/{offset}/{eof}/{path+name}
const CHUNK_SIZE = 65536; /*16384;*/

async function uploadFileToCurrentPath(file) {
    const total = file.size;
    const basePath = currentPath === "/" ? "" : currentPath.slice(1); // remove leading "/"

    const safeName = makeSafeUploadName(file.name);

    // Prefer single-request streaming upload (faster, progress via XHR),
    // but keep chunked upload as a fallback.
    uploadProgressShow();
    uploadProgressUpdate(0, total);

    const STREAM_MAX = 64 * 1024; // 64KB (safe starting point)

    try {
        if (file.size <= STREAM_MAX) {
            await uploadFileStreamToCurrentPath(file, safeName);
        } else {
            throw new Error("too big for stream");
        }
    } catch (e) {
        console.warn("Stream upload skipped/failed, falling back to chunked:", e);
        await uploadFileChunkedToCurrentPath(file, basePath, safeName);
    }

    await refreshUsbUsage();
    uploadProgressHide();
}

function uploadFileStreamToCurrentPath(file, safeName) {
    const total = file.size;

    return new Promise((resolve, reject) => {
        const url =
            `${API_BASE}/upload?path=${encodeURIComponent(currentPath)}&name=${encodeURIComponent(safeName)}`;

        const xhr = new XMLHttpRequest();
        xhr.open("POST", url, true);

        xhr.setRequestHeader("X-File-Name", safeName);
        xhr.setRequestHeader("X-File-Path", currentPath);

        // Progress of bytes sent from browser to the board
        xhr.upload.onprogress = (evt) => {
            if (evt && evt.lengthComputable) {
                uploadProgressUpdate(evt.loaded, evt.total);
            } else {
                // Fallback if total not available
                uploadProgressUpdate(Math.min(total, evt.loaded || 0), total);
            }
        };

        xhr.onload = () => {
            if (xhr.status >= 200 && xhr.status < 300) {
                resolve();
            } else {
                reject(new Error(`Stream upload failed (HTTP ${xhr.status}) ${xhr.responseText || ""}`));
            }
        };

        xhr.onerror = () => reject(new Error("Stream upload failed (network error)"));
        xhr.send(file);
    });
}

// Chunked upload to /upload-chunk/{offset}/{eof}/{path+name}
async function uploadFileChunkedToCurrentPath(file, basePath, safeName) {
    const total = file.size;
    const encodedName = encodeURIComponent(safeName);

    let offset = 0;
    let chunkIndex = 0;

    while (offset < total) {
        const end = Math.min(offset + CHUNK_SIZE, total);
        const eof = end >= total ? 1 : 0;
        const chunk = file.slice(offset, end);

        let url = `/upload-chunk/${offset}/${eof}`;
        if (basePath) {
            url += "/" + basePath;
        }
        url += "/" + encodedName;

        const res = await fetch(url, { method: "POST", body: chunk });
        if (!res.ok) {
            const txt = await res.text().catch(() => "");
            throw new Error(`Upload chunk ${chunkIndex} failed (HTTP ${res.status}) ${txt}`);
        }

        offset = end;
        chunkIndex++;

        uploadProgressUpdate(offset, total);

        // small pacing helps NetX (optional)
        await new Promise(r => setTimeout(r, 1));
    }
}

// ----------------------
// Rendering functions
// ----------------------
function renderDirectoryTree(path) {
    // For now, keep simple: root only + maybe current path
    // You can later extend this to show nested tree.
    const ul = els.directoryTree.querySelector(".tree-list");
    ul.innerHTML = `
        <li><i class="fas fa-home"></i> <a href="#" data-path="/">Root Folder</a></li>
    `;
    if (path !== "/" && path !== "") {
        const label = basename(path);
        const li = document.createElement("li");
        li.innerHTML =
            `<i class="fas fa-folder-open"></i> ` +
            `<a href="#" data-path="${path}">${label}</a>`;
        ul.appendChild(li);
    }
}

function renderFileList(entries) {
    const list = els.fileList;
    list.innerHTML = "";

    if (!entries || !entries.length) {
        els.emptyMsg.style.display = "block";
        return;
    }
    els.emptyMsg.style.display = "none";

    // Sort: folders first, then files, both alphabetically
    const sorted = entries.slice().sort((a, b) => {
        if (a.type === "dir" && b.type !== "dir") return -1;
        if (a.type !== "dir" && b.type === "dir") return 1;
        return a.name.localeCompare(b.name, undefined, { sensitivity: "base" });
    });

    const filter = (els.searchInput.value || "").toLowerCase();

    for (const entry of sorted) {
        if (filter && !entry.name.toLowerCase().includes(filter)) {
            continue;
        }

        const li = document.createElement("li");
        li.className = "file-item " + (entry.type === "dir" ? "folder" : "file");

        const fullPath = joinPath(currentPath, entry.name);

        const iconHtml = entry.type === "dir"
            ? '<i class="fas fa-folder"></i>'
            : '<i class="fas fa-file"></i>';

        const sizeText = entry.type === "dir"
            ? "--"
            : (entry.size != null ? `${entry.size} B` : "");

        li.innerHTML = `
            <div class="file-name-col">
                <span class="file-icon">${iconHtml}</span>
                <a href="#" class="item-name-link">${entry.name}</a>
            </div>
            <div class="file-size-col">${sizeText}</div>
            <div class="item-actions">
                ${entry.type === "file"
                ? '<button class="action-btn download-btn" title="Download"><i class="fas fa-download"></i></button>'
                : ""}
                <button class="action-btn delete-btn" title="Delete"><i class="fas fa-trash"></i></button>
            </div>
        `;

        // Click to open folder / download file
        const nameLink = li.querySelector(".item-name-link");
        nameLink.addEventListener("click", async (e) => {
            e.preventDefault();
            if (entry.type === "dir") {
                await navigateTo(fullPath);
            } else {
                try {
                    if (isPreviewable(entry.name)) {
                        await openPreview(fullPath, entry.name);
                    } else {
                        await apiDownload(fullPath);
                    }
                } catch (err) {
                    showError(err.message);
                }
            }
        });

        // Download button
        const dlBtn = li.querySelector(".download-btn");
        if (dlBtn) {
            dlBtn.addEventListener("click", async (e) => {
                e.stopPropagation();
                try {
                    await apiDownload(fullPath);
                } catch (err) {
                    showError(err.message);
                }
            });
        }

        // Delete button
        const delBtn = li.querySelector(".delete-btn");
        delBtn.addEventListener("click", async (e) => {
            e.stopPropagation();
            const confirmMsg = entry.type === "dir"
                ? `Delete folder "${entry.name}" (and its content, if supported by device)?`
                : `Delete file "${entry.name}"?`;
            if (!confirm(confirmMsg)) return;
            try {
                await apiDelete(fullPath);
                await refreshCurrent();
                await refreshUsbUsage();
            } catch (err) {
                showError(err.message);
            }
        });

        list.appendChild(li);
    }
}

// ----------------------
// Navigation
// ----------------------
async function loadDirectory(path) {
    const normalized = normalizePath(path);
    const data = await apiList(normalized);
    // Trust the backend path if provided
    const backendPath = (data && typeof data.path === "string") ? normalizePath(data.path) : normalized;
    currentPath = (backendPath === normalized) ? backendPath : normalized;
    setCurrentPathDisplay();
    renderDirectoryTree(currentPath);
    renderFileList(data.entries || []);
    applyViewMode();
}

async function navigateTo(path, pushHistory = true) {
    const normalized = normalizePath(path);

    closePreview();

    // Create a history entry so browser Back goes to previous folder (not login)
    if (pushHistory) {
        // Push a normal (non-sentinel) history entry so Back navigates folders.
        setUrlPath(normalized, { replace: false, sentinel: false });
    }

    try {
        await loadDirectory(normalized);
    } catch (err) {
        showError(err.message);
    }
}

async function refreshCurrent() {
    // Refresh should not add a new history entry
    return navigateTo(currentPath, false);
}
// ----------------------
// Event wiring
// ----------------------
function setupEvents() {
    // Directory tree clicks
    els.directoryTree.addEventListener("click", (e) => {
        const link = e.target.closest("a[data-path]");
        if (!link) return;
        e.preventDefault();
        const path = link.getAttribute("data-path") || "/";
        navigateTo(path);
    });

    // Search filter
    els.searchInput.addEventListener("input", () => {
        // Just re-render with the current entries in memory by reloading
        refreshCurrent();
    });

    // New folder
    els.newFolderBtn.addEventListener("click", async () => {
        const name = prompt("New folder name:");
        if (!name) return;
        // We implement "new folder" as uploading an empty marker file named "foldername/.keep"
        // because the backend currently exposes files and folders only from FileX.
        // If you later add a dedicated /api/mkdir endpoint, plug it here instead.
        const pseudoFile = new File([""], ".dir_" + name, { type: "application/octet-stream" });
        const oldPath = currentPath;
        const newPath = joinPath(currentPath, name);
        try {
            // Create directory by uploading into "currentPath/name" will cause FileX to create directories
            // along the path if your upload handler does that. If not, you can skip this feature.
            currentPath = newPath; // temporarily
            await uploadFileToCurrentPath(pseudoFile);
        } catch (err) {
            showError("This device firmware does not support creating folders via upload yet.");
        } finally {
            currentPath = oldPath;
            await refreshCurrent();
        }
    });

    // Upload button + hidden <input type=file>
    const fileInput = document.createElement("input");
    fileInput.type = "file";
    fileInput.multiple = false;
    fileInput.style.display = "none";
    document.body.appendChild(fileInput);

    els.uploadBtn.addEventListener("click", (e) => {
        e.preventDefault();
        fileInput.value = "";
        fileInput.click();
    });

    fileInput.addEventListener("change", async () => {
        if (!fileInput.files || !fileInput.files.length) return;
        const file = fileInput.files[0];
        try {
            await uploadFileToCurrentPath(file);
            await refreshCurrent();
            await refreshUsbUsage();
        } catch (err) {
            showError(err.message);
        }

    });

    // Logout
    els.logoutBtn.addEventListener("click", async (e) => {
        e.preventDefault();
        try {
            await authFetch(`${API_BASE}/logout`, { method: "POST" });
        } catch (e2) {
            // ignore
        }
        window.location.href = "/index.html";
    });

    // Close preview on backdrop click
    if (els.previewBackdrop) {
        els.previewBackdrop.addEventListener("click", (e) => {
            e.preventDefault();
            closePreview();
        });
    }

    // Preview panel close
    if (els.previewCloseBtn) {
        els.previewCloseBtn.addEventListener("click", (e) => {
            e.preventDefault();
            closePreview();
        });
    }

    // Preview panel download
    if (els.previewDownloadBtn) {
        els.previewDownloadBtn.addEventListener("click", async (e) => {
            e.preventDefault();
            if (!previewCurrentPath) return;
            try {
                await apiDownload(previewCurrentPath);
            } catch (err) {
                showError(err.message);
            }
        });
    }



    // ESC closes preview
    window.addEventListener("keydown", (e) => {
        if (e.key === "Escape") closePreview();
    });

    // View mode toggle (list/grid)
    const viewBtn = document.getElementById("view-toggle-btn");
    if (viewBtn) {
        viewBtn.addEventListener("click", (e) => {
            e.preventDefault();
            toggleViewMode();
        });
    }
    applyViewMode();
}

// ----------------------
// Init
// ----------------------
(async function init() {
    // If this page doesn't have the dashboard elements, do nothing.
    // This prevents any weird behaviour if file_manager.js ever gets
    // included on other pages like index.html.
    const fileListEl = document.getElementById("file-listing");
    if (!fileListEl) {
        return;
    }

    try {
        await apiAuthCheck();
    } catch (_) {
        // apiAuthCheck already redirected (or we are on login page),
        // so just stop here.
        return;
    }

    setupEvents();

    // Browser Back/Forward handler.
    // - Normal states: load their path.
    // - Sentinel state: re-push the current directory state so the user cannot exit to login via Back.
    window.addEventListener("popstate", (e) => {
        const st = e.state;

        if (st && st.__sentinel) {
            // User tried to go "back past" the first in-app page.
            // Re-add the current state and stay in the app.
            const p = getUrlPath();
            setUrlPath(p, { replace: false, sentinel: false });
            loadDirectory(p).catch(err2 => showError(err2.message));
            return;
        }

        const path = (st && typeof st.path === "string") ? st.path : getUrlPath();
        loadDirectory(path).catch(err2 => showError(err2.message));
    });

    // Initial path comes from URL (?path=...), default "/"
    const initialPath = getUrlPath();

    // Hard-lock the browser Back button at the app boundary:
    //
    // 1) Replace current history entry with a sentinel (same URL).
    // 2) Push a normal state for the initialPath.
    //    From now on, Back at the first normal state goes to sentinel, which we intercept and cancel.
    setUrlPath(initialPath, { replace: true, sentinel: true });
    setUrlPath(initialPath, { replace: false, sentinel: false });

    try {
        await loadDirectory(initialPath);
        await refreshUsbUsage();
    } catch (err) {
        showError(err.message);
    }
})();
