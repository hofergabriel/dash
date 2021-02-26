default: 
	gcc dash.c -lm -o dash

doc:
	pdflatex doc.tex
	evince doc.pdf

