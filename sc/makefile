
all: generate compile

generate: risingNotes.sc
	sclang -d ./ risingNotes.sc

compile: risingNotes.osc
	scsynth -N risingNotes.osc _ ../wav/risingNotes.wav 8500 WAV int16 -o 1
