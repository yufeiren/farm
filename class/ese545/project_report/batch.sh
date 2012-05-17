#! /bin/bash

for f in bigview mem test_fig1 test_fig2
do
	a2ping --bboxfrom=compute-gs $f.pdf $f.eps
done

rm -rf ese545-report.aux ese545-report.bbl ese545-report.blg ese545-report.dvi ese545-report.log ese545-report.ps ese545-report.pdf

latex ese545-report
bibtex ese545-report
latex ese545-report
bibtex ese545-report
latex ese545-report
dvips ese545-report
ps2pdf ese545-report.ps ese545-report.pdf

rm -rf ese545-report.aux ese545-report.bbl ese545-report.blg ese545-report.dvi ese545-report.log ese545-report.ps
