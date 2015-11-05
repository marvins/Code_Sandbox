all: resume.pdf cover-letter.pdf

resume.pdf: resume.tex
	pdflatex resume.tex
	pdflatex resume.tex

cover-letter.pdf: cover-letter.tex
	pdflatex $<
	pdflatex $<

open:
	open resume.pdf

clean:
	rm *.aux *.log *.out *.pdf

