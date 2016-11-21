# Javier Santacruz (C) 2010
# Nombre del "proyecto"
NOM=iberogre-hackathon

# Fuentes 
TEX=${NOM}.tex
# TEX temporal
TEXTTEMP=${TEX}.temp
PDF=${NOM}.pdf 	# Pdf resultado
CTEX=pdflatex 	# Compilador latex

.PHONY: clean clear

# Compilar el tex en pdf
#    La dependencia es el .tex
#    Compilamos dos veces para que haga el índice
$(PDF): $(TEX) 
	$(CTEX) $(TEX)
	$(CTEX) $(TEX)

# Primero compila y después arranca evince
# para visualizar el pdf del tirón.
view: $(PDF)
	evince $(PDF) &

# Limpia los archivos temporales
clear: 
	rm -rf *.aux *.dvi *.log *.idx *.toc *.out *.temp 

# Limpia todo
clean: clear
	rm -rf *.pdf
