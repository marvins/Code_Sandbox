TITLE=PhotogrammetryNotes

BAD_EXT=*.glg *.glo *.xdy *.gls *.bbl *.out *.blg *.idx *.log *.aux *.toc *.ilg *.ind

VIEWER=evince

BIBTEX_FILE=$(TITLE).bib
GLOSSARY_FILE=glossary.tex

INCLUDES=$(BIBTEX_FILE) $(GLOSSARY_FILE)

$(TITLE).pdf: $(TITLE).tex $(INCLUDES) Makefile
	pdflatex  $<
	pdflatex  $<
	makeindex $(TITLE)
	pdflatex  $<
	pdflatex  $<
	bibtex    $(TITLE)
	pdflatex  $<
	pdflatex  $<
	makeglossaries $(TITLE)
	pdflatex  $<
	pdflatex  $<

show: 
	$(VIEWER) $(TITLE).pdf

trim:
	rm $(BAD_EXT)

clean:
	rm $(BAD_EXT) $(TITLE).pdf
