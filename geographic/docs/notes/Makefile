TITLE=PhotogrammetryNotes

$(TITLE).pdf: $(TITLE).tex
	pdflatex  $<
	pdflatex  $<
	makeindex $(TITLE)
	pdflatex  $<
	bibtex    $(TITLE)
	pdflatex  $<
	pdflatex  $<


trim:
	rm *.bbl *.out *.blg *.idx *.log *.aux *.toc *.ilg *.ind

clean:
	rm *.bbl *.out *.blg *.idx *.log *.pdf *.aux *.toc *.ilg *.ind
