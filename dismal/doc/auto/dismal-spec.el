(TeX-add-style-hook "dismal-spec"
 (lambda ()
    (TeX-add-symbols
     '("code" 1)
     '("tablehead" 1))
    (TeX-run-style-hooks
     "epstopdf"
     "pdftex"
     "hyperref"
     "graphicx"
     "longtable"
     "ulem"
     "wrapfig"
     "lastpage"
     "charter"
     "avant"
     "fontenc"
     "T1"
     "fancyhdr"
     "geometry"
     "latex2e"
     "scrbook10"
     "scrbook"
     "a4paper")))

