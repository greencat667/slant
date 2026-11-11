#!/usr/bin/env python3
"""
Icon generator for Prompt Deck's menu: Draw Things (local Stable Diffusion
app, its Automatic1111-compatible API on port 7860) generates each icon,
then this thresholds it to pure black/white, resizes to ICON_SIZE x
ICON_SIZE, packs it MSB-first (device's drawBitmap() format), and writes
slant/mode_icons.h.

Regenerated at 96x96 (up from the original 80x80) - the panel is a 200x200
1.54" e-paper, and at 80px each icon only used ~11mm of that with visibly
blocky ink from being thresholded straight out of a 512x512 render. 96 is
the largest multiple of 8 (needed so ICON_SIZE // 8 packs with no row
padding, same reasoning as the 80x80 original) that still leaves
render_menu_item() in slant.ino enough room below the icon for the
worst-case wrapped label ("Sync Now: Stakeholder Perspective", 3 lines at
the smallest font candidate) - checked against slant.ino's actual
layout math before picking this, not just bumped up arbitrarily. Same
ICON_SIZE ember-watch's sibling project independently converged on.

Needs Pillow (pip install pillow) and a local Stable Diffusion server.
    python3 tools/gen_icons.py
"""
import base64
import io
import json
import urllib.request

from PIL import Image

DRAW_THINGS_URL = "http://127.0.0.1:7860"
GEN_SIZE = 512
ICON_SIZE = 96
STEPS = 6

STYLE = (
    "minimalist black ink line icon, single simple pictogram, app icon style, "
    "bold clean outline, no shading, no color, no text, pure black on white, "
    "centered, plenty of white space around it"
)
NEGATIVE = (
    "color, gradient, shading, photorealistic, text, watermark, blurry, "
    "grayscale, crosshatching, clutter, busy detail, signature, multiple objects"
)

# (C identifier, prompt subject) - the 13 real menu items (five modes have
# grown to eleven, plus Read Me, Sync Now, and Emily Dickinson). Steal From
# Elsewhere's magpie needs its long tail and a shiny stolen object called
# out explicitly - a magpie's usual signature (black/white plumage) doesn't
# read as a *shape* in outline-only line art (see README's "Icons" section).
ICONS = [
    ("ICON_READ_ME", "an open book"),
    ("ICON_OBLIQUE", "a single card tilted at an angle, drawn from a deck"),
    ("ICON_PREMORTEM", "a simple tombstone"),
    ("ICON_STAKEHOLDER", "three simple silhouette figures standing together"),
    ("ICON_CONSTRAINT", "a simple wireframe cube"),
    ("ICON_STEAL", "a magpie in flight with a long tail, clutching a single shiny gemstone in its beak"),
    ("ICON_VALUES", "a simple balance scale, evenly balanced"),
    ("ICON_ECOSYSTEM", "a magnifying glass held over a single leaf"),
    ("ICON_DECISION", "a single coin viewed edge-on, about to be flipped"),
    ("ICON_FUTURES", "a signpost with three arrows pointing in different directions"),
    ("ICON_SCALING", "three nested squares increasing in size, small to large"),
    ("ICON_SYNC", "two curved arrows forming a refresh/cycle symbol"),
    ("ICON_DICKINSON", "a single feather"),
]


def generate(prompt):
    payload = {
        "prompt": f"{STYLE}, {prompt}",
        "negative_prompt": NEGATIVE,
        "width": GEN_SIZE,
        "height": GEN_SIZE,
        "steps": STEPS,
    }
    req = urllib.request.Request(
        f"{DRAW_THINGS_URL}/sdapi/v1/txt2img",
        data=json.dumps(payload).encode("utf-8"),
        headers={"content-type": "application/json"},
        method="POST",
    )
    with urllib.request.urlopen(req, timeout=120) as resp:
        result = json.loads(resp.read().decode("utf-8"))
    png_bytes = base64.b64decode(result["images"][0])
    return Image.open(io.BytesIO(png_bytes)).convert("L")


def to_1bpp_bytes(img):
    img = img.resize((ICON_SIZE, ICON_SIZE), Image.LANCZOS)
    bw = img.point(lambda p: 255 if p >= 140 else 0)
    px = bw.load()
    row_bytes = ICON_SIZE // 8
    out = bytearray(row_bytes * ICON_SIZE)
    for y in range(ICON_SIZE):
        for bx in range(row_bytes):
            byte = 0
            for bit in range(8):
                x = bx * 8 + bit
                # Device convention (matches the original mode_icons.h):
                # bit=1 -> drawn/foreground (black ink), matching
                # Adafruit_GFX's drawBitmap() where a set bit paints.
                if px[x, y] < 128:  # black ink pixel
                    byte |= 0x80 >> bit
            out[y * row_bytes + bx] = byte
    return bytes(out)


def emit_header(icons_bytes, path):
    row_bytes = ICON_SIZE // 8
    size_bytes = row_bytes * ICON_SIZE
    lines = [
        "#ifndef MODE_ICONS_H",
        "#define MODE_ICONS_H",
        "",
        "#include <pgmspace.h>",
        "",
        f"#define ICON_SIZE {ICON_SIZE}  // {ICON_SIZE}x{ICON_SIZE}, {size_bytes} bytes each",
        "",
    ]
    for name, data in icons_bytes:
        lines.append(f"static const uint8_t {name}[{size_bytes}] PROGMEM = {{")
        for i in range(0, len(data), 16):
            chunk = data[i:i + 16]
            lines.append("  " + ", ".join(f"0x{b:02x}" for b in chunk) + ",")
        lines.append("};")
        lines.append("")
    lines.append("#endif")
    with open(path, "w") as f:
        f.write("\n".join(lines) + "\n")


def save_contact_sheet(icons_bytes, path):
    """Decode the packed bitmaps back into a visual contact sheet PNG so
    quality can be checked before committing to flash."""
    row_bytes = ICON_SIZE // 8
    cols = len(icons_bytes)
    sheet = Image.new("L", (ICON_SIZE * cols, ICON_SIZE), 255)
    for i, (name, data) in enumerate(icons_bytes):
        img = Image.new("L", (ICON_SIZE, ICON_SIZE), 255)
        px = img.load()
        for y in range(ICON_SIZE):
            for x in range(ICON_SIZE):
                byte = data[y * row_bytes + (x // 8)]
                bit = (byte >> (7 - (x % 8))) & 1
                px[x, y] = 0 if bit else 255
        sheet.paste(img, (i * ICON_SIZE, 0))
    sheet.save(path)


def main():
    icons_bytes = []
    for name, prompt in ICONS:
        print(f"generating {name} ({prompt})...")
        img = generate(prompt)
        data = to_1bpp_bytes(img)
        icons_bytes.append((name, data))
    out_path = "../slant/mode_icons.h"
    emit_header(icons_bytes, out_path)
    print(f"wrote {out_path}")
    save_contact_sheet(icons_bytes, "icon_preview.png")
    print("wrote icon_preview.png (contact sheet) for visual review")


if __name__ == "__main__":
    main()
