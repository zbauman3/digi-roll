// DigiRoll bitmap editor — plain JS, no build step.
// Each pixel stores an integer level from 0 (off) to 2^depth - 1 (full brightness).

const els = {
  width: document.getElementById('width'),
  height: document.getElementById('height'),
  depth: document.getElementById('depth'),
  clear: document.getElementById('clear'),
  palette: document.getElementById('palette'),
  grid: document.getElementById('grid'),
};

const state = {
  width: 16,
  height: 16,
  depth: 1,
  pixels: new Uint8Array(16 * 16),
  brush: 1, // level to paint; 0 = eraser
};

const maxLevel = () => (1 << state.depth) - 1;

// ---- Rendering ----

function renderGrid() {
  renderBitmap(els.grid, state);
}

function renderPalette() {
  els.palette.replaceChildren();
  const add = (level, label) => {
    const b = document.createElement('button');
    b.type = 'button';
    b.className = 'swatch' + (level === 0 ? ' eraser' : '');
    b.title = level === 0 ? 'Eraser' : `Level ${level}`;
    if (label) b.textContent = label;
    else b.style.background = levelColor(level, state.depth);
    if (level === state.brush) b.classList.add('active');
    b.addEventListener('click', () => {
      state.brush = level;
      renderPalette();
    });
    els.palette.appendChild(b);
  };
  add(0, 'Eraser');
  for (let l = 1; l <= maxLevel(); l++) add(l);
}

function paintCell(index) {
  if (state.pixels[index] === state.brush) return;
  state.pixels[index] = state.brush;
  els.grid.children[index].style.background = levelColor(state.brush, state.depth);
}

// ---- Drawing (click + drag, mouse or touch) ----

let drawing = false;

function cellIndexAt(x, y) {
  const el = document.elementFromPoint(x, y);
  return el && el.classList.contains('cell') ? Number(el.dataset.index) : -1;
}

els.grid.addEventListener('pointerdown', (e) => {
  drawing = true;
  const i = cellIndexAt(e.clientX, e.clientY);
  if (i >= 0) paintCell(i);
});

window.addEventListener('pointermove', (e) => {
  if (!drawing) return;
  const i = cellIndexAt(e.clientX, e.clientY);
  if (i >= 0) paintCell(i);
});

window.addEventListener('pointerup', () => { drawing = false; });
window.addEventListener('pointercancel', () => { drawing = false; });

// ---- Controls ----

function clampInt(value, min, max, fallback) {
  const n = parseInt(value, 10);
  return Number.isNaN(n) ? fallback : Math.min(max, Math.max(min, n));
}

function resize() {
  const w = clampInt(els.width.value, 1, 64, state.width);
  const h = clampInt(els.height.value, 1, 64, state.height);
  els.width.value = w;
  els.height.value = h;
  if (w === state.width && h === state.height) return;

  // Keep the overlapping top-left region of the existing drawing.
  const next = new Uint8Array(w * h);
  for (let y = 0; y < Math.min(h, state.height); y++) {
    for (let x = 0; x < Math.min(w, state.width); x++) {
      next[y * w + x] = state.pixels[y * state.width + x];
    }
  }
  Object.assign(state, { width: w, height: h, pixels: next });
  renderGrid();
}

els.width.addEventListener('change', resize);
els.height.addEventListener('change', resize);

els.depth.addEventListener('change', () => {
  const oldMax = maxLevel();
  state.depth = Number(els.depth.value);
  const newMax = maxLevel();
  // Rescale existing levels so the drawing keeps its relative brightness.
  state.pixels = state.pixels.map((v) => Math.round((v / oldMax) * newMax));
  state.brush = state.brush === 0 ? 0 : newMax;
  renderPalette();
  renderGrid();
});

els.clear.addEventListener('click', () => {
  state.pixels.fill(0);
  renderGrid();
});

// ---- Download modal ----

const dl = {
  dialog: document.getElementById('download-dialog'),
  name: document.getElementById('export-name'),
  layout: document.getElementById('export-layout'),
  code: document.getElementById('export-code'),
  copy: document.getElementById('copy'),
  save: document.getElementById('save'),
};

const refreshExport = () => {
  dl.layout.disabled = state.depth === 1; // layouts are identical at 1-bit
  dl.code.value = generateC(state, dl.name.value, dl.layout.value);
};

document.getElementById('open-download').addEventListener('click', () => {
  refreshExport();
  dl.dialog.showModal();
});

dl.name.addEventListener('input', refreshExport);
dl.layout.addEventListener('change', refreshExport);

dl.copy.addEventListener('click', async () => {
  try {
    await navigator.clipboard.writeText(dl.code.value);
    dl.copy.textContent = 'Copied';
  } catch {
    dl.code.select();
    dl.copy.textContent = 'Press ⌘/Ctrl+C';
  }
  setTimeout(() => { dl.copy.textContent = 'Copy'; }, 1500);
});

dl.save.addEventListener('click', () => {
  const blob = new Blob([dl.code.value], { type: 'text/x-c' });
  const a = document.createElement('a');
  a.href = URL.createObjectURL(blob);
  a.download = `${toIdentifier(dl.name.value)}.h`;
  a.click();
  URL.revokeObjectURL(a.href);
});

// ---- Upload modal ----

const ul = {
  dialog: document.getElementById('upload-dialog'),
  file: document.getElementById('import-file'),
  code: document.getElementById('import-code'),
  error: document.getElementById('import-error'),
  load: document.getElementById('load'),
};

document.getElementById('open-upload').addEventListener('click', () => {
  ul.file.value = '';
  ul.code.value = '';
  ul.error.textContent = '';
  ul.dialog.showModal();
});

// Picking a file loads it straight away.
ul.file.addEventListener('change', async () => {
  const file = ul.file.files[0];
  if (!file) return;
  ul.code.value = await file.text();
  loadImport();
});

ul.load.addEventListener('click', loadImport);

function loadImport() {
  let result;
  try {
    result = parseC(ul.code.value);
  } catch (err) {
    ul.error.textContent = err.message;
    return;
  }
  Object.assign(state, {
    width: result.width,
    height: result.height,
    depth: result.depth,
    pixels: result.pixels,
  });
  if (state.brush > maxLevel()) state.brush = maxLevel();
  els.width.value = result.width;
  els.height.value = result.height;
  els.depth.value = String(result.depth);
  dl.name.value = result.name;
  if (result.depth > 1) dl.layout.value = result.layout;
  renderPalette();
  renderGrid();
  ul.dialog.close();
}

renderPalette();
renderGrid();
