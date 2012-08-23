TITLE=PhotogrammetryNotes

$(TITLE).pdf: $(TITLE).tex
	pdflatex  $<
	pdflatex  $<
	makeindex $(TITLE)
	pdflatex  $<
	bibtex    $(TITLE)
	pdflatex  $<
	pdflatex  $<


clean:
	rm *.idx *.log *.pdf *.aux *.toc *.ilg *.ind
