// DigiRoll bitmap animator — loops through loaded .h files at a chosen frame rate.

const els = {
  files: document.getElementById('files'),
  fps: document.getElementById('fps'),
  play: document.getElementById('play'),
  prev: document.getElementById('prev'),
  next: document.getElementById('next'),
  cellSize: document.getElementById('cell-size'),
  clear: document.getElementById('clear'),
  stage: document.getElementById('stage'),
  status: document.getElementById('status'),
  hint: document.getElementById('hint'),
  frames: document.getElementById('frames'),
};

// Ordered frames: { name, bitmap } or { name, error }. Errored frames are skipped when playing.
let frames = [];
let current = 0;
let timer = null;

const playable = () => frames.some((f) => f.bitmap);
const byName = (a, b) => a.name.localeCompare(b.name, undefined, { numeric: true });

// ---- Rendering ----

function renderStage() {
  const frame = frames[current];
  if (!frame || !frame.bitmap) {
    els.stage.replaceChildren();
    els.status.textContent = frames.length ? '' : 'No frames';
  } else {
    renderBitmap(els.stage, frame.bitmap);
    const { width, height, depth } = frame.bitmap;
    els.status.textContent = `Frame ${current + 1}/${frames.length} · ${frame.name} · ${width}×${height} · ${depth}-bit`;
  }
  [...els.frames.children].forEach((li, i) => li.classList.toggle('current', i === current));
}

function button(label, title, onClick) {
  const b = document.createElement('button');
  b.type = 'button';
  b.textContent = label;
  b.title = title;
  b.addEventListener('click', (e) => {
    e.stopPropagation();
    onClick();
  });
  return b;
}

function move(i, delta) {
  const j = i + delta;
  if (j < 0 || j >= frames.length) return;
  [frames[i], frames[j]] = [frames[j], frames[i]];
  if (current === i) current = j;
  else if (current === j) current = i;
  renderList();
}

function renderList() {
  els.hint.hidden = frames.length > 0;
  const items = frames.map((frame, i) => {
    const li = document.createElement('li');
    li.className = 'frame';
    li.addEventListener('click', () => {
      current = i;
      renderStage();
    });

    const index = document.createElement('span');
    index.className = 'index';
    index.textContent = i + 1;
    li.appendChild(index);

    if (frame.bitmap) {
      const thumb = document.createElement('div');
      thumb.className = 'grid';
      renderBitmap(thumb, frame.bitmap);
      li.appendChild(thumb);
    }

    const name = document.createElement('span');
    name.className = frame.error ? 'error' : 'name';
    name.textContent = frame.error ? `${frame.name}: ${frame.error}` : frame.name;
    name.title = name.textContent;
    li.appendChild(name);

    li.append(
      button('↑', 'Move up', () => move(i, -1)),
      button('↓', 'Move down', () => move(i, 1)),
      button('⧉', 'Duplicate', () => {
        frames.splice(i + 1, 0, frame);
        renderList();
      }),
      button('✕', 'Remove', () => {
        frames.splice(i, 1);
        if (current > i || current >= frames.length) current = Math.max(0, current - 1);
        if (!playable()) stop();
        renderList();
      }),
    );
    return li;
  });
  els.frames.replaceChildren(...items);
  renderStage();
}

// ---- Playback ----

function step(delta) {
  if (!playable()) return;
  do {
    current = (current + delta + frames.length) % frames.length;
  } while (!frames[current].bitmap);
  renderStage();
}

function fps() {
  const v = parseFloat(els.fps.value);
  return Number.isFinite(v) && v > 0 ? Math.min(60, v) : 4;
}

function start() {
  if (!playable()) return;
  clearInterval(timer);
  timer = setInterval(() => step(1), 1000 / fps());
  els.play.textContent = 'Pause';
}

function stop() {
  clearInterval(timer);
  timer = null;
  els.play.textContent = 'Play';
}

els.play.addEventListener('click', () => (timer ? stop() : start()));
els.prev.addEventListener('click', () => { stop(); step(-1); });
els.next.addEventListener('click', () => { stop(); step(1); });
els.fps.addEventListener('input', () => { if (timer) start(); });

// Space toggles playback; arrow keys step.
window.addEventListener('keydown', (e) => {
  if (e.target.matches('input, select, textarea, button')) return;
  if (e.code === 'Space') { e.preventDefault(); els.play.click(); }
  if (e.code === 'ArrowLeft') els.prev.click();
  if (e.code === 'ArrowRight') els.next.click();
});

// ---- Loading ----

async function addFiles(fileList) {
  const added = [];
  for (const file of [...fileList].sort(byName)) {
    try {
      added.push({ name: file.name, bitmap: parseC(await file.text()) });
    } catch (err) {
      added.push({ name: file.name, error: err.message });
    }
  }
  frames = frames.concat(added);
  renderList();
}

els.files.addEventListener('change', () => {
  addFiles(els.files.files);
  els.files.value = ''; // allow adding the same files again
});

els.clear.addEventListener('click', () => {
  stop();
  frames = [];
  current = 0;
  renderList();
});

const applyCellSize = () => els.stage.style.setProperty('--cell', `${els.cellSize.value}px`);
els.cellSize.addEventListener('input', applyCellSize);
applyCellSize();

window.addEventListener('dragover', (e) => {
  e.preventDefault();
  document.body.classList.add('dropping');
});
window.addEventListener('dragleave', (e) => {
  if (!e.relatedTarget) document.body.classList.remove('dropping');
});
window.addEventListener('drop', (e) => {
  e.preventDefault();
  document.body.classList.remove('dropping');
  addFiles(e.dataTransfer.files);
});

renderList();
