// DigiRoll bitmap rendering. Shared by the editor and gallery.

// Off pixels are dark grey (an unlit LED); lit levels scale up to white.
function levelColor(level, depth) {
  if (level === 0) return '#222';
  const t = level / ((1 << depth) - 1);
  const v = Math.round(60 + t * (255 - 60));
  return `rgb(${v}, ${v}, ${v})`;
}

// Fill `container` (a .grid element) with one .cell per pixel.
// Each cell carries data-index so the editor can map pointer hits to pixels.
function renderBitmap(container, bitmap) {
  const { width, height, depth, pixels } = bitmap;
  container.style.gridTemplateColumns = `repeat(${width}, var(--cell))`;
  const cells = [];
  for (let i = 0; i < width * height; i++) {
    const cell = document.createElement('div');
    cell.className = 'cell';
    cell.dataset.index = i;
    cell.style.background = levelColor(pixels[i], depth);
    cells.push(cell);
  }
  container.replaceChildren(...cells);
}
