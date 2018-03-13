<CsoundSynthesizer>
;<CsOptions>
;</CsOptions>
; ==============================================
<CsInstruments>

sr	=	44100
ksmps	=	1
nchnls	=	2
0dbfs	=	1

instr 1	

kamp = 1
kcps = 440
ifn  = p4

asig oscil kamp, kcps, ifn
     outs asig,asig

endin

</CsInstruments>
; ==============================================
<CsScore>

f1 0 32768 10 1

i 1 0 10 1

</CsScore>
</CsoundSynthesizer>

