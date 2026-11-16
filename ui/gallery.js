// DigiRoll bitmap gallery — display-only. Load many .h files and render them side by side.

const els = {
  files: document.getElementById('files'),
  cellSize: document.getElementById('cell-size'),
  hint: document.getElementById('hint'),
  gallery: document.getElementById('gallery'),
};

const byName = (a, b) => a.name.localeCompare(b.name, undefined, { numeric: true });

function card(name, bitmap, error) {
  const fig = document.createElement('figure');
  fig.className = 'card';
  if (error) {
    const p = document.createElement('p');
    p.className = 'error';
    p.textContent = error;
    fig.appendChild(p);
  } else {
    const grid = document.createElement('div');
    grid.className = 'grid';
    renderBitmap(grid, bitmap);
    fig.appendChild(grid);
  }
  const caption = document.createElement('figcaption');
  caption.textContent = bitmap
    ? `${name} · ${bitmap.width}×${bitmap.height} · ${bitmap.depth}-bit`
    : name;
  fig.appendChild(caption);
  return fig;
}

// Each load replaces what's shown with the chosen files, sorted by name.
async function showFiles(fileList) {
  const files = [...fileList].sort(byName);
  const cards = [];
  for (const file of files) {
    try {
      cards.push(card(file.name, parseC(await file.text())));
    } catch (err) {
      cards.push(card(file.name, null, err.message));
    }
  }
  els.gallery.replaceChildren(...cards);
  els.hint.hidden = cards.length > 0;
}

els.files.addEventListener('change', () => showFiles(els.files.files));

const applyCellSize = () => els.gallery.style.setProperty('--cell', `${els.cellSize.value}px`);
els.cellSize.addEventListener('input', applyCellSize);
applyCellSize();

// Drag and drop anywhere on the page.
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
  showFiles(e.dataTransfer.files);
});
