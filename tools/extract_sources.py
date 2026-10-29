"""Extract plain text from course PDFs and PPTX files into sources/text/ so they can be read and cited.
Run with: .venv\\Scripts\\python.exe tools\\extract_sources.py
"""
import re
import sys
import zipfile
from pathlib import Path

from pypdf import PdfReader

sys.stdout.reconfigure(encoding="utf-8")

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "sources" / "blackboard_2026-27"
OUT = ROOT / "sources" / "text"


def pdf_text(path: Path) -> str:
    reader = PdfReader(str(path))
    parts = []
    for i, page in enumerate(reader.pages, 1):
        parts.append(f"\n--- page {i} ---\n{page.extract_text() or ''}")
    return "".join(parts)


def pptx_text(path: Path) -> str:
    parts = []
    with zipfile.ZipFile(path) as z:
        slides = sorted(
            (n for n in z.namelist() if re.match(r"ppt/slides/slide\d+\.xml$", n)),
            key=lambda n: int(re.search(r"(\d+)", n.rsplit("/", 1)[1]).group(1)),
        )
        for n in slides:
            xml = z.read(n).decode("utf-8", errors="ignore")
            paras = re.findall(r"<a:p>(.*?)</a:p>", xml, re.DOTALL)
            lines = ["".join(re.findall(r"<a:t>(.*?)</a:t>", p)) for p in paras]
            num = re.search(r"(\d+)", n.rsplit("/", 1)[1]).group(1)
            parts.append(f"\n--- slide {num} ---\n" + "\n".join(l for l in lines if l.strip()))
    return "".join(parts)


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    for path in sorted(SRC.rglob("*")):
        if path.suffix.lower() not in {".pdf", ".pptx"}:
            continue
        rel = path.relative_to(SRC)
        target = OUT / (str(rel).replace("\\", "__").replace("/", "__") + ".txt")
        try:
            text = pdf_text(path) if path.suffix.lower() == ".pdf" else pptx_text(path)
            target.write_text(f"SOURCE: {rel}\n{text}", encoding="utf-8")
            print(f"ok   {len(text):>7} chars  {rel}")
        except Exception as e:  # report and continue
            print(f"FAIL {rel}: {e}")


if __name__ == "__main__":
    main()
